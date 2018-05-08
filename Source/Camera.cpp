#define _USE_MATH_DEFINES

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>
#include "GameObject.cpp"

GLCamera::GLCamera()
{
}


GLCamera::~GLCamera()
{
}

float GLCamera::getAspectRatio()
{
	return x_dim / y_dim;
}

glm::mat4 GLCamera::getViewMatrix()
{
	glm::mat4 viewMatrix = glm::inverse(gameObject->getTransform());

	return viewMatrix;
}

glm::mat4 GLCamera::getProjectionMatrix()
{
	double theta = fov * M_PI / 180.0;
	glm::mat4 projection;
	projection[0][0] = 1 / (aspect_ratio*tan(theta));
	projection[1][1] = 1 / (tan(theta));
	projection[2][2] = -(f + n) / (f - n);
	projection[3][2] = (-2 * f*n) / (f - n);
	projection[2][3] = 1;
	projection[3][3] = 0;

	return projection;
}
