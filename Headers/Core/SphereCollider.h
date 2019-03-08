/*
	Subclass of Collider
	Represents a spherical collision entity
*/

#include "Headers.h"
#include "Collider.h"

class SphereCollider : public Collider {
public:
	SphereCollider();
	~SphereCollider();

public:
	//Information on the shape of the collider
	glm::vec3 position;
	float radius;

	//Required collision functions
	collisionInfo checkCollision(SphereCollider* collider);
	collisionInfo checkCollision(BoxCollider* collider);
	collisionInfo checkCollision(MeshCollider* collider);
};