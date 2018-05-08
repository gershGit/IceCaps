#include "..\Headers\VulkanSwapchain.h"
#include "..\Headers\VulkanDevice.h"
#include "..\Headers\VulkanInstance.h"
#include "..\Headers\VulkanRenderer.h"
#include "..\Headers\VulkanApplication.h"


VulkanSwapchain::VulkanSwapchain(VulkanRenderer* renderer)
{
	rendererObj = renderer;
	appObj = VulkanApplication::GetInstance();
}


VulkanSwapchain::~VulkanSwapchain()
{
	scPrivateVars.swapchainImages.clear();
	scPrivateVars.surfFormats.clear();
	scPrivateVars.presentModes.clear();
}

VkResult VulkanSwapchain::createSwapChainExtensions() {
	//Dependent on creating the presentation window
	VkInstance& instance = appObj->instanceObj.instance;
	VkDevice& device = appObj->deviceObj->device;

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);

	// Get Instance based swap chain extension function pointer
	fpGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	fpGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	fpGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	fpDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");

	// Get Device based swap chain extension function pointer
	fpCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR");
	fpDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR");
	fpGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR");

	return VK_SUCCESS;
}

VkResult VulkanSwapchain::createSurface() {
	//Dependent on creating the presentation window, needs an ampty window handle
	VkInstance& instance = appObj->instanceObj.instance;

	//Construct the surface description
	//GLFW version
	if (glfwVulkanSupported()) {
		std::cout << "Vulkan supported by GLFW" << std::endl;
	}
	VkResult result = glfwCreateWindowSurface(instance, rendererObj->window, NULL, &scPublicVars.surface);
	assert(result == VK_SUCCESS);
	return VK_SUCCESS;
}

uint32_t VulkanSwapchain::getGraphicsQueueWithPresentationSupport() {
	VulkanDevice* device = appObj->deviceObj;
	uint32_t queueCount = device->queueFamilyCount;
	VkPhysicalDevice gpu = *device->gpu;
	std::vector<VkQueueFamilyProperties>& queueProps = device->queueFamilyProps;

	//Iterate over each queue and get presentation status for each
	VkBool32* supportsPresent = (VkBool32 *)malloc(queueCount * sizeof(VkBool32));
	for (uint32_t i = 0; i < queueCount; i++) {
		fpGetPhysicalDeviceSurfaceSupportKHR(gpu, i, scPublicVars.surface, &supportsPresent[i]);
	}

	//Search for a graphics queue and a present queue in the array of queue families, try to find one that supports both
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < queueCount; i++) {
		if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
			if (graphicsQueueNodeIndex == UINT32_MAX) {
				graphicsQueueNodeIndex = i;
			}
			if (supportsPresent[i] == VK_TRUE) {
				graphicsQueueNodeIndex = i;
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	if (presentQueueNodeIndex == UINT32_MAX) {
		//No queue found that supports both graphics and present so we find a present only one
		for (uint32_t i = 0; i < queueCount; ++i) {
			if (supportsPresent[i] == VK_TRUE) {
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	free(supportsPresent);

	//Generate an error if could not find both a graphicsc and a present index
	if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX) {
		return UINT32_MAX;
	}

	return graphicsQueueNodeIndex;
}

void VulkanSwapchain::getSupportedFormats() {
	VkPhysicalDevice gpu = *rendererObj->getDevice()->gpu;
	VkResult result;

	//Get the list of VkFormats that are supported
	uint32_t formatCount;
	result = fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, NULL);
	assert(result == VK_SUCCESS);
	scPrivateVars.surfFormats.clear();
	scPrivateVars.surfFormats.resize(formatCount);

	//Get VkFormats in allocated objects
	result = fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, &scPrivateVars.surfFormats[0]);
	assert(result == VK_SUCCESS);

	//In case its a VK_FORMAT_UNDEFINED then surface has no preferred format. We use BRGA 32 bit format
	if (formatCount == 1 && scPrivateVars.surfFormats[0].format == VK_FORMAT_UNDEFINED) {
		scPublicVars.format = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else {
		assert(formatCount >= 1);
		scPublicVars.format = scPrivateVars.surfFormats[0].format;
	}
}

void VulkanSwapchain::initialize() {
	//Querying swapchain extensions
	createSwapChainExtensions();

	//Create surface and associate with windowing
	createSurface();

	//Getting a graphics queue with presentation support
	uint32_t index = getGraphicsQueueWithPresentationSupport();
	if (index == UINT32_MAX) {
		std::cout << "Could not find a graphics and a present queue" << std::endl;
		exit(-1);
	}
	rendererObj->getDevice()->graphicsQueueWithPresentIndex = index;

	//Get the list of formats that are supported
	getSupportedFormats();
}

void VulkanSwapchain::createSwapchain(const VkCommandBuffer& commandBuffer) {
	//Use extensions and get the surface capabilites, present mode
	getSurfaceCapabilitiesAndPresentMode();

	//Gather necessary information for present mode
	managePresentMode();

	//Create the swap chain images
	createSwapChainColorImages();

	//Get the create color image drawing surfaces
	createColorImageView(commandBuffer);
}

void VulkanSwapchain::getSurfaceCapabilitiesAndPresentMode() {
	VkResult result;
	VkPhysicalDevice gpu = *appObj->deviceObj->gpu;
	result = fpGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, scPublicVars.surface, &scPrivateVars.surfCapabilities);
	assert(result == VK_SUCCESS);

	result = fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, NULL);
	assert(result == VK_SUCCESS);

	scPrivateVars.presentModes.clear();
	scPrivateVars.presentModes.resize(scPrivateVars.presentModeCount);
	assert(scPrivateVars.presentModes.size() >= 1);

	result = fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, &scPrivateVars.presentModes[0]);
	assert(result == VK_SUCCESS);

	if (scPrivateVars.surfCapabilities.currentExtent.width == (uint32_t)-1) {
		//If the surface width and height is not defined, then set them equal to image size
		scPrivateVars.swapChainExtent.width = rendererObj->width;
		scPrivateVars.swapChainExtent.height = rendererObj->height;
	}
	else {
		//If the surface size is defined, then it must match the swapchain size
		scPrivateVars.swapChainExtent = scPrivateVars.surfCapabilities.currentExtent;
	}
}

//TODO overload with differnt presentation variables
void VulkanSwapchain::managePresentMode() {
	//If mailbox mode is available, use it as it is the lowest latency without tearing
	scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (size_t i = 0; i < scPrivateVars.presentModeCount; i++) {
		if (scPrivateVars.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		if ((scPrivateVars.swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (scPrivateVars.presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
			scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	//Determine the number of VkImages to use in the swapchain
	//We want 1 at a time besides the one being displayed and queued for display
	scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.minImageCount;
	if ((scPrivateVars.surfCapabilities.maxImageCount > 0) && (scPrivateVars.desiredNumberOfSwapChainImages > scPrivateVars.surfCapabilities.maxImageCount)) {
		//Application must settles for fewer images than desired
		scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.maxImageCount;
	}

	if (scPrivateVars.surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		scPrivateVars.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else {
		scPrivateVars.preTransform = scPrivateVars.surfCapabilities.currentTransform;
	}
}

void VulkanSwapchain::createSwapChainColorImages() {
	VkResult result;

	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.pNext = NULL;
	swapchainInfo.surface = scPublicVars.surface;
	swapchainInfo.minImageCount = scPrivateVars.desiredNumberOfSwapChainImages;
	swapchainInfo.imageFormat = scPublicVars.format;
	swapchainInfo.imageExtent.width = scPrivateVars.swapChainExtent.width;
	swapchainInfo.imageExtent.height = scPrivateVars.swapChainExtent.height;
	swapchainInfo.preTransform = scPrivateVars.preTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.presentMode = scPrivateVars.swapchainPresentMode;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
	swapchainInfo.clipped = true;
	swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.queueFamilyIndexCount = 0;
	swapchainInfo.pQueueFamilyIndices = NULL;

	result = fpCreateSwapchainKHR(rendererObj->getDevice()->device, &swapchainInfo, NULL, &scPublicVars.swapChain);
	assert(result == VK_SUCCESS);

	//Create the swapchain object
	result = fpGetSwapchainImagesKHR(rendererObj->getDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, NULL);
	assert(result == VK_SUCCESS);

	scPrivateVars.swapchainImages.clear();
	//Get the number of images the swapchain has
	scPrivateVars.swapchainImages.resize(scPublicVars.swapchainImageCount);
	assert(scPrivateVars.swapchainImages.size() >= 1);

	//Retrieve the swapchain image surfaces
	result = fpGetSwapchainImagesKHR(rendererObj->getDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, &scPrivateVars.swapchainImages[0]);
	assert(result == VK_SUCCESS);
}

void VulkanSwapchain::createColorImageView (const VkCommandBuffer commandBuffer) {
	VkResult result;
	scPublicVars.colorBuffer.clear();
	for (uint32_t i = 0; i < scPublicVars.swapchainImageCount; i++) {
		SwapChainBuffer sc_buffer;

		VkImageViewCreateInfo imgViewInfo = {};
		imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imgViewInfo.pNext = NULL;
		imgViewInfo.format = scPublicVars.format;
		imgViewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
		imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imgViewInfo.subresourceRange.baseMipLevel = 0;
		imgViewInfo.subresourceRange.levelCount = 1;
		imgViewInfo.subresourceRange.baseArrayLayer = 0;
		imgViewInfo.subresourceRange.layerCount = 1;
		imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imgViewInfo.flags = 0;

		sc_buffer.image = scPrivateVars.swapchainImages[i];
		//Since the swapchain is not owned by us we cannot set the image layout
		imgViewInfo.image = sc_buffer.image;

		result = vkCreateImageView(rendererObj->getDevice()->device, &imgViewInfo, NULL, &sc_buffer.view);
		scPublicVars.colorBuffer.push_back(sc_buffer);
		assert(result == VK_SUCCESS);
	}
	scPublicVars.currentColorBuffer = 0;
}

void VulkanSwapchain::destroySwapchain() {
	VulkanDevice* deviceObj = appObj->deviceObj;

	for (uint32_t i = 0; i < scPublicVars.swapchainImageCount; i++) {
		vkDestroyImageView(deviceObj->device, scPublicVars.colorBuffer[i].view, NULL);
	}
	fpDestroySwapchainKHR(deviceObj->device, scPublicVars.swapChain, NULL);
	vkDestroySurfaceKHR(appObj->instanceObj.instance, scPublicVars.surface, NULL);
}