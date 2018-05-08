#pragma once
#include "..\Headers\VulkanInstance.h"
#include "..\Headers\VulkanDevice.h"
#include "..\Headers\VulkanRenderer.h"
#include "..\Headers\VulkanLayerAndExtension.h"

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
	VulkanRenderer * rendererPointer;
	VulkanInstance instanceObj;
	VulkanDevice* deviceObj;

	void initialize();
	void prepare();
	void update();
	bool render();
	void deInitialize();

	VulkanApplication();
	~VulkanApplication();

public:
	static VulkanApplication* GetInstance();
};
