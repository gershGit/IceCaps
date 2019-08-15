#include "Core/CollisionSystem.h"
#include "Core/GameTimer.h"
#include "Core/ThreadPool.h"

//Checks to see if two entities collide in any fashion
collision CollisionSystem::checkCollision(int entityOne, int entityTwo) {
	collision collisionInfo = collision();
	collisionInfo.collision = false;
	collider *collider_one = cManager->getComponentAddress(entityOne);
	collider *collider_two = cManager->getComponentAddress(entityTwo);
	transform t1 = tManager->getComponent(entityOne);
	transform t2 = tManager->getComponent(entityTwo);

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

//Moves and adds physics to the objects involved in a collision
void CollisionSystem::applyCollisionPhysics(int entityID, int oneOrTwo, collision cInfo, collider thisCollider) {
	transform * t = tManager->getComponentAddress(entityID);
	rigid_body * rBody = rManager->getComponentAddress(entityID);
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

//Determines what entities should be physically affected by a collision
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
		applyCollisionPhysics(entityTwoID, 2, collisionInfo, cManager->getComponent(entityTwoID));
	}
	else if (rBodyTwo->isStatic) {
		applyCollisionPhysics(entityOneID, 1, collisionInfo, cManager->getComponent(entityTwoID));
	}
	else {
		//Dynamic dynamic collision
		//applyCollisionPhysics(rBodyOne, rBodyTwo, tManager->transformArray[entityOneID], tManager->transformArray[entityTwoID], collisionInfo);
	}
}

//Moves the collisions from last frame to an old list
void CollisionSystem::setLastCollisions(std::vector<collision> *lastCollisionsIn) {
	lastFrameCollisions->clear();
	for (collision c : *lastCollisionsIn) {
		if (c.state != COLLISION_EXIT) {
			lastFrameCollisions->push_back(c);
		}
	}
}

//Pushes a collision into the collision manager
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

//Adds exiting from all collisions to this frames found collisions
void CollisionSystem::addExitCollisions(VectorManager<collision>* manager) {
	for (collision c : *lastFrameCollisions) {
		c.state = COLLISION_EXIT;
		manager->addComponent(0, c);
	}
}

//Searches for any collisions with a nodes entities
void CollisionSystem::searchNodeForCollisions(SceneNode& scene_node)
{
	//Check for collisions between dynamic objects and objects within its bounds
	for (int i = 0; i < scene_node.dynamicEntities->size(); i++) {
		checkForCollisions(scene_node.dynamicEntities->at(i), scene_node);
		SceneNode* parent = getParentNode(scene, scene_node.id, config->sceneChildren);
		if (parent != nullptr) {
			checkForCollisionsInParent(scene_node.dynamicEntities->at(i), parent);
		} 
	}

	//Search all subscene_nodes for collisions
	if (!scene_node.isLeaf) {
		for (int i = 0; i < config->sceneChildren; i++) {
			auto searchFuncChild = std::bind(&CollisionSystem::searchNodeForCollisions, this, std::ref(scene_node.children[i]));
			ThreadPool::submitJob(searchFuncChild);
		}
	}
}

//Checks if the parent node has any collisions with the given node
void CollisionSystem::checkForCollisionsInParent(int entity, SceneNode * scene_node) {
	//Only need to check against static objects as parent dynamic objects will already have been checked
	for (int i = 0; i < scene_node->staticEntityCount; i++) {
		collision possibleCollision = checkCollision(entity, scene_node->staticEntities[i]);
		if (possibleCollision.collision) {
			listMutex.lock();
			addCollision(possibleCollision, clManager);
			listMutex.unlock();
			if (possibleCollision.state == COLLISION_ENTER) {
				manageCollisionPhysics(entity, scene_node->staticEntities[i], possibleCollision, rManager, tManager);
			}
		}
	}
}

//Checks for collisions within a node
void CollisionSystem::checkForCollisions(int entity, SceneNode & scene_node)
{
	//Checks to see if the entity collides with scene_node as a whole
	collider col = cManager->getComponent(entity);
	glm::vec3 pos = tManager->getComponent(entity).pos;
	AABB bounds = getBounds(col, pos);
	if (!boundsIntersect(scene_node.bounds, bounds)) {
		return;
	}

	//Check for collisions between the entity and static objects in the scene_node
	for (int i = 0; i < scene_node.staticEntityCount; i++) {
		collision possibleCollision = checkCollision(entity, scene_node.staticEntities[i]);
		if (possibleCollision.collision) {
			addCollision(possibleCollision, clManager);
			if (possibleCollision.state == COLLISION_ENTER) {
				manageCollisionPhysics(entity, scene_node.staticEntities[i], possibleCollision, rManager, tManager);
			}
		}
	}

	//Check for collisions between the entity and dynamic objects in the scene_node
	for (int i = 0; i < scene_node.dynamicEntities->size(); i++) {
		if (scene_node.dynamicEntities->at(i) != entity) {
			collision possibleCollision = checkCollision(entity, scene_node.dynamicEntities->at(i));
			if (possibleCollision.collision) {
				addCollision(possibleCollision, clManager);
				if (possibleCollision.state == COLLISION_ENTER) {
					manageCollisionPhysics(entity, scene_node.dynamicEntities->at(i), possibleCollision, rManager, tManager);
				}
			}
		}
	}

	//Check if the entity collides with any objects in subscene_nodes
	if (!scene_node.isLeaf) {
		for (int i = 0; i < config->sceneChildren; i++) {
			auto checkFunc = std::bind(&CollisionSystem::checkForCollisions, this, entity, std::ref(scene_node.children[i]));
			ThreadPool::submitJob(checkFunc);
		}
	}
}

//Initialize by setting the proper managers
void CollisionSystem::initialize()
{
	cManager = dynamic_cast<MappedManager<collider>*>(getCManager<collider>(*managers, COLLIDER));
	rManager = dynamic_cast<MappedManager<rigid_body>*>(getCManager<rigid_body>(*managers, RIGID_BODY));
	tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	clManager = dynamic_cast<VectorManager<collision>*>(getCManager<collision>(*managers, COLLISION));
	delete managers;
}

//On update this system handles collisions
void CollisionSystem::onUpdate()
{
	setLastCollisions(clManager->getComponents());
	clManager->getComponents()->clear();
	auto searchFunc = std::bind(&CollisionSystem::searchNodeForCollisions, this, std::ref(*scene));
	ThreadPool::submitJob(searchFunc);
	ThreadPool::workToComplete();
	addExitCollisions(clManager);
	for (collision cInfo : *clManager->getComponents()) {
		auto mFunc = std::bind(&CollisionSystem::manageCollision, this, cInfo);
		ThreadPool::submitJob(mFunc);
	}
	ThreadPool::workToComplete();
}

//User defined function for what should happen when two objects collide
void CollisionSystem::manageCollision(collision cInfo) {
	if (cInfo.state == COLLISION_ENTER) {
		std::cout << "Entering collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	}
	else if (cInfo.state == COLLISION_EXIT) {
		std::cout << "Exiting collision between " << cInfo.entityA << " and " << cInfo.entityB << std::endl;
	}
}