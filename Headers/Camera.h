/*
 Camera class that can be used as a reference point from which scenes are rendered
 --> Inherits from the Property class to be used as a property for a game object
*/

#pragma once
#include "Headers.h"
#include "Property.h"

class GLCamera : public Property
{
public:
	//Constructor and destructor
	GLCamera();
	~GLCamera();

	//FOV controls zoom of camera
	float fov = 45;

	//Aspect ratio is used to maintain the shapes of objects when screen is stretched or squished
	float aspect_ratio = 1.333333f;

	//Near and far planes of the frustrum for culling
	float n = 0.1f;
	float f = 50.0f;

	//Dimensions that the camera renders to
	float x_dim = 1280;
	float y_dim = 720;

	//The positioning of the cursor. This can be used to control a camera
	float lastXpos = 640;
	float lastYpos = 360;

	//Returns the aspect ratio based on the dimensions of the camera
	float getAspectRatio();

	//Returns a matrix so that models can be moved to view space
	glm::mat4 getViewMatrix();

	//Returns a matrix so that objects in view space are warped by perspective
	glm::mat4 getProjectionMatrix();
};
