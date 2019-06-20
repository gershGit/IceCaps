/*
	Class to control vulkan command pools
*/

#pragma once
#include "Vulkan/V_Device.h"
#include "Vulkan/V_Swapchain.h"
#include "Vulkan/V_GraphicsPipeline.h"
#include "Vulkan/V_DescriptorPool.h"

//Possible types the pool could be used for
enum pool_type {GRAPHICS, COMPUTE, TRANSFER, IMAGE_OPERATIONS};

class V_CommandPool
{
private:
	//This pools use type
	pool_type poolType;

	//References to other vulkan classes
	V_Device* device;	
	VkSurfaceKHR surface;	
	V_Swapchain* swapchain;

	//Command pool and command buffers as a part of those pools
	VkCommandPool commandPool;
public:
	//A vector containing all the command buffers of this pool
	std::vector<VkCommandBuffer> commandBuffers;
	int bufferIterator = 0;
	bool saturated = false;

	V_CommandPool();

	//Creates a command pool based on the device and surface
	void createCommandPool(pool_type poolType, V_Device* device, VkSurfaceKHR surface);
	void allocateCommandBuffers(int bufferCount);

	//Getter functions
	VkCommandPool getCommandPool() { return commandPool; };
	std::vector<VkCommandBuffer>* getCommandBuffers() { return &commandBuffers; };
	VkCommandBuffer getCommandBufferAt(int index) { return commandBuffers.at(index); };
	VkCommandBuffer getNextCommandBuffer();
	pool_type getPoolType() { return poolType; };
	V_Device* getDevice() { return device; };

	void cleanup();
	~V_CommandPool();
};

