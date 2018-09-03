#include "..\Headers\RigidBody.h"

void RigidBody::setStart(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration)
{
	lastPosition = position;
	lastVelocity = velocity;
	lastAcceleration = acceleration;
}

void RigidBody::updateAll(glm::vec3 & returnPos, float timestep, glm::vec3 addedForce)
{
	//std::cout << "Starting at " << returnPos.x << ", " << returnPos.y << ", " << returnPos.z << std::endl;
	//std::cout << "\t-->Timestep of " << timestep << std::endl;
	if (!is_grounded) {
		glm::vec3 change = timestep * (lastVelocity + timestep * gravity_acceleration * 0.5f);
		//std::cout << "\t-->Change of " << change.x << ", " << change.y << ", " << change.z << std::endl;

		//TODO fix this to match terrain or ground floor
		lastPosition += timestep * (lastVelocity + timestep * gravity_acceleration * 0.5f);
		lastVelocity += timestep * gravity_acceleration;
		if (lastPosition.y <= 0.4f) {
			lastVelocity.y = -lastVelocity.y/1.65;
			if (lastVelocity.y < 0.001f) {
				returnPos = glm::vec3(lastPosition.x, 0.4f, lastPosition.z);
				is_grounded = true;
			}
		}
		else {
			returnPos = lastPosition;
		}
	}
	//std::cout << "Ending at " << returnPos.x << ", " << returnPos.y << ", " << returnPos.z << std::endl;
}

RigidBody::RigidBody()
{
}
RigidBody::~RigidBody()
{
}