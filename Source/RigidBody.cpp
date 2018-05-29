#include "..\Headers\RigidBody.h"



void RigidBody::setStart(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration)
{
}

void RigidBody::updateAll(glm::vec3 & returnPos, float timestep, glm::vec3 addedForce)
{
	if (!is_grounded) {
		lastPosition += timestep * (lastVelocity + timestep * gravity_acceleration * 0.5f);
		lastVelocity += timestep * gravity_acceleration;
		if (lastPosition.y <= 0) {
			returnPos = glm::vec3(lastPosition.x, 0, lastPosition.z);
			is_grounded = true;
		}
		else {
			returnPos = lastPosition;
		}
	}
}

RigidBody::RigidBody()
{
}
RigidBody::~RigidBody()
{
}