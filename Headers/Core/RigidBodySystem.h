/*
	System to control rigid bodies and interactions on them
*/

#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"

class RigidBodySystem : public EntitySystem {
private:	
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);	//Gravitaional vector
public:
	float airResistance = 0.0001f;
	RigidBodySystem();
	~RigidBodySystem();

	void onUpdate();
};