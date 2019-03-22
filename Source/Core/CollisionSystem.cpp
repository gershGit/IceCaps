#include "Core/CollisionSystem.h"
#include "Core/GameTimer.h"

collision CollisionSystem::checkCollision(int entityOne, int entityTwo) {
	collision collisionInfo = collision();
	collisionInfo.collision = false;
	collider *collider_one = &getCManager<collider>(*managers, COLLIDER)->getComponent(entityOne);
	collider *collider_two = &getCManager<collider>(*managers, COLLIDER)->getComponent(entityTwo);
	transform t1 = getCManager<transform>(*managers, TRANSFORM)->getComponent(entityOne);
	transform t2 = getCManager<transform>(*managers, TRANSFORM)->getComponent(entityTwo);

	if (collider_one->type == SPHERE_COLLIDER) {
		if (collider_two->type == SPHERE_COLLIDER) {
			glm::vec3 oneToTwo = t2.pos - t1.pos;
			if (glm::length(oneToTwo) < collider_one->radius + collider_two->radius) {
				collisionInfo.collision = true;
				collisionInfo.entityA = entityOne;
				collisionInfo.entityB = entityTwo;
				collisionInfo.collisionPointA = t1.pos + glm::normalize(oneToTwo)*collider_one->radius;
				collisionInfo.collisionPointB = t2.pos + -glm::normalize(oneToTwo)*collider_two->radius;
				collisionInfo.collisionNormalA = glm::normalize(oneToTwo);
				collisionInfo.collisionNormalB = -collisionInfo.collisionNormalA;
				return collisionInfo;
			}
		}
	}
	else if (collider_two->type == AABB_COLLIDER) {

	}
	return collisionInfo;
}

void CollisionSystem::applyCollisionPhysics(int entityID, int oneOrTwo, collision cInfo, collider thisCollider) {
	transform * t = &getCManager<transform>(*managers, TRANSFORM)->getComponent(entityID);
	rigid_body * rBody = getCManager<rigid_body>(*managers, RIGID_BODY)->getComponentAddress(entityID);
	glm::vec3 cPoint, cNormal;
	if (oneOrTwo == 1) {
		cPoint = cInfo.collisionPointB;
		cNormal = cInfo.collisionNormalB;
	}
	else {
		cPoint = cInfo.collisionPointA;
		cNormal = cInfo.collisionNormalA;
	}
	glm::vec3 cToP = glm::normalize(t->pos - cPoint);
	t->pos = t->pos + cToP * collisionOffset * (thisCollider.radius - glm::length(t->pos - cPoint));
	rBody->lastPosition = t->pos;

	float linearPortion = glm::dot(glm::normalize(-rBody->lastVelocity), cToP);

	glm::vec3 rotationForce = (1.0f - linearPortion) * -rBody->lastVelocity;
	
	rBody->rotationalVelocity += glm::cross(rotationForce, cToP);
	rBody->lastVelocity = glm::reflect(rBody->lastVelocity, cNormal) * rBody->elasticity * linearPortion;
}

void CollisionSystem::manageCollisionPhysics(int entityOneID, int entityTwoID, collision collisionInfo, MappedManager<rigid_body>* rManager, ArrayManager<transform>* tManager) {
	rigid_body* rBodyOne = nullptr;
	rigid_body* rBodyTwo = nullptr;
	if (rManager->hasEntity(entityOneID)) {
		rBodyOne = &rManager->getComponent(entityOneID);
	}
	if (rManager->hasEntity(entityTwoID)) {
		rBodyTwo = &rManager->getComponent(entityTwoID);
	}
	if ((rBodyOne == nullptr || rBodyOne->isStatic || rBodyOne->is_grounded) && (rBodyTwo->isStatic || rBodyTwo->is_grounded || rBodyTwo == nullptr)) {
		return;
	}
	else if (rBodyOne->isStatic) {
		applyCollisionPhysics(entityTwoID, 2, collisionInfo, getCManager<collider>(*managers, COLLIDER)->getComponent(entityTwoID));
	}
	else if (rBodyTwo->isStatic) {
		applyCollisionPhysics(entityOneID, 1, collisionInfo, getCManager<collider>(*managers, COLLIDER)->getComponent(entityTwoID));
	}
	else {
		//Dynamic dynamic collision
		//applyCollisionPhysics(rBodyOne, rBodyTwo, tManager->transformArray[entityOneID], tManager->transformArray[entityTwoID], collisionInfo);
	}
}

void CollisionSystem::setLastCollisions(std::vector<collision> *lastCollisionsIn) {
	lastFrameCollisions->clear();
	for (collision c : *lastCollisionsIn) {
		if (c.state != COLLISION_EXIT) {
			lastFrameCollisions->push_back(c);
		}
	}
}

void CollisionSystem::addCollision(collision &detectedCol, VectorManager<collision>* manager) {
	detectedCol.state = COLLISION_ENTER;
	for (int i = 0; i < lastFrameCollisions->size(); i++) {
		if (lastFrameCollisions->at(i).entityA == detectedCol.entityA && lastFrameCollisions->at(i).entityB == detectedCol.entityB ||
			lastFrameCollisions->at(i).entityB == detectedCol.entityA && lastFrameCollisions->at(i).entityA == detectedCol.entityB) {
			detectedCol.state = COLLISION_CONTINUE;
			lastFrameCollisions->erase(lastFrameCollisions->begin() + i);
		}
	}
	manager->addComponent(0, detectedCol);
}

void CollisionSystem::addExitCollisions(VectorManager<collision>* manager) {
	for (collision c : *lastFrameCollisions) {
		c.state = COLLISION_EXIT;
		manager->addComponent(0, c);
	}
}

void CollisionSystem::onUpdate()
{
	//TODO for ALL systems, this should be done once during initialization not during every update
	MappedManager<collider>* cManager = dynamic_cast<MappedManager<collider>*>(getCManager<collider>(*managers, COLLIDER));
	MappedManager<rigid_body>* rManager = dynamic_cast<MappedManager<rigid_body>*>( getCManager<rigid_body>(*managers, RIGID_BODY ));
	ArrayManager<transform>* tManager = dynamic_cast<ArrayManager<transform>*>( getCManager<transform>(*managers, TRANSFORM) );
	VectorManager<collision>* clManager = dynamic_cast<VectorManager<collision>*>( getCManager<collision>(*managers, COLLISION) );
	
	setLastCollisions(clManager->getComponents());
	clManager->getComponents()->clear();
	for (int i = 0; i < entities->size() - 1; i++) {
		for (int j = i+1; j < entities->size(); j++) {
			collision possibleCollision = checkCollision(entities->at(i), entities->at(j));
			if (possibleCollision.collision) {
				addCollision(possibleCollision, clManager);

				//Attempt to apply physics only if it is a new collision, as repeating collisions only occur if something besides physics is involved
				if (possibleCollision.state == COLLISION_ENTER) {
					manageCollisionPhysics(entities->at(i), entities->at(j), possibleCollision, rManager, tManager);
				}
			}
		}
	}
	addExitCollisions(clManager);
	for (collision cInfo : *clManager->getComponents()) {
		manageCollision(cInfo);
	}
}

void CollisionSystem::manageCollision(collision cInfo) {
	if (cInfo.state == COLLISION_ENTER) {
		std::cout << "Entering collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	}
	else if (cInfo.state == COLLISION_EXIT) {
		std::cout << "Exiting collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	}
}