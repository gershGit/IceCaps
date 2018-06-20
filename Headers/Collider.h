/*
	Base class for collision testing objects
*/

#pragma once
#include "Headers.h"

class GameObject;
struct collisionInfo {
	GameObject* collisionObject;
	bool collision;
	glm::vec3 collisionPosition;
};
enum collider_type {SPHERE_COLLIDER, BOX_COLLIDER, MESH_COLLIDER};

class Collider {
public:
	//GameObject * object;
	collider_type type;

	Collider() {};
	~Collider() {};

	//virtual bool checkCollision(Collider* collider){};
};