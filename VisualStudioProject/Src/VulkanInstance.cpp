#pragma once
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
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Define the vulkan instance create infor structure
	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext = NULL;
	instInfo.flags = 0;
	instInfo.enabledLayerCount = layers.size();
	instInfo.ppEnabledLayerNames = layers.data();
	instInfo.enabledExtensionCount = extensionNames.size();
	instInfo.ppEnabledExtensionNames = extensionNames.data();
	instInfo.pApplicationInfo = &appInfo;

	VkResult res = vkCreateInstance(&instInfo, NULL, &instance);
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
