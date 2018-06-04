#pragma once
#include "Headers.h"
#include "Property.h"


class Light : Property {
public:
	glm::vec3 color;
	float strength;
};