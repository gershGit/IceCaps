/*
	Base class for collision testing objects
*/

#pragma once
#include "Headers.h"

//Forward declarations of necessary objects
class GameObject;
class SphereCollider;
class BoxCollider;
class MeshCollider;

//Structure that stores information relvant to single instance of a collision
struct collisionInfo {
	GameObject* collisionObject;	//Object collided with
	bool collision;					//True when a collision occured
	glm::vec3 collisionPosition;	//Vector of the point at which the collision occured
};
enum collider_type { SPHERE_COLLIDER, BOX_COLLIDER, MESH_COLLIDER };

class Collider {
public:
	//The game object this collider is attached to
	GameObject * object;
	//Specifies which collider type this object is
	collider_type type;

	Collider() {};
	~Collider() {};

	//Virtual functions to ensure all subclasses are required to handle all types of collisions
	virtual collisionInfo checkCollision(SphereCollider* collider) { return collisionInfo(); };
	virtual collisionInfo checkCollision(BoxCollider* collider) { return collisionInfo(); };
	virtual collisionInfo checkCollision(MeshCollider* collider) { return collisionInfo(); };
};