/*
	Static functions for vulkan buffer operations
*/

#pragma once

#include "Vulkan_Headers.h"
#include "Global_Callbacks.h"
#include "V_Device.h"

class V_CommandPool;

class V_BufferHelper
{
public:
	//Creates a vulkan buffer, staging buffer, image, or image view
	static void createBuffer(V_Device *device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void createStagingBuffer(V_Device* device, VkDeviceSize size, VkBuffer& buffer, VkDeviceMemory &bufferMemory);
	static void createImage(V_Device* device, uint32_t width, uint32_t height, uint32_t miplevels, VkSampleCountFlagBits numSamples,
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	static VkImageView createImageView(V_Device * device, VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectFlags, bool swizzle);

	//Copies data from one buffer to another, or from cpu to gpu buffer, or from a buffer into an image
	static void copyBuffer(V_CommandPool * commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	static void copyToBuffer();
	static void copyBufferToImage(V_CommandPool * commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	//Transitions the layout of an image from one layout to another
	static void transitionImageLayout(V_CommandPool * commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	
	//Begins and ends single use command buffers
	static VkCommandBuffer beginSingleTimeCommands(V_CommandPool * commandPool);
	static void endSingleTimeCommands(VkCommandBuffer commandBuffer, V_CommandPool * commandPool);

	V_BufferHelper();
	~V_BufferHelper();
};

