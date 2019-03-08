/*
	Wrapper class for a vulkan swapchain
*/

#pragma once
#include "V_Device.h"
#include "Global_Callbacks.h"

class V_CommandPool;

class V_Swapchain
{
private:
	//Reference tot he configuration
	configurationStructure* config;
	
	//Variables for swapchain settings
	VkSurfaceFormatKHR selectedFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D extent;
	uint32_t imageCount;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	//Actual vulkan swapchain handle
	VkSwapchainKHR swapchain;

	//Color and depth images rendered to
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	//Images in the swapchain that are accessed only through the API
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	//Vulkan classes needed for creation
	VkSurfaceKHR surface;
	V_Device* device;
	V_CommandPool* commandPool;
public:
	V_Swapchain(VkSurfaceKHR surface_in, V_Device* device_in); //Constructor

	//Intialization and creation functions
	void initialize();
	void aqcuireImages();
	void createImageViews();
	void createColorImage();
	void createDepthImage();

	//Recreates the swapchain (such as on window resize)
	void recreateSwapchain();

	//Cleans up the swapchain
	void cleanup();

	//Getters and setters
	void setCommandPoolForImageTransfers(V_CommandPool* commandPool_in) { commandPool = commandPool_in; };
	void setConfig(configurationStructure* config_in) { config = config_in; };
	VkExtent2D getExtent() { return extent; };
	VkSurfaceFormatKHR getFormat() { return selectedFormat; };
	VkSurfaceKHR getSurface() { return surface; };
	VkSwapchainKHR getSwapchain() { return swapchain; };
	VkImageView getDepthImageView () { return depthImageView; };
	VkImageView getColorImageView() { return colorImageView; };
	VkImageView getImageView(int index) { return swapchainImageViews[index]; };
	~V_Swapchain();
};

