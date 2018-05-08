#include "..\Headers\VulkanInstance.h"


VkResult VulkanInstance::createInstance(vector<const char*>& layers, vector<const char*>& extensionNames, const char * applicationName)
{
	//Set the instance specific layer and extension information
	layerExtension.appRequestedLayerNames = extensionNames;
	layerExtension.appRequestedExtensionNames = layers;

	//Define the vulkan application structure
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = applicationName;
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = applicationName;
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

	//Define the vulkan instance create infor structure
	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext = &layerExtension.dbgReportCreateInfo; //Enables debugging
	instInfo.flags = 0;
	instInfo.pApplicationInfo = &appInfo;

	//Specify the list of layer name to be enabled
	instInfo.enabledLayerCount = (uint32_t)layers.size();
	instInfo.ppEnabledLayerNames = layers.size() ? layers.data() : NULL;

	//Specify the list of extensions to be used in the application
	instInfo.enabledExtensionCount = (uint32_t)extensionNames.size();
	instInfo.ppEnabledExtensionNames = extensionNames.size() ? extensionNames.data() : NULL;
	
	VkResult res = vkCreateInstance(&instInfo, NULL, &instance);
	assert(res == VK_SUCCESS);

	return res;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(instance, NULL);
}

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
}