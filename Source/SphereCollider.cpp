#include "..\Headers\SphereCollider.h"



SphereCollider::SphereCollider()
{
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::checkCollision(SphereCollider * collider)
{
	if ((collider->position - position).length < ((collider->position - glm::normalize(collider->position - position)*collider->radius) - (position - glm::normalize(position - collider->position)*radius)).length) {
		Collision collision;
		//collision.collisionObject = collider->object;
		return true;
	}
	return false;
}
