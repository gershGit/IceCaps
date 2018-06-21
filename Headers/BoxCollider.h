#include "Headers.h"
#include "Collider.h"

class BoxCollider : public Collider {
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	collisionInfo checkCollision(SphereCollider* collider);
	collisionInfo checkCollision(BoxCollider* collider);
	collisionInfo checkCollision(MeshCollider* collider);
};