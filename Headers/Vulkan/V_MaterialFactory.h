/*
	Class to build a vulkan material components
*/

#pragma once
#include "V_Components.h"

class V_MaterialFactory
{
public:
	//Load from file functions
	static void loadTextureFromFile(std::string textureFile, v_material & material, texture_type textureType, configurationStructure & config);
	static void loadMaterialFromFile(std::string fileName, v_material &material, configurationStructure &config);

	V_MaterialFactory();
	~V_MaterialFactory();
};

