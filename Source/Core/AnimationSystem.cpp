#include "Core/AnimationSystem.h"
#include "Core/GameTimer.h"
#include "glm/gtx/matrix_decompose.hpp"

void AnimationSystem::initialize()
{
	tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	aManager = dynamic_cast<MappedManager<animation>*>(getCManager<animation>(*managers, ANIMATION_COMPONENT));
	armManager = dynamic_cast<MappedManager<armature>*>(getCManager<armature>(*managers, ARMATURE_COMPONENT));
	armAnManager = dynamic_cast<MappedManager<armature_animation>*>(getCManager<armature_animation>(*managers, ARMATURE_ANIMATION));
}

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
armature_key* AnimationSystem::getFramePrior(armature_animation* anim, double time) {
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
armature_key* AnimationSystem::getFramePost(armature_animation* anim, double time)
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
key_frame AnimationSystem::getLeftOver(double t, animation *anim, key_frame* leftOverStart, key_frame* thisFrameStart) {
	key_frame leftOverFrame;
	leftOverFrame.snap.deltaPosition = leftOverStart->snap.deltaPosition - anim->lastCalulatedFrame.snap.deltaPosition;
	leftOverFrame.snap.deltaRotation = leftOverStart->snap.deltaRotation - anim->lastCalulatedFrame.snap.deltaRotation;
	leftOverFrame.snap.deltaScale = leftOverStart->snap.deltaScale - anim->lastCalulatedFrame.snap.deltaScale;
	unsigned int nextIndex = leftOverStart->index + 1;
	while (nextIndex <= thisFrameStart->index) {
		leftOverFrame.snap.deltaPosition += anim->frames[nextIndex].snap.deltaPosition;
		leftOverFrame.snap.deltaRotation += anim->frames[nextIndex].snap.deltaRotation;
		leftOverFrame.snap.deltaScale += anim->frames[nextIndex].snap.deltaScale;
		nextIndex++;
	}
	return leftOverFrame;
}
armature_key AnimationSystem::getLeftOver(double t, armature_animation* anim, armature_key* leftOverStart, armature_key* thisFrameStart) {
	armature_key leftOverFrame;
	for (int i = 0; i < anim->boneIDs.size(); i++) {
		leftOverFrame.snaps[i].deltaPosition = leftOverStart->snaps[i].deltaPosition - anim->lastCalulatedFrame.snaps[i].deltaPosition;
		leftOverFrame.snaps[i].deltaRotation = leftOverStart->snaps[i].deltaRotation - anim->lastCalulatedFrame.snaps[i].deltaRotation;
		leftOverFrame.snaps[i].deltaScale = leftOverStart->snaps[i].deltaScale - anim->lastCalulatedFrame.snaps[i].deltaScale;
		unsigned int nextIndex = leftOverStart->index + 1;
		while (nextIndex <= thisFrameStart->index) {
			leftOverFrame.snaps[i].deltaPosition += anim->frames[nextIndex].snaps[i].deltaPosition;
			leftOverFrame.snaps[i].deltaRotation += anim->frames[nextIndex].snaps[i].deltaRotation;
			leftOverFrame.snaps[i].deltaScale += anim->frames[nextIndex].snaps[i].deltaScale;
			nextIndex++;
		}
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
			newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaPosition, timePercentage) * anim->animationWeight;
			newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaRotation, timePercentage) * anim->animationWeight;
			newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaScale, timePercentage) * anim->animationWeight;

			t->pos += (newDeltaPos - anim->lastCalulatedFrame.snap.deltaPosition);
			t->rot += (newDeltaRot - anim->lastCalulatedFrame.snap.deltaRotation);
			t->scale += (newDeltaScale - anim->lastCalulatedFrame.snap.deltaScale);
		}
		else {
			//Skipped frame or moved to next frame
			key_frame* leftOverStartFrame = anim->lastFrameEnd;
			anim->lastFrameStart = getFramePrior(anim, currentTime - anim->startTime);
			anim->lastFrameEnd = getFramePost(anim, anim->lastFrameStart->t);

			double timePercentage = (currentTime - (anim->startTime + anim->lastFrameStart->t)) / (anim->lastFrameEnd->t - anim->lastFrameStart->t);
			newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaPosition, timePercentage) * anim->animationWeight;
			newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaRotation, timePercentage) * anim->animationWeight;
			newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snap.deltaScale, timePercentage) * anim->animationWeight;

			key_frame leftOverFrame = getLeftOver(currentTime - anim->startTime, anim, leftOverStartFrame, anim->lastFrameStart);

			t->pos += newDeltaPos + leftOverFrame.snap.deltaPosition;
			t->rot += newDeltaRot + leftOverFrame.snap.deltaRotation;
			t->scale += newDeltaScale + leftOverFrame.snap.deltaScale;
		}

		//Update the last delta
		anim->lastCalulatedFrame.snap.deltaPosition = newDeltaPos;
		anim->lastCalulatedFrame.snap.deltaRotation = newDeltaRot;
		anim->lastCalulatedFrame.snap.deltaScale = newDeltaScale;
	}
}

//Applies an animation onto an entity that uses an armature
void AnimationSystem::applyArmatureAnimation(armature_animation* anim, armature* arm, int entityID) {
	if (anim->state == ANIMATION_PLAYING) {
		double currentTime = GameTimer::getCurrentTime();
		if (!anim->repeat && currentTime - anim->startTime > anim->length) {
			anim->state = ANIMATION_OVER;
			return;
		}

		for (int i = 0; i < anim->boneIDs.size(); i++) {
			glm::vec3 newDeltaPos;
			glm::vec3 newDeltaRot;
			glm::vec3 newDeltaScale;

			if (currentTime - anim->startTime < anim->lastFrameEnd->t) {
				//Still between the same 2 keyframes
				double timePercentage = (currentTime - (anim->startTime + anim->lastFrameStart->t)) / (anim->lastFrameEnd->t - anim->lastFrameStart->t);
				newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaPosition, timePercentage) * anim->animationWeight;
				newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaRotation, timePercentage) * anim->animationWeight;
				newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaScale, timePercentage) * anim->animationWeight;

				bone* thisBone = &arm->bones.at(anim->boneIDs.at(i));
				thisBone->poseTransform.pos += (newDeltaPos - anim->lastCalulatedFrame.snaps[i].deltaPosition);
				thisBone->poseTransform.rot += (newDeltaRot - anim->lastCalulatedFrame.snaps[i].deltaRotation);
				thisBone->poseTransform.scale += (newDeltaScale - anim->lastCalulatedFrame.snaps[i].deltaScale);
			}
			else {
				//Skipped frame or moved to next frame
				armature_key* leftOverStartFrame = anim->lastFrameEnd;
				anim->lastFrameStart = getFramePrior(anim, currentTime - anim->startTime);
				anim->lastFrameEnd = getFramePost(anim, anim->lastFrameStart->t);

				double timePercentage = (currentTime - (anim->startTime + anim->lastFrameStart->t)) / (anim->lastFrameEnd->t - anim->lastFrameStart->t);
				newDeltaPos = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaPosition, timePercentage) * anim->animationWeight;
				newDeltaRot = quaternionInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaRotation, timePercentage) * anim->animationWeight;
				newDeltaScale = linearInterpolation(glm::vec3(0), anim->lastFrameEnd->snaps[i].deltaScale, timePercentage) * anim->animationWeight;

				armature_key leftOverFrame = getLeftOver(currentTime - anim->startTime, anim, leftOverStartFrame, anim->lastFrameStart);

				bone* thisBone = &arm->bones.at(anim->boneIDs.at(i));
				thisBone->poseTransform.pos += newDeltaPos + leftOverFrame.snaps[i].deltaPosition;
				thisBone->poseTransform.rot += newDeltaRot + leftOverFrame.snaps[i].deltaRotation;
				thisBone->poseTransform.scale += newDeltaScale + leftOverFrame.snaps[i].deltaScale;
			}

			//Update the last delta
			anim->lastCalulatedFrame.snaps[i].deltaPosition = newDeltaPos;
			anim->lastCalulatedFrame.snaps[i].deltaRotation = newDeltaRot;
			anim->lastCalulatedFrame.snaps[i].deltaScale = newDeltaScale;
		}
	}
}

//Applies animations to all entities
void AnimationSystem::onUpdate()
{
	for (int i = 0; i < entities->size(); i++) {
		if (armManager->hasEntity(entities->at(i))) {
			applyArmatureAnimation(armAnManager->getComponentAddress(entities->at(i)), armManager->getComponentAddress(entities->at(i)), entities->at(i));
		}
		else {
			applyAnimation(aManager->getComponentAddress(entities->at(i)), entities->at(i), tManager);
		}
	}
}