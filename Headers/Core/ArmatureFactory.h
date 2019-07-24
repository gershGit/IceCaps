#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntityComponents.h"

class ArmatureFactory {
public:
	static void loadFromFile(char* fileIn, armature& anim, configurationStructure& config);
};