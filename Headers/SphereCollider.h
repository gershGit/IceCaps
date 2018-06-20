/*
	A subclass of collider that handles a sphere shaped collision detection system
*/

#pragma once
#include "Headers.h"
#include "Collider.h"

class GameObject;

class SphereCollider : Collider
{
public:
	GameObject * object;
	glm::vec3 position;
	float radius;	

	SphereCollider();
	~SphereCollider();

	collisionInfo checkCollision(SphereCollider* collider);

	//bool checkCollision(Collider* collider) override;
};
