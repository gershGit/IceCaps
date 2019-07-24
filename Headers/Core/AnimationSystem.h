/*
	System to run animations and update entities accordingly
*/

#pragma once
#include "Core/EntitySystem.h"
#include "Core/ManagersFactories.h"

class AnimationSystem : public EntitySystem {
private:
	ArrayManager<transform>* tManager = nullptr;
	MappedManager<animation>* aManager = nullptr;
	MappedManager<armature>* armManager = nullptr;
	MappedManager<armature_animation>* armAnManager = nullptr;
public:
	AnimationSystem() { operatesOn = { TRANSFORM, ANIMATION_COMPONENT, ARMATURE_COMPONENT }; entityListRequiredComponents = { {TRANSFORM, ANIMATION_COMPONENT}, {TRANSFORM, ANIMATION_COMPONENT, ARMATURE_COMPONENT} }; systemType = ANIMATION_SYSTEM; };
	~AnimationSystem() {};
	void initialize();
	void start();

	key_frame* getFramePrior(animation *anim, double time);
	armature_key* getFramePrior(armature_animation* anim, double time);
	key_frame* getFramePost(animation *anim, double time);
	armature_key* getFramePost(armature_animation* anim, double time);
	glm::vec3 linearInterpolation(glm::vec3 start, glm::vec3 end, double time);
	glm::vec3 quaternionInterpolation(glm::vec3 start, glm::vec3 end, double time);

	key_frame getLeftOver(double t, animation* anim, key_frame* leftOverStart, key_frame* thisFrameStart);
	armature_key getLeftOver(double t, armature_animation* anim, armature_key* leftOverStart, armature_key* thisFrameStart);

	void applyAnimation(animation *anim, int entityID, ArrayManager<transform>* tManager);
	void applyArmatureAnimation(armature_animation* anim, armature* arm, int entityID);
	void onUpdate();
};