/*
	Wrapper class that holds a device for vulkan to interface with
*/

#pragma once
#include "Vulkan_Headers.h"
#include "Global_Callbacks.h"

class V_Device
{
private:
	//Structure holding all information relating to the class
	configurationStructure* config;

	//Vulkan objects
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = VK_NULL_HANDLE;

	//Queues to submit into
	std::vector<VkQueue> graphicsQueues;
	VkQueue presentQueue;
	VkQueue transferQueue;
	VkQueue imageOperationsQueue;
	std::vector <VkQueue> computeQueues;

	//Surface for the device to connect to
	VkSurfaceKHR surface;

	//Whether or not validation is turned on
	bool enableValidationLayers;

	//Validation layer names
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	//Extensions for the device
	const std::vector<const char*> extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

public:
	//Number of cores the device is supposed to handle
	unsigned int coreCount;

	V_Device(); 	//Constructor

	//Creates the logical device
	void createLogicalDevice();

	//Initialization functions
	void initialize(VkPhysicalDevice physicalDeviceIn, VkSurfaceKHR surfaceIn, configurationStructure* config);

	//Setters
	void setCoreCount(unsigned int coreCount_in) { coreCount = coreCount_in; };

	//Getters
	VkPhysicalDevice getPhysicalDevice();
	VkDevice getLogicalDevice();
	VkQueue getGraphicsQueue();
	VkQueue getGraphicsQueue(int coreIndex);
	VkQueue getImageOperationsQueue();
	VkQueue getPresentQueue();
	VkQueue getTransferQueue();
	VkQueue getComputeQueueWithIndex(int index);
	VkQueue getGraphicsQueueWithIndex(int index);

	//Setters
	void setPhysicalDevice(VkPhysicalDevice physicalDeviceIn);
	void setSurface(VkSurfaceKHR surfaceIn);

	//Destruction functions
	void cleanup();
	~V_Device();
};

