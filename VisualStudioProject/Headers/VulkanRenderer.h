#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Headers.h"
#include "VulkanApplication.h"
#include <GLFW/glfw3.h>
#include "VulkanSwapchain.h"

class VulkanRenderer
{
private:
	VulkanApplication* application;
	// The device object associated with this Presentation layer.
	VulkanDevice* deviceObj;
	VulkanSwapchain* swapchainObj;

public:
	GLFWwindow* window;
	char windowName[80];
	struct Depth {
		VkFormat format;
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;
	};
	VkCommandBuffer		cmdDepthImage;	// Command buffer for depth image layout
	VkCommandPool		cmdPool;		// Command pool
	int	width, height;

	//------------------Functions----------------

	void initialize(const int &width = 640, const int &height = 480);
	void render();

	void createPresentationWindow(const int &width = 640, const int &height = 480);
	void setImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits sourceAccessMask, const VkCommandBuffer& commandBuffer);
	void destroyPresentationWindow();

	// Getter functions for member variable specific to classes.
	inline VulkanApplication* getApplication() { return application; }
	inline VulkanDevice*  getDevice() { return deviceObj; }
	//inline VulkanSwapChain*  getSwapChain() { return swapChainObj; }

	void createCommandPool();							// Create command pool
	void buildSwapChainAndDepthImage();					// Create swapchain color image and depth image
	void createDepthImage();							// Create depth image

	void destroyCommandBuffer();
	void destroyCommandPool();
	void destroyDepthBuffer();

	VulkanRenderer(VulkanApplication * applicationPointer, VulkanDevice * devicePointer);
	~VulkanRenderer();
};
