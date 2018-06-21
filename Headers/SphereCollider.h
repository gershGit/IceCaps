#include "Headers.h"
#include "Collider.h"

class SphereCollider : public Collider {
public:
	glm::vec3 position;
	float radius;

	collisionInfo checkCollision(SphereCollider* collider);
	collisionInfo checkCollision(BoxCollider* collider);
	collisionInfo checkCollision(MeshCollider* collider);
};