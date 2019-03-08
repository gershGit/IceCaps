/*
* This class contains callback functions for vulkan and OpenGL
* These callbacks are necessary throughout the engine so must be globally accessed
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include <optional>


//Structure that holds the indices for various queue families
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	int graphicsFamilyQueues = 0;
	std::optional<uint32_t> computeFamily;
	int computeFamilyQueues = 0;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	//True when a graphics and present family have been identified
	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
	bool isCompleteWithCompute() {
		return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
	}
};

//Details on what is supported by the available swapchain
struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


//Finds the queue families on a graphics card and returns the index of the family supporting graphics
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices findTransferFamily(VkPhysicalDevice device, VkSurfaceKHR surface);

//Finds the details of what is supported by the swapchain
SwapchainSupportDetails querySwapchainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);

//Message callback function
static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}

//Creates a messenger for debug statements
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

//Destroys the messenger for debug statements
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

//Finds the memeory type based on a device with memory flags
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

//Prints a list of queue families available on a physical device
void printQueueFamilies(std::vector<VkQueueFamilyProperties> queueFamiles, VkPhysicalDevice device, VkSurfaceKHR surface);

//Functions for finding and tranlating the sampling available on a physical device
int getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
VkSampleCountFlagBits getMaxUsableSampleCountFlags(VkPhysicalDevice physicalDevice);
VkSampleCountFlagBits intToSampleFlagBits(int bits);