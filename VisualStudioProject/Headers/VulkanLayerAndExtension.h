#pragma once
#include "Headers.h"

using namespace std;

struct LayerProperties {
	VkLayerProperties properties;
	vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension
{
	
public:

	// List of layer and extension names requested by the application
	vector<const char *> appRequestedLayerNames;
	vector<const char *> appRequestedExtensionNames;

	// Layers and corresponding extions lists
	vector<LayerProperties> layerPropertyList;

	//Global extensions
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu = NULL);

	//Device based extensions
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

	//Instance / global layer
	VkResult getInstanceLayerProperties();

	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();
};
