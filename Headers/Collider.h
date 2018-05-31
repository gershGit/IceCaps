/*
	Base class for collision testing objects
*/

#pragma once
#include "Headers.h"
//#include "GameObject.h"

enum collider_type {SPHERE_COLLIDER, BOX_COLLIDER, MESH_COLLIDER};
struct Collision  {
	//GameObject* collisionObject;
	glm::vec3 collisionPosition;
};


class Collider {
public:
	//GameObject * object;
	collider_type type;

	Collider() {};
	~Collider() {};

	//virtual bool checkCollision(Collider* collider){};
};