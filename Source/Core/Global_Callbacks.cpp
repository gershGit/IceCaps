#pragma once
#include "Core/Global_Callbacks.h"
#include <algorithm>

//Prints the queue families available on a physical device
void printQueueFamilies(std::vector<VkQueueFamilyProperties> queueFamiles, VkPhysicalDevice device, VkSurfaceKHR surface) {
	std::cout << queueFamiles.size() << " queue families found on device" << std::endl;
	int i = 0;
	for (VkQueueFamilyProperties queueFamily : queueFamiles) {
		std::cout << "\tQueue Family: " << i << std::endl;
		std::cout << "\t\tContains " << queueFamily.queueCount << " Queues" << std::endl;
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			std::cout << "\t\tSUPPORTS GRAPHICS" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			std::cout << "\t\tSUPPORTS COMPUTE" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			std::cout << "\t\tSUPPORTS TRANSFER" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
			std::cout << "\t\tSUPPORTS SPARSE BINDING" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT) {
			std::cout << "\t\tSUPPORTS PROTECTED" << std::endl;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			std::cout << "\t\tSUPPORTS PRESENT" << std::endl;
		}
		i++;
	}
}

//Finds the queue families on a graphics card and returns the index of the family supporting graphics
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());
	
	//Attempt separate graphics and compute queue families
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			indices.graphicsFamilyQueues = queueFamily.queueCount;
		}
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			if (i != indices.graphicsFamily) {
				indices.computeFamily = i;
				indices.computeFamilyQueues = queueFamily.queueCount;
			}
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (!indices.presentFamily.has_value() && queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isCompleteWithCompute()) {
			break;
		}
		i++;
	}

	//Attempt to find separate transfer queue family
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (!indices.graphicsFamily.has_value() && !indices.computeFamily.has_value()) {
				indices.transferFamily = i;
			}
			else if (!indices.graphicsFamily.has_value() && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
			else if (!indices.computeFamily.has_value() && (i != indices.graphicsFamily)) {
				indices.transferFamily = i;
			}
			else if ((i != indices.graphicsFamily) && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
		}
		i++;
	}
	if (indices.transferFamily.has_value()) {
		return indices;
	}

	//Settle for a family being used for graphics or compute as well
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			indices.transferFamily = i;
		}
		i++;
	}

	if (!indices.computeFamily.has_value() && indices.graphicsFamily.has_value()) {
		if (queueFamiles[indices.graphicsFamily.value()].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			indices.computeFamily = indices.graphicsFamily.value();
			indices.computeFamilyQueues = indices.graphicsFamilyQueues;
		}
	}

	return indices;
}
QueueFamilyIndices findTransferFamily(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());

	//Attempt separate graphics and compute queue families
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			if (!indices.graphicsFamily.has_value() || (indices.graphicsFamily.has_value() && i != indices.graphicsFamily)) {
				indices.computeFamily = i;
			}
		}
		i++;
	}

	//Attempt to find separate transfer queue family
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (!indices.graphicsFamily.has_value() && !indices.computeFamily.has_value()) {
				indices.transferFamily = i;
			}
			else if (!indices.graphicsFamily.has_value() && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
			else if (!indices.computeFamily.has_value() && (i != indices.graphicsFamily)) {
				indices.transferFamily = i;
			}
			else if ((i != indices.graphicsFamily) && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
		}
		i++;
	}
	if (indices.transferFamily.has_value()) {
		return indices;
	}

	//Settle for a family being used for graphics or compute as well
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			indices.transferFamily = i;
		}
		i++;
	}

	return indices;
}

//Returns a structure of the details on what is supported for the swapchain
SwapchainSupportDetails querySwapchainSupport(VkSurfaceKHR surface, VkPhysicalDevice device) {
	SwapchainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

//Creates the messenfer for debugs
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

//Destroys the messenger for debugs
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

//Finds a memory type
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

//Functions to translate between sampling information
int getMaxUsableSampleCount(VkPhysicalDevice physicalDevice) {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return 64; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return 32; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return 16; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return 8; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return 4; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return 2; }

	return 1;
}
VkSampleCountFlagBits getMaxUsableSampleCountFlags(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
VkSampleCountFlagBits intToSampleFlagBits(int bits) {
	if (bits == 64) { return VK_SAMPLE_COUNT_64_BIT; }
	if (bits == 32) { return VK_SAMPLE_COUNT_32_BIT; }
	if (bits == 16) { return VK_SAMPLE_COUNT_16_BIT; }
	if (bits == 8) { return VK_SAMPLE_COUNT_8_BIT; }
	if (bits == 4) { return VK_SAMPLE_COUNT_4_BIT; }
	if (bits == 2) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}