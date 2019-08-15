#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntityComponents.h"

class AnimationFactory {
public:
	static void loadFromFile(char * fileIn, animation &anim, configurationStructure &config);
	static void loadArmatureAnimationFromFile(char* fileIn, armature_animation& anim, configurationStructure& config);
};