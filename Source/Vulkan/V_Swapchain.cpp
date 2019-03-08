#include "V_Swapchain.h"
#include <algorithm>
#include "V_BufferHelper.h"
#include "Global_Callbacks.h"
#include "StringTranslation.h"
#include <array>

V_Swapchain::V_Swapchain(VkSurfaceKHR surface_in, V_Device* device_in)
{
	device = device_in;
	surface = surface_in;
}

//Chooses a surface format from the available formats and a configuration
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, configurationStructure* config) {
	VkSurfaceFormatKHR chosenFormat;

	bool found = false;
	std::cout << "Formats Available:" << std::endl;
	//Search for a format that matches the configuration file
	for (VkSurfaceFormatKHR availableFormat : availableFormats) {
		std::cout << "\t" << formatToString(availableFormat).c_str() << std::endl;
		if (availableFormat.format == config->surfaceFormat.format && availableFormat.colorSpace == config->surfaceFormat.colorSpace) {
			found = true;
			chosenFormat = availableFormat;
		}
	}
	if (found) {
		std::cout << "format chosen: " << formatToString(chosenFormat).c_str() << std::endl;
		return chosenFormat;
	}

	//Can chooses any format
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		std::cout << "\t" << "format chosen: BGRA UNORM" << std::endl;
		return config->surfaceFormat;
	}

	//Format not found so first available format is used
	std::cout << "\t" << "format chosen: " << formatToString(availableFormats[0]).c_str() << std::endl;
	return availableFormats[0];
}

//Chooses a presentation mode from available modes and a configuraion
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes, configurationStructure* config) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	std::cout << "Present modes available:" << std::endl;
	bool found = false, found_mail = false;
	for (const auto& availablePresentMode : availablePresentModes) {
		std::cout << "\t" << presentModeToString(availablePresentMode).c_str() << std::endl;
		if (availablePresentMode == config->presentMode) {
			found = true;
			bestMode = availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR && !found) {
			found_mail = true;
			bestMode = availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR && !found && !found_mail) {
			bestMode = availablePresentMode;
		}
	}
	std::cout << "Present mode chosen: " << presentModeToString(bestMode).c_str() << std::endl;
	return bestMode;
}

//Chooses an extent given the capabilites of the surface
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, configurationStructure* config) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		std::cout << "Extent chosen: " << capabilities.currentExtent.width << ", " << capabilities.currentExtent.height << std::endl;
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(config->window, &width, &height);
		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.minImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}

//Chooses an image count given capabilities and user set preferences
uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities, configurationStructure* config) {
	uint32_t count = capabilities.minImageCount + (config->swapchainBuffering - capabilities.minImageCount);
	if (count < 1) {
		count = 1;
	}
	if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount) {
		std::cout << "\t" << "count of images chosen: " << capabilities.maxImageCount << std::endl;
		return capabilities.maxImageCount;
	}
	std::cout << "Count of images chosen: " << count << std::endl;
	return count;
}

//Initializes a swapchain
void V_Swapchain::initialize()
{
	std::cout << "Building swapchain" << std::endl;
	if (config->anti_aliasing_resolution <= getMaxUsableSampleCount(device->getPhysicalDevice())) {
		msaaSamples = intToSampleFlagBits(config->anti_aliasing_resolution);
	}
	else {
		msaaSamples = getMaxUsableSampleCountFlags(device->getPhysicalDevice());
	}

	SwapchainSupportDetails details = querySwapchainSupport(surface, device->getPhysicalDevice());
	selectedFormat = chooseSwapSurfaceFormat(details.formats, config);
	presentMode = chooseSwapPresentMode(details.presentModes, config);
	extent = chooseSwapExtent(details.capabilities, config);
	imageCount = chooseImageCount(details.capabilities, config);

	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = surface;
	swapchainInfo.minImageCount = imageCount;
	swapchainInfo.imageFormat = selectedFormat.format;
	swapchainInfo.imageColorSpace = selectedFormat.colorSpace;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1; //Only change for stereoscpoic 3D
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(device->getPhysicalDevice(), surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	//Ensure coherency between queue families if the graphics queue family is different than the present family
	if (indices.graphicsFamily != indices.presentFamily) {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = nullptr;
	}
	swapchainInfo.preTransform = details.capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult res = vkCreateSwapchainKHR(device->getLogicalDevice(), &swapchainInfo, nullptr, &swapchain);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}
	std::cout << "\tSwapchain initialized" << std::endl;
	aqcuireImages();
	createImageViews();
}

//Gets images from from vulkan API
void V_Swapchain::aqcuireImages()
{
	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapchain, &imageCount, swapchainImages.data());
	std::cout << "\t" << imageCount << " Images aqcuired from swapchain" << std::endl;
}

//Creates images for attachments
void V_Swapchain::createImageViews()
{
	std::cout << "\t" << "Creating image views" << std::endl;
	swapchainImageViews.resize(swapchainImages.size());
	for (size_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = V_BufferHelper::createImageView(device, swapchainImages[i], selectedFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, true);
	}
}
void V_Swapchain::createColorImage() {
	if (config->debugOn) {
		std::cout << "Creating Color Image" << std::endl;
	}

	V_BufferHelper::createImage(device, extent.width, extent.height, 1, msaaSamples, selectedFormat.format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	colorImageView = V_BufferHelper::createImageView(device, colorImage, selectedFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	V_BufferHelper::transitionImageLayout(commandPool, colorImage, selectedFormat.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
}
void V_Swapchain::createDepthImage()
{
	if (config->debugOn) {
		std::cout << "Creating Depth Image" << std::endl;
	}

	V_BufferHelper::createImage(device, extent.width, extent.height, 1, msaaSamples, config->depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = V_BufferHelper::createImageView(device, depthImage, config->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, true);
	V_BufferHelper::transitionImageLayout(commandPool, depthImage, config->depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

//Recreates swapchain when window is resized
void V_Swapchain::recreateSwapchain()
{
	cleanup();
	initialize();
	createColorImage();
	createDepthImage();
}

//Cleans up vulkan objects
void V_Swapchain::cleanup()
{
	std::cout << "\tDestroying Depth Image and Views" << std::endl;
	vkDestroyImageView(device->getLogicalDevice(), depthImageView, nullptr);
	vkDestroyImage(device->getLogicalDevice(), depthImage, nullptr);
	vkFreeMemory(device->getLogicalDevice(), depthImageMemory, nullptr);

	std::cout << "\tDestroying Swapchain Image Views" << std::endl;
	vkDestroyImageView(device->getLogicalDevice(), colorImageView, nullptr);
	vkDestroyImage(device->getLogicalDevice(), colorImage, nullptr);
	vkFreeMemory(device->getLogicalDevice(), colorImageMemory, nullptr);

	for (auto imageView : swapchainImageViews) {
		vkDestroyImageView(device->getLogicalDevice(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(device->getLogicalDevice(), swapchain, nullptr);
}

V_Swapchain::~V_Swapchain()
{
}