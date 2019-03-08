/*
	Class for loading scenes, primarily from .icap files
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include "Core/ComponentManager.h"
#include "Core/ManagersFactories.h"
#include "Core/SystemsHeader.h"

class SceneLoader
{
public:
	SceneLoader();

	//Loads file info into component managers
	static void loadScene(int sceneIndex, configurationStructure &config, std::vector<ComponentManager*> &managers, std::vector<EntitySystem*> &systems);
	//Unloads the scene from the managers and calls associated cleanup functions
	static void unloadScene(configurationStructure &config, std::vector<ComponentManager*> &componentManagers);

	~SceneLoader();
};

