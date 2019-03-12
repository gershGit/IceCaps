#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntityComponents.h"

class AnimationFactory {
public:
	static void loadFromFile(std::string fileIn, animation &anim, configurationStructure &config);
};