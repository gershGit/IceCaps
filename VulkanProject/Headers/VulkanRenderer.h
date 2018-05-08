#pragma once
#include "..\Headers\Headers.h"
#include "..\Headers\VulkanSwapchain.h"
#include "..\Headers\VulkanDrawable.h"
#include "..\Headers\VulkanShader.h"
#include "..\Headers\VulkanPipeline.h"

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

class VulkanRenderer
{
private:
	VulkanApplication* application;
	// The device object associated with this Presentation layer.
	VulkanDevice* deviceObj;
	VulkanSwapchain* swapchainObj;
	std::vector<VulkanDrawable*> drawableList;
	std::vector<VkPipeline*> pipelineList;
	VulkanShader shaderObj;
	VulkanPipeline pipelineObj;

public:
	GLFWwindow* window;
	char windowName[80];
	struct {
		VkFormat		format;
		VkImage			image;
		VkDeviceMemory	mem;
		VkImageView		view;
	}Depth;
	VkCommandBuffer	cmdDepthImage;	// Command buffer for depth image layout
	VkCommandPool cmdPool;		// Command pool
	VkCommandBuffer		cmdVertexBuffer;

	int	width, height;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> framebuffers;

	//------------------Functions----------------

	void initialize(const int &width, const int &height);
	void prepare();
	bool render();

	void createPresentationWindow(const int &width = 640, const int &height = 480);
	void createShaders();
	void createPipelineStateManagement();
	void setImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits sourceAccessMask, const VkCommandBuffer& commandBuffer);
	void destroyPipeline();
	void destroyPresentationWindow();

	// Getter functions for member variable specific to classes.
	inline VulkanApplication* getApplication() { return application; }
	inline VulkanDevice*  getDevice() { return deviceObj; }
	inline VulkanSwapchain * getSwapchain() { return swapchainObj; }
	inline VulkanPipeline* getPipelineObject() { return &pipelineObj; }
	inline VulkanShader* getShader() { return &shaderObj; }

	void createCommandPool();							// Create command pool
	void buildSwapChainAndDepthImage();					// Create swapchain color image and depth image
	void createVertexBuffer();
	void createDepthImage();							// Create depth image

	void destroyCommandBuffer();
	void destroyCommandPool();
	void destroyDrawableCommandBuffer();
	void destroyDrawableSynchronizationObjects();
	void destroyDepthBuffer();

	//Record render pass command buffer
	void createRenderPassCB(bool includeDepth);

	//Render Pass creation
	void createRenderPass(bool includeDepth, bool clear = true);

	void destroyFramebuffer();

	//Destroy the render pass object when no longer required
	void destroyRenderpass();

	void destroyDrawableVertexBuffer();

	//Create and destroy FrameBuffer
	void createFrameBuffer(bool includeDepth, bool clear = true);
	void destroyFrameBuffer();

	inline VkCommandPool* getCommandPool() { return &cmdPool; }

	VulkanRenderer(VulkanApplication * applicationPointer, VulkanDevice * devicePointer);
	~VulkanRenderer();
};
