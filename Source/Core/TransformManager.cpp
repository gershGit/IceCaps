#include "Core/TransformManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <chrono>

void TransformManager::setSize(int size_in)
{
	size = size_in;
	transformArray = (transform*) malloc(sizeof(transform) * size);
}

//Creates a scale, rotation, and translation matrix from the given entity index and returns the transformation
glm::mat4 TransformManager::getTransformMatrix(int entityID)
{
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), transformArray[entityID].scale);
	glm::mat4 rotMat = glm::mat4_cast(glm::quat(transformArray[entityID].rot));
	glm::mat4 transMat = glm::translate(glm::mat4(1), transformArray[entityID].pos);

	return transMat * rotMat * scaleMat;
}

TransformManager::~TransformManager()
{
}
