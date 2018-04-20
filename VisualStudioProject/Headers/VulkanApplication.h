#pragma once
#include "Headers.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"

class VulkanApplication
{
private:
	// Variable for Singleton implementation
	static std::unique_ptr<VulkanApplication> instance;
	static std::once_flag onlyOnce;

	// Create the Vulkan instance object
	VkResult createVulkanInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions, const char* applicationName);
	VkResult handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions);
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList);

	bool debugFlag;

public:
	static VulkanApplication* GetInstance();
	VulkanInstance instanceObj;
	VulkanDevice* deviceObj;
	VulkanRenderer* rendererObj;

	void initialize();
	void prepare();
	void update();
	bool render();
	void deInitialize();

	VulkanApplication();
	~VulkanApplication();
};
