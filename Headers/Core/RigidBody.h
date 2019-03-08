/*
	Class to compute and update rigidbody physics
*/

#pragma once
#include "Headers.h"
#include "Property.h"
#include <vector>

class RigidBody : Property
{
public:
	//Information stored for every object
	const glm::vec3 gravity_acceleration = glm::vec3(0, -9.8f, 0);
	glm::vec3 lastAcceleration;
	glm::vec3 lastVelocity;
	glm::vec3 lastPosition;

public:
	//Public information that affects movement and collisions
	float mass;
	bool use_gravity = true;
	bool is_grounded = false;
	bool is_active = true;

	//Set the starting information for the rigidbody
	void setStart(glm::vec3 position, glm::vec3 velocity = glm::vec3(0), glm::vec3 acceleration = glm::vec3(0));
	//Updates the rigidbody due to current forces and acceleration
	void updateAll(glm::vec3 &returnPos, float timestep, glm::vec3 addedForce = glm::vec3(0));
	RigidBody();
	~RigidBody();
};
