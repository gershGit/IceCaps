#include "..\Headers\VulkanDevice.h"
#include "..\Headers\VulkanInstance.h"
#include "..\Headers\VulkanApplication.h"
#include <assert.h>


VulkanDevice::VulkanDevice(VkPhysicalDevice* physicalDevice)
{
	gpu = physicalDevice;
}


VulkanDevice::~VulkanDevice()
{
}

VkResult VulkanDevice::createDevice(std::vector<const char*>& layers, std::vector<const char *>& extensions) {
	layerExtension.appRequestedLayerNames = layers;
	layerExtension.appRequestedExtensionNames = extensions;

	// Create Device with available queue information.

	VkResult result;
	float queuePriorities[1] = { 0.0 };
	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.queueFamilyIndex = graphicsQueueIndex;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = NULL;
	queueInfo.queueCount = 1;
	queueInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;											// Device layers are deprecated
	deviceInfo.enabledExtensionCount = (uint32_t)extensions.size();
	deviceInfo.ppEnabledExtensionNames = extensions.size() ? extensions.data() : NULL;
	deviceInfo.pEnabledFeatures = NULL;

	result = vkCreateDevice(*gpu, &deviceInfo, NULL, &device);
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanDevice::getPhysicalDeviceQueuesAndProperties()
{
	// Query queue families count with pass NULL as second parameter.
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, NULL);

	// Allocate space to accomodate Queue properties.
	queueFamilyProps.resize(queueFamilyCount);

	// Get queue family properties
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, queueFamilyProps.data());
}

uint32_t VulkanDevice::getGraphicsQueueHandle()
{
	bool found = false;

	for (unsigned int i = 0; i < queueFamilyCount; i++) {
		//		Graphics Queue		- VK_QUEUE_GRAPHICS_BIT 
		//		Compute Queue		- VK_QUEUE_COMPUTE_BIT
		//		DMA/Transfer Queue	- VK_QUEUE_TRANSFER_BIT
		//		Sparse memory		- VK_QUEUE_SPARSE_BINDING_BIT

		if (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			found = true;
			graphicsQueueIndex = i;
			break;
		}
	}

	// Assert if there is no queue found.
	assert(found);

	return 0;
}

//Queue related functions
void VulkanDevice::getDeviceQueue()
{
	// Parminder: this depends on intialiing the SwapChain to 
	// get the graphics queue with presentation support
	vkGetDeviceQueue(device, graphicsQueueWithPresentIndex, 0, &queue);
}

void VulkanDevice::destroyDevice()
{
	vkDestroyDevice(device, NULL);
}

bool VulkanDevice::memoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t * typeIndex)
{
	// Search memtypes to find first index with those properties
	for (uint32_t i = 0; i < 32; i++) {
		if ((typeBits & 1) == 1) {
			// Type is available, does it match user properties?
			if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
				*typeIndex = i;
				return true;
			}
		}
		typeBits >>= 1;
	}
	// No memory types matched, return failure
	return false;
}
