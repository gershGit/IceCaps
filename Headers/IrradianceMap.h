#pragma once
#include "Headers.h"
#include "Property.h"

class Imap : Property {
public:
	GLuint envMap;
	int mapTexNumber;
	Imap(const char * fileName, int textureNumber);

	glm::vec3 color;
	float strength;
};