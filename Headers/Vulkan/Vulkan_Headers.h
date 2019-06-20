/*
	Headers needed for any class using the vulkan api
*/

#pragma once

#include "Core/Main_Headers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>

struct VulkanSceneNode {
	material_type mType;
	std::vector<int> dynamicEntities;
	VkCommandBuffer* dynamicBuffers;

	int lightMax;
	int lightCountCurrent = 0;
	int* lightIDs = nullptr;
	VkBuffer* lightBuffers;
	VkDeviceMemory* lightBufferVRAM;
	VkDescriptorSet* camLightDescSets;

	bool hasCommandBuffer = false;
	VkCommandBuffer staticDrawCommands;
};