#include "Core/CollisionSystem.h"
#include "Core/GameTimer.h"

collision CollisionSystem::checkCollision(int entityOne, int entityTwo) {
	collision collisionInfo = collision();
	collisionInfo.collision = false;
	collider *collider_one = &getColliderManager(*managers)->getComponent(entityOne);
	collider *collider_two = &getColliderManager(*managers)->getComponent(entityTwo);
	transform t1 = getTransformManager(*managers)->transformArray[entityOne];
	transform t2 = getTransformManager(*managers)->transformArray[entityTwo];

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
	transform * t = &getTransformManager(*managers)->transformArray[entityID];
	rigid_body * rBody = getRigidBodyManager(*managers)->getComponentAddress(entityID);
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
	if (glm::length(rBody->lastVelocity) < staticSpeed) {
		rBody->is_grounded = true;
	}
}

void CollisionSystem::manageCollisionPhysics(int entityOneID, int entityTwoID, collision collisionInfo, RigidBodyManager* rManager, TransformManager* tManager) {
	rigid_body* rBodyOne = nullptr;
	rigid_body* rBodyTwo = nullptr;
	if (rManager->hasEntity(entityOneID)) {
		rBodyOne = &rManager->getComponent(entityOneID);
	}
	if (rManager->hasEntity(entityTwoID)) {
		rBodyTwo = &rManager->getComponent(entityTwoID);
	}
	if ((rBodyOne == nullptr || rBodyTwo == nullptr) || (rBodyOne->isStatic && rBodyTwo->isStatic)) {
		return;
	}
	else if (rBodyOne->isStatic) {
		applyCollisionPhysics(entityTwoID, 2, collisionInfo, getColliderManager(*managers)->getComponent(entityTwoID));
	}
	else if (rBodyTwo->isStatic) {
		applyCollisionPhysics(entityOneID, 1, collisionInfo, getColliderManager(*managers)->getComponent(entityTwoID));
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

void CollisionSystem::addCollision(collision detectedCol, CollisionManager* manager) {
	detectedCol.state = COLLISION_ENTER;
	for (int i = 0; i < lastFrameCollisions->size(); i++) {
		if (lastFrameCollisions->at(i).entityA == detectedCol.entityA && lastFrameCollisions->at(i).entityB == detectedCol.entityB ||
			lastFrameCollisions->at(i).entityB == detectedCol.entityA && lastFrameCollisions->at(i).entityA == detectedCol.entityB) {
			detectedCol.state = COLLISION_CONTINUE;
			lastFrameCollisions->erase(lastFrameCollisions->begin() + i);
		}
	}
	manager->addCollision(detectedCol);
}

void CollisionSystem::addExitCollisions(CollisionManager* manager) {
	for (collision c : *lastFrameCollisions) {
		c.state = COLLISION_EXIT;
		manager->addCollision(c);
	}
}

void CollisionSystem::onUpdate()
{
	ColliderManager* cManager = getColliderManager(*managers);
	RigidBodyManager* rManager = getRigidBodyManager(*managers);
	TransformManager* tManager = getTransformManager(*managers);
	CollisionManager* clManager = getCollisionManager(*managers);
	
	setLastCollisions(clManager->getCollisions());
	clManager->getCollisions()->clear();
	for (int i = 0; i < entities->size() - 1; i++) {
		for (int j = i+1; j < entities->size(); j++) {
			collision possibleCollision = checkCollision(entities->at(i), entities->at(j));
			if (possibleCollision.collision) {
				addCollision(possibleCollision, clManager);
				manageCollisionPhysics(entities->at(i), entities->at(j), possibleCollision, rManager, tManager);
			}
		}
	}
	addExitCollisions(clManager);
	for (collision cInfo : *clManager->getCollisions()) {
		manageCollision(cInfo);
	}
}

void CollisionSystem::manageCollision(collision cInfo) {
	if (cInfo.state == COLLISION_ENTER) {
		std::cout << "Entering collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	} else if (cInfo.state == COLLISION_EXIT) {
		std::cout << "Exiting collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	}
}