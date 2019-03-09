#include "Core/RigidBodySystem.h"
#include "Core/ManagersFactories.h"
#include "Core/GameTimer.h"

RigidBodySystem::RigidBodySystem() {
	operatesOn = { TRANSFORM, PREFAB_COMPONENT, RIGID_BODY };
	entityListRequiredComponents = { {TRANSFORM, RIGID_BODY} };
}

RigidBodySystem::~RigidBodySystem() {

}

void RigidBodySystem::onUpdate()
{
	RigidBodyManager* rManager = getRigidBodyManager(*managers);
	TransformManager* tManager = getTransformManager(*managers);
	std::vector<rigid_body>* rComponents = getRigidBodyManager(*managers)->getComponents();
	for (int i = 0; i < entities->size(); i++) {
		//Create a temporary body to store update information
		rigid_body tempBody = rManager->getComponent(entities->at(i));
		if (!tempBody.is_grounded) {
			//Calulate change in position
			glm::vec3 deltaPos = (float) GameTimer::getDeltaTime() * (tempBody.lastVelocity + (float) GameTimer::getDeltaTime() * gravity * 0.5f);

			//Update position and velocity
			tempBody.lastPosition += deltaPos;
			tempBody.lastVelocity += (float)GameTimer::getDeltaTime() * gravity;

			//Check for grounding
			if (tempBody.lastPosition.y <= groundPlane) {
				tempBody.lastVelocity.y = -tempBody.lastVelocity.y * tempBody.elasticity;
				//Check if now static
				if (glm::length(tempBody.lastVelocity) < staticSpeed) {
					tManager->transformArray[entities->at(i)].pos = tempBody.lastPosition;
					tempBody.is_grounded = true;
				}
			}
			
			//Update components
			tManager->transformArray[entities->at(i)].pos = tempBody.lastPosition;
			rManager->setComponent(entities->at(i), tempBody);
		}
	}
}
