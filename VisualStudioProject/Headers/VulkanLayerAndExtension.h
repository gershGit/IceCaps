#pragma once
#include "../Headers/Headers.h"

using namespace std;

struct LayerProperties {
	VkLayerProperties properties;
	vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension
{
	
private:
	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	VkDebugReportCallbackEXT debugReportCallback;

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

	/******* VULKAN DEBUGGING MEMBER FUNCTION AND VARAIBLES *******/
	VkBool32 areLayersSupported(vector<const char *>& layerNames);
	VkResult createDebugReportCallback();
	VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
	void	destroyDebugReportCallback();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction(VkFlags msgFlags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char *layerPrefix,
		const char *msg,
		void *userData);


	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();
};
