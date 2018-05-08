#pragma once
#include "Headers.h"
#include "Property.h"

class GLCamera : public Property
{
public:
	GLCamera();
	~GLCamera();

	float fov = 45;
	float aspect_ratio = 1.333333f;
	float n = 0;
	float f = 100;
	float x_dim = 800;
	float y_dim = 600;

	float getAspectRatio();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};
