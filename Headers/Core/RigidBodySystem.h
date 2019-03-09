/*
	System to control rigid bodies and interactions on them
*/

#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"

class RigidBodySystem : public EntitySystem {
private:
	float groundPlane = 0.0f;	//Acts as a physical wall that objects cannot go below
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);	//Gravitaional vector
	float staticSpeed = 0.1f; //Speed at which objects become static
public:
	RigidBodySystem();
	~RigidBodySystem();

	void onUpdate();
};