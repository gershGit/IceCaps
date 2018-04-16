#pragma once
#include "../Headers/Headers.h"
#include "../Headers/VulkanLayerAndExtension.h"

class VulkanInstance
{
public:
	//Vulkan instance object variable
	VkInstance instance;

	//Vulkan instance specific layer and extensions
	VulkanLayerAndExtension layerExtension;

	//Create a new instance
	VkResult createInstance(vector<const char *>& layers, vector<const char *>& extensionNames, const char* applicationName);

	//Destroy the instance
	void destroyInstance();

	VulkanInstance();
	~VulkanInstance();
};
