#include "Core/RigidBodySystem.h"
#include "Core/ManagersFactories.h"
#include "Core/GameTimer.h"

RigidBodySystem::RigidBodySystem() {
	operatesOn = { TRANSFORM, PREFAB_COMPONENT, RIGID_BODY };
	entityListRequiredComponents = { {TRANSFORM, RIGID_BODY} };
	systemType = RIGID_BODY_SYSTEM;
}

RigidBodySystem::~RigidBodySystem() {

}

void RigidBodySystem::onUpdate()
{
	//std::cout << "Rigid Body update" << std::endl;
	CManager<rigid_body>* rManager = getCManager<rigid_body>(*managers, RIGID_BODY);
	ArrayManager<transform>* tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	for (int i = 0; i < entities->size(); i++) {
		//Create a temporary body to store update information
		rigid_body tempBody = rManager->getComponent(entities->at(i));
		if (!tempBody.isStatic && !tempBody.is_grounded) {
			tempBody.lastPosition = tManager->getComponent(entities->at(i)).pos;
			tempBody.lastRotation = tManager->getComponent(entities->at(i)).rot;

			//Calulate change in position
			glm::vec3 deltaPos = (float) GameTimer::getDeltaTime() * (tempBody.lastVelocity + (float) GameTimer::getDeltaTime() * gravity * 0.5f);

			//Update position and velocity
			tempBody.lastPosition += deltaPos;
			tempBody.lastVelocity += (float)GameTimer::getDeltaTime() * gravity;
			tempBody.lastVelocity *= (1.0f - airResistance);
 
			//Update rotation and rotational velocity
			tempBody.rotationalVelocity *= (1.0f - airResistance);
			tempBody.lastRotation += (float)GameTimer::getDeltaTime() * tempBody.rotationalVelocity;
			
			//Update components
			transform temp;
			temp.pos = tempBody.lastPosition;
			temp.rot = tempBody.lastRotation;
			temp.scale = tManager->getComponent(entities->at(i)).scale;

			tManager->setComponent(entities->at(i), temp);
			rManager->setComponent(entities->at(i), tempBody);
		}
	}
}
