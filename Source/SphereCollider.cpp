#include "..\Headers\SphereCollider.h"



SphereCollider::SphereCollider()
{
}


SphereCollider::~SphereCollider()
{
}

collisionInfo SphereCollider::checkCollision(SphereCollider * collider)
{
	if ((collider->position - position).length < ((collider->position - glm::normalize(collider->position - position)*collider->radius) - (position - glm::normalize(position - collider->position)*radius)).length) {
		collisionInfo collision;
		collision.collision = true;
		collision.collisionObject = collider->object;
		std::cout << "Collision!!!" << std::endl;
		return collision;
	}
	return collisionInfo();
}
