#pragma once
#include "Headers.h"
#include <vector>

class RigidBody
{
private:
	const glm::vec3 gravity_acceleration = glm::vec3(0, -9.8f, 0);
	glm::vec3 lastAcceleration;
	glm::vec3 lastVelocity;
	glm::vec3 lastPosition;

public:
	float mass;
	bool use_gravity = true;
	bool is_grounded = false;
	bool is_active = true;

	//Return 
	void setStart(glm::vec3 position, glm::vec3 velocity = glm::vec3(0), glm::vec3 acceleration = glm::vec3(0));
	void updateAll(glm::vec3 &returnPos, float timestep, glm::vec3 addedForce = glm::vec3(0));
	RigidBody();
	~RigidBody();
};
