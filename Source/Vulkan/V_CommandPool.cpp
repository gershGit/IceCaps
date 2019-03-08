#include "Vulkan/V_CommandPool.h"
#include "Vulkan/V_Swapchain.h"
#include <array>

V_CommandPool::V_CommandPool()
{
}

//Creates a command pool of a given type
void V_CommandPool::createCommandPool(pool_type poolType_in, V_Device * device_in, VkSurfaceKHR surface_in)
{
	poolType = poolType_in;
	device = device_in;
	surface = surface_in;
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device->getPhysicalDevice(), surface);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	if (poolType == GRAPHICS || poolType == IMAGE_OPERATIONS) {
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	}
	else if (poolType == COMPUTE) {
		poolInfo.queueFamilyIndex = queueFamilyIndices.computeFamily.value();
	}
	else if (poolType == TRANSFER) {
		poolInfo.queueFamilyIndex = queueFamilyIndices.transferFamily.value();
	}
	else {
		//Default graphics as it should have the most capabilities
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	}
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult res = vkCreateCommandPool(device->getLogicalDevice(), &poolInfo, nullptr, &commandPool);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

//Allocates as many buffers as needed
void V_CommandPool::allocateCommandBuffers(int bufferCount)
{
	commandBuffers.resize(bufferCount);
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = commandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = (uint32_t) commandBuffers.size();

	if (poolType == GRAPHICS) {
		std::cout << "\t Allocating " << commandBuffers.size() << " command buffers for rendering" << std::endl;
	}
	else if (poolType == COMPUTE) {
		std::cout << "\t Allocating " << commandBuffers.size() << " command buffers for computing" << std::endl;
	}	
	VkResult res = vkAllocateCommandBuffers(device->getLogicalDevice(), &allocateInfo, commandBuffers.data());
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

//Cleans up the buffers and pool
void V_CommandPool::cleanup()
{
	vkFreeCommandBuffers(device->getLogicalDevice(), commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	vkDestroyCommandPool(device->getLogicalDevice(), commandPool, nullptr);
}

V_CommandPool::~V_CommandPool()
{
}
