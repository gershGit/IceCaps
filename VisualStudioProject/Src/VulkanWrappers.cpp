#define _CRT_SECURE_NO_WARNINGS
#include "..\Headers\VulkanWrappers.h"

void CommandBufferManager::allocCommandBuffer(const VkDevice * device, const VkCommandPool commandPool, VkCommandBuffer * commandBuffer, const VkCommandBufferAllocateInfo * commandBufferInfo)
{
	VkResult result;
	//Use command buffer allocate info if provided
	if (commandBufferInfo) {
		result = vkAllocateCommandBuffers(*device, commandBufferInfo, commandBuffer);
		assert(!result);
		return;
	}

	//Create an allocation info if not provided
	VkCommandBufferAllocateInfo defaultCommandBufferInfo = {};
	defaultCommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	defaultCommandBufferInfo.pNext = NULL;
	defaultCommandBufferInfo.commandPool = commandPool;
	defaultCommandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	defaultCommandBufferInfo.commandBufferCount = (uint32_t) sizeof(commandBuffer) / sizeof(VkCommandBuffer);

	//Default memory allocation
	result = vkAllocateCommandBuffers(*device, &defaultCommandBufferInfo, commandBuffer);
	assert(!result);
}

void CommandBufferManager::beginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo * commandBufferBeginInfo)
{
	VkResult result;
	//If a command buffer begin info structure is provided, use it
	if (commandBufferBeginInfo) {
		result = vkBeginCommandBuffer(commandBuffer, commandBufferBeginInfo);
		assert(result == VK_SUCCESS);
		return;
	}

	//Create a default implementation for command buffer begin info
	VkCommandBufferInheritanceInfo defaultInheritanceInfo = {};
	defaultInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	defaultInheritanceInfo.pNext = NULL;
	defaultInheritanceInfo.renderPass = VK_NULL_HANDLE;
	defaultInheritanceInfo.subpass = 0;
	defaultInheritanceInfo.framebuffer = VK_NULL_HANDLE;
	defaultInheritanceInfo.occlusionQueryEnable = VK_FALSE;
	defaultInheritanceInfo.queryFlags = 0;
	defaultInheritanceInfo.pipelineStatistics = 0;

	VkCommandBufferBeginInfo defaultBeginInfo = {};
	defaultBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	defaultBeginInfo.pNext = NULL;
	defaultBeginInfo.flags = 0;
	defaultBeginInfo.pInheritanceInfo = &defaultInheritanceInfo;

	result = vkBeginCommandBuffer(commandBuffer, &defaultBeginInfo);
	assert(result == VK_SUCCESS);
}

void CommandBufferManager::endCommandBuffer(VkCommandBuffer commandBuffer)
{
	VkResult result;
	result = vkEndCommandBuffer(commandBuffer);
	assert(result == VK_SUCCESS);
}

void CommandBufferManager::submitCommandBuffer(const VkQueue & queue, const VkCommandBuffer * commandBufferList, const VkSubmitInfo * submitInfo, const VkFence & fence)
{
	VkResult result;

	//Use submit information if provided
	if (submitInfo) {
		vkQueueSubmit(queue, 1, submitInfo, fence);
		result = vkQueueWaitIdle(queue);
		return;
	}

	//If no submit info is provided build a default one
	VkSubmitInfo defaultSubmitInfo = {};
	defaultSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	defaultSubmitInfo.pNext = NULL;
	defaultSubmitInfo.waitSemaphoreCount = 0;
	defaultSubmitInfo.pWaitSemaphores = NULL;
	defaultSubmitInfo.pWaitDstStageMask = NULL;
	defaultSubmitInfo.commandBufferCount = (uint32_t)sizeof(commandBufferList) / sizeof(VkCommandBuffer);
	defaultSubmitInfo.pCommandBuffers = commandBufferList;
	defaultSubmitInfo.signalSemaphoreCount = 0;
	defaultSubmitInfo.pSignalSemaphores = NULL;

	result = vkQueueSubmit(queue, 1, &defaultSubmitInfo, fence);
	assert(!result);

	result = vkQueueWaitIdle(queue);
	assert(!result);
}

void* readFile(const char *spvFileName, size_t *fileSize) {

	FILE *fp = fopen(spvFileName, "rb");
	if (!fp) {
		return NULL;
	}

	size_t retval;
	long int size;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	void* spvShader = malloc(size + 1); // Plus for NULL character '\0'
	memset(spvShader, 0, size + 1);

	retval = fread(spvShader, size, 1, fp);
	assert(retval == 1);

	*fileSize = size;
	fclose(fp);
	return spvShader;
}