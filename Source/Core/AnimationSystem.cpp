#include "Core/AnimationSystem.h"
#include "Core/GameTimer.h"
#include "glm/gtx/matrix_decompose.hpp"

void AnimationSystem::start()
{
	for (int id : *entities) {
		animation * anim = getCManager<animation>(*managers, ANIMATION_COMPONENT)->getComponentAddress(id);
		if (anim->playOnStartup) {
			anim->startTime = GameTimer::getCurrentTime();
			anim->lastFrameEnd = &(anim->frames[0]);
			anim->lastFrameStart = &(anim->frames[0]);
			anim->lastCalulatedFrame = anim->frames[0];
			anim->state = ANIMATION_PLAYING;
		}
	}
}

//Returns the closest frame before the current time
key_frame* AnimationSystem::getFramePrior(animation *anim, double time)
{
	for (unsigned int i = 0; i < anim->frameCount; i++) {
		if (anim->frames[i].t > time) {
			return &anim->frames[i - 1];
		}
	}
	return &anim->frames[anim->frameCount - 1];
}

//Returns the closest frame after the current time
key_frame* AnimationSystem::getFramePost(animation *anim, double time)
{
	for (unsigned int i = 0; i < anim->frameCount; i++) {
		if (anim->frames[i].t > time) {
			return &anim->frames[i];
		}
	}
	return &anim->frames[anim->frameCount - 1];
}

//Interpolates a vector linearly
glm::vec3 AnimationSystem::linearInterpolation(glm::vec3 start, glm::vec3 end, double time)
{
	return start + (end * (float) time);
}

//Interpolates a vector by converting a quaternion and slerping
glm::vec3 AnimationSystem::quaternionInterpolation(glm::vec3 start, glm::vec3 end, double time)
{
	glm::quat startQuat = glm::quat(start);
	glm::quat endQuat = glm::quat(end);
	glm::quat intermediateQuat = glm::slerp(startQuat, endQuat, (float) time);
	return glm::eulerAngles(intermediateQuat);
}

//Calculates the left over deltas
key_frame getLeftOver(double t, animation *anim, key_frame* leftOverStart, key_frame* thisFrameStart) {
	key_frame leftOverFrame;
	leftOverFrame.deltaPosition = leftOverStart->deltaPosition - anim->lastCalulatedFrame.deltaPosition;
	leftOverFrame.deltaRotation = leftOverStart->deltaRotation - anim->lastCalulatedFrame.deltaRotation;
	leftOverFrame.deltaScale = leftOverStart->deltaScale - anim->lastCalulatedFrame.deltaScale;
	unsigned int nextIndex = leftOverStart->index + 1;
	while (nextIndex <= thisFrameStart->index) {
		leftOverFrame.deltaPosition += anim->frames[nextIndex].deltaPosition;
		leftOverFrame.deltaRotation += anim->frames[nextIndex].deltaRotation;
		leftOverFrame.deltaScale += anim->frames[nextIndex].deltaScale;
		nextIndex++;
	}
	return leftOverFrame;
}

//Applies an animation to an entity
void AnimationSystem::applyAnimation(animation *anim, int entityID, ArrayManager<transform>* tManager)
{
	if (anim->state == ANIMATION_PLAYING) {
		double currentTime = GameTimer::getCurrentTime();
		if (!anim->repeat && currentTime - anim->startTime > anim->length) {
			anim->state = ANIMATION_OVER;
			return;
		}

		transform * t = tManager->getComponentAddress(entities->at(entityID));
		glm::vec3 newDeltaPos;
		glm::vec3 newDeltaRot;
		glm::vec3 newDeltaScale;
		if (currentTime - anim->startTime < anim->lastFrameEnd->t) {
			//Still between the same 2 keyframes
			double timePercentage = (currentTime - (anim->startTime + anim->lastFrameStart->t)) / (anim->lastFrameEnd->t - anim->lastFrameStart->t);
			newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaPosition, timePercentage) * anim->animationWeight;
			newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaRotation, timePercentage) * anim->animationWeight;
			newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaScale, timePercentage) * anim->animationWeight;

			t->pos += (newDeltaPos - anim->lastCalulatedFrame.deltaPosition);
			t->rot += (newDeltaRot - anim->lastCalulatedFrame.deltaRotation);
			t->scale += (newDeltaScale - anim->lastCalulatedFrame.deltaScale);
		}
		else {
			//Skipped frame or moved to next frame
			key_frame* leftOverStartFrame = anim->lastFrameEnd;
			anim->lastFrameStart = getFramePrior(anim, currentTime - anim->startTime);
			anim->lastFrameEnd = getFramePost(anim, anim->lastFrameStart->t);

			double timePercentage = (currentTime - (anim->startTime + anim->lastFrameStart->t)) / (anim->lastFrameEnd->t - anim->lastFrameStart->t);
			newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaPosition, timePercentage) * anim->animationWeight;
			newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaRotation, timePercentage) * anim->animationWeight;
			newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->deltaScale, timePercentage) * anim->animationWeight;

			key_frame leftOverFrame = getLeftOver(currentTime - anim->startTime, anim, leftOverStartFrame, anim->lastFrameStart);

			t->pos += newDeltaPos + leftOverFrame.deltaPosition;
			t->rot += newDeltaRot + leftOverFrame.deltaRotation;
			t->scale += newDeltaScale + leftOverFrame.deltaScale;
		}

		//Update the last delta
		anim->lastCalulatedFrame.deltaPosition = newDeltaPos;
		anim->lastCalulatedFrame.deltaRotation = newDeltaRot;
		anim->lastCalulatedFrame.deltaScale = newDeltaScale;
	}
}

//Applies animations to all entities
void AnimationSystem::onUpdate()
{
	ArrayManager<transform>* tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	MappedManager<animation>* aManager = dynamic_cast<MappedManager<animation>*>( getCManager<animation>(*managers, ANIMATION_COMPONENT));
	for (int i = 0; i < entities->size(); i++) {
		applyAnimation( aManager->getComponentAddress(entities->at(i)), entities->at(i), tManager );
	}
}