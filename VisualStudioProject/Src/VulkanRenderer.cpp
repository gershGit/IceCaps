#include "..\Headers\VulkanRenderer.h"


VulkanRenderer::VulkanRenderer(VulkanApplication * applicationPointer, VulkanDevice * devicePointer)
{
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::initialize(const int &width = 640, const int &height = 480)
{
	//Create an empty glfw window
	createPresentationWindow(width, height);

	//Initialize swapchain
	swapchainObj->initialize();

	//Create a command pool
	createCommandPool();

	//Create the necessary images for the swapchain
	buildSwapChainAndDepthImage();
}

bool VulkanRenderer::render()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	return true;
}

void VulkanRenderer::createPresentationWindow(const int & windowWidth, const int & windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}

void VulkanRenderer::destroyPresentationWindow()
{
	glfwDestroyWindow(window);
}

void VulkanRenderer::createCommandPool()
{
	//Retrieve a pointer to the device
	VulkanDevice* deviceObj = application->deviceObj;

	VkResult res;

	//Create the command pool info
	VkCommandPoolCreateInfo defaultPoolInfo = {};
	defaultPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	defaultPoolInfo.pNext = NULL;
	defaultPoolInfo.queueFamilyIndex = deviceObj->graphicsQueueWithPresentIndex;
	defaultPoolInfo.flags = 0;

	res = vkCreateCommandPool(deviceObj->device, &defaultPoolInfo, NULL, &cmdPool);
	assert(res == VK_SUCCESS);
}

void VulkanRenderer::buildSwapChainAndDepthImage()
{
	//Get the device queue
	deviceObj->getDeviceQueue();

	//Create the swapchain
	swapchainObj->createSwapchain(cmdDepthImage);

	//Create the depth image
	createDepthImage();
}
