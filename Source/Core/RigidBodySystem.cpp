#include "Core/RigidBodySystem.h"
#include "Core/ManagersFactories.h"
#include "Core/GameTimer.h"
#include "Core/StringTranslation.h"

//Set the entity system data
RigidBodySystem::RigidBodySystem() {
	operatesOn = { TRANSFORM, PREFAB_COMPONENT, RIGID_BODY, TAGS_COMPONENT };
	entityListRequiredComponents = { {TRANSFORM, RIGID_BODY} };
	systemType = RIGID_BODY_SYSTEM;
}

RigidBodySystem::~RigidBodySystem() {

}

//On update this system applies physics to entities using a rigid body model
void RigidBodySystem::onUpdate()
{
	CManager<rigid_body>* rManager = getCManager<rigid_body>(*managers, RIGID_BODY);
	ArrayManager<transform>* tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	ArrayManager<uint64_t>* tagManager = dynamic_cast<ArrayManager<uint64_t>*>(getCManager<uint64_t>(*managers, TAGS_COMPONENT));
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

			uint64_t currentTag = tagManager->getComponent(entities->at(i));
			tagManager->setComponent(entities->at(i), addTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED)));
		}
	}
}
