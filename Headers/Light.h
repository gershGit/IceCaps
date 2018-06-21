/*
	Class to store information of a single light
*/

#pragma once
#include "Headers.h"
#include "Property.h"

//TODO support more types of lights
class Light : Property {
public:
	//Stores the color and strength of the light
	glm::vec3 color;
	float strength;
};