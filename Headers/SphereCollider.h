/*
	A subclass of collider that handles a sphere shaped collision detection system
*/

#pragma once
#include "Headers.h"
#include "Collider.h"

class SphereCollider : Collider
{
public:
	glm::vec3 position;
	float radius;	

	SphereCollider();
	~SphereCollider();

	bool checkCollision(SphereCollider* collider);

	//bool checkCollision(Collider* collider) override;
};
