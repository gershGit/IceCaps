#pragma once

#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"
#include "Core/ManagersFactories.h"

class CollisionSystem : public EntitySystem {
private:
	float collisionOffset = 1.001f;
	std::vector<collision>* lastFrameCollisions = new std::vector<collision>();
public:
	CollisionSystem() { operatesOn = { COLLIDER, TRANSFORM, RIGID_BODY }; entityListRequiredComponents = { {TRANSFORM, COLLIDER} }; };
	~CollisionSystem() {};

	collision checkCollision(int entityOne, int entityTwo);

	void applyCollisionPhysics(int entityID, int oneOrTwo, collision cInfo, collider thisCollider);

	void manageCollisionPhysics(int entityOneID, int entityTwoID, collision collisionInfo, MappedManager<rigid_body> * rManager, ArrayManager<transform> * tManager);

	void setLastCollisions(std::vector<collision>* lastCollisions);

	void addCollision(collision &detectedCol, VectorManager<collision> * manager);

	void addExitCollisions(VectorManager<collision> * manager);

	void onUpdate();
	void manageCollision(collision cInfo);
};