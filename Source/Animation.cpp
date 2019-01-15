#include "..\Headers\Animation.h"
#include "glm/gtx/quaternion.hpp"

Animation::Animation(GameTimer* timer)
{
}


void Animation::setInterpolationType(InterpolationType type)
{
	interpolationType = type;
}

void Animation::start(double currentTime)
{
	startTime = currentTime;
	playing = true;
}

void Animation::end()
{
	playing = false;
}

glm::mat4 Animation::getMatrix(double currentTime)
{
	//TODO make these into functions so that curved animations work too
	if (currentTime-startTime < lastFrame_end.targetTime) {
		double timePercentage = ((currentTime-startTime) - lastFrame_start.targetTime) / lastFrame_end.targetTime;
		glm::vec3 currentPos = linearInterpolation(lastFrame_start.position, lastFrame_end.position, timePercentage);
		glm::mat4 myTranslate = glm::mat4(1);
		myTranslate[3][0] = currentPos.x;
		myTranslate[3][1] = currentPos.y;
		myTranslate[3][2] = currentPos.z;
		glm::mat4 currentRotMat = quaternionInterpolation(lastFrame_start.rotation, lastFrame_end.rotation, timePercentage);
		glm::vec3 currentScale = linearInterpolation(lastFrame_start.scale, lastFrame_end.scale, timePercentage);
		glm::mat4 myScale = glm::mat4();
		myScale[0][0] = currentScale.x > 0 ? currentScale.x : 1;
		myScale[1][1] = currentScale.y > 0 ? currentScale.y : 1;
		myScale[2][2] = currentScale.z > 0 ? currentScale.z : 1;
		myScale[3][3] = 1;
		return myTranslate * currentRotMat * myScale;
	}
	else {
		lastFrame_start = getFramePrior(currentTime - startTime);
		lastFrame_end = getFramePost(lastFrame_start.targetTime);
		double timePercentage = ((currentTime - startTime) - lastFrame_start.targetTime) / lastFrame_end.targetTime;
		glm::vec3 currentPos = linearInterpolation(lastFrame_start.position, lastFrame_end.position, timePercentage);
		glm::mat4 myTranslate = glm::mat4(1);
		myTranslate[3][0] = currentPos.x;
		myTranslate[3][1] = currentPos.y;
		myTranslate[3][2] = currentPos.z;
		glm::mat4 currentRotMat = quaternionInterpolation(lastFrame_start.rotation, lastFrame_end.rotation, timePercentage);
		glm::vec3 currentScale = linearInterpolation(lastFrame_start.scale, lastFrame_end.scale, timePercentage);
		glm::mat4 myScale = glm::mat4();
		myScale[0][0] = currentScale.x > 0 ? currentScale.x : 1;
		myScale[1][1] = currentScale.y > 0 ? currentScale.y : 1;
		myScale[2][2] = currentScale.z > 0 ? currentScale.z : 1;
		myScale[3][3] = 1;
		return myTranslate * currentRotMat * myScale;
	}
	return glm::mat4(1);
}

void Animation::addKeyframe(KeyFrame newFrame)
{
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].targetTime > newFrame.targetTime) {
			if (i == 0) {
				frames.insert(frames.begin(), newFrame);
				return;
			}
			else {
				frames.insert(frames.begin() + i, newFrame);
				return;
			}
		}
	}
	frames.push_back(newFrame);
}

void Animation::deleteKeyframe(std::string keyName)
{
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].name == keyName) {
			frames.erase(frames.begin() + i);
			return;
		}
	}
	std::cout << "Cannot delete frame. No such frame exists!" << std::endl;
}

KeyFrame Animation::getFramePrior(double time)
{
	for (int i = 0; i<frames.size(); i++) {
		if (frames[i].targetTime > time) {
			return frames[i - 1];
		}
	}
	return frames[frames.size() - 1];
}

KeyFrame Animation::getFramePost(double time)
{
	for (int i = 0; i<frames.size(); i++) {
		if (frames[i].targetTime > time) {
			return frames[i];
		}
	}
	return frames[frames.size() - 1];
}

glm::vec3 Animation::linearInterpolation(glm::vec3 start, glm::vec3 end, float t)
{
	return start + end * t;
}

glm::mat4 Animation::quaternionInterpolation(glm::vec3 start, glm::vec3 end, float t)
{
	glm::quat startQuat = glm::quat(start);
	glm::quat endQuat = glm::quat(end);
	//TODO modify this for lerp/slerp in accordance to curvature of animation
	glm::quat intermediateQuat = glm::slerp(startQuat, endQuat, t);
	return glm::toMat4(intermediateQuat);
}

Animation::~Animation()
{
}
