/*
	Subclass of Collider
	Represents an Axis-Aligned Bounding Box for collision detection
*/

#include "Headers.h"
#include "Collider.h"

class BoxCollider : public Collider {
public:
	BoxCollider();
	~BoxCollider();
public:
	//Position and scale used for determining edges of bounding box
	glm::vec3 position;
	glm::vec3 scale;
	//When rotated a new bounding box will be computed from the original bounding box (High performance but poorer than actual rotated boxes)
	glm::vec3 rotation;

	//Required collision tests
	collisionInfo checkCollision(SphereCollider* collider);
	collisionInfo checkCollision(BoxCollider* collider);
	collisionInfo checkCollision(MeshCollider* collider);
};