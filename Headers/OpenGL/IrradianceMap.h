/*
	Class to store an equirectangular irradiance or environment map
*/

#pragma once
#include "Headers.h"
#include "Property.h"

//TODO store all cubemaps for a single probe in one object
class Imap : Property {
public:
	//Texture information for the map
	GLuint envMap;
	int mapTexNumber;

	//Constructor for an irradiance/environment map
	Imap(const char * fileName, int textureNumber);

	//Optional values for solid color environment maps or adjusting the strength of the map
	glm::vec3 color;
	float strength = 1.0f;
};