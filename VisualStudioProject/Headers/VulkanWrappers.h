#pragma once
#include "Headers.h"

class CommandBufferManager {
public:
	//Allocate memory for command buffer from comand buffer pool
	static void allocCommandBuffer(const VkDevice* device, const VkCommandPool commandPool, VkCommandBuffer* commandBuffer, const VkCommandBufferAllocateInfo* commandBufferInfo);
	//Start command buffer recording
	static void beginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* commandBufferBeginInfo = NULL);
	//End the command buffer recording
	static void endCommandBuffer(VkCommandBuffer commandBuffer);
	//Submit the command buffer for execution
	static void submitCommandBuffer(const VkQueue &queue, const VkCommandBuffer* commandBufferList, const VkSubmitInfo* submitInfo = NULL, const VkFence &fence = VK_NULL_HANDLE);
};