#pragma once
#include "Headers.h"
#include "VulkanLayerAndExtension.h"
#include  <vector>

using namespace std;

class VulkanDevice
{
public:
	VulkanDevice(VkPhysicalDevice* gpu);
	~VulkanDevice();

	//Device related member variables
	VkDevice	device;
	VkPhysicalDevice* gpu;
	VkPhysicalDeviceProperties gpuProps;
	VkPhysicalDeviceMemoryProperties memoryProperties;

	//Queue related properties

	//Vulkan Queses object
	VkQueue queue;

	//Store all queue families exposed by the physical device
	vector<VkQueueFamilyProperties> queueFamilyProps;

	//Stores graphics queue index
	uint32_t graphicsQueueIndex;

	//Graphics queue index capable of presentation
	uint32_t graphicsQueueWithPresentIndex;

	//Number of queue family exposed by device
	uint32_t queueFamilyCount;

	//Device specific extensions
	VulkanLayerAndExtension layerExtension;

public:
	//This class exposes the below function to the outside world
	VkResult createDevice(std::vector<const char *>& layers, std::vector<const char *>& extensions);
	void getDeviceQueue();
	void destroyDevice();

	bool memoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex);

	// Get the avaialbe queues exposed by the physical devices
	void getPhysicalDeviceQueuesAndProperties();

	// Query physical device to retrive queue properties
	uint32_t getGraphicsQueueHandle();
};