#include "Core/ComponentManager.h"

class CollisionManager : public ComponentManager {
private:
	std::vector<collision>* collisions = new std::vector<collision>();
public:
	CollisionManager() { type = COLLISION; };
	~CollisionManager() {};
	void addCollision(collision collision_in) { collisions->push_back(collision_in); };
	std::vector<collision>* getCollisions() { return collisions; };
};