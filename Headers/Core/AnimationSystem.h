/*
	System to run animations and update entities accordingly
*/

#pragma once
#include "Core/EntitySystem.h"
#include "Core/ManagersFactories.h"

class AnimationSystem : public EntitySystem {
public:
	AnimationSystem() { operatesOn = { TRANSFORM, ANIMATION_COMPONENT }; entityListRequiredComponents = { {TRANSFORM, ANIMATION_COMPONENT} }; systemType = ANIMATION_SYSTEM; };
	~AnimationSystem() {};
	void start();

	key_frame* getFramePrior(animation *anim, double time);
	key_frame* getFramePost(animation *anim, double time);
	glm::vec3 linearInterpolation(glm::vec3 start, glm::vec3 end, double time);
	glm::vec3 quaternionInterpolation(glm::vec3 start, glm::vec3 end, double time);

	void applyAnimation(animation *anim, int entityID, ArrayManager<transform>* tManager);
	void onUpdate();
};