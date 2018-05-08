#include "..\Headers\VulkanRenderer.h"
#include "..\Headers\VulkanApplication.h"
#include "..\Headers\VulkanWrappers.h"
#include "..\Headers\SimpleMesh.h"

VulkanRenderer::VulkanRenderer(VulkanApplication * applicationPointer, VulkanDevice * devicePointer)
{
	assert(applicationPointer != NULL);
	assert(devicePointer != NULL);

	//Note: members must be initialized with 0
	memset(&Depth, 0, sizeof(Depth));

	application = applicationPointer;
	deviceObj = devicePointer;

	swapchainObj = new VulkanSwapchain(this);
	VulkanDrawable* drawableObj = new VulkanDrawable(this);
	drawableList.push_back(drawableObj);
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::createPresentationWindow(const int & windowWidth, const int & windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(800, 600, "Project Icecaps", nullptr, nullptr);
}

void VulkanRenderer::destroyPresentationWindow()
{
	glfwDestroyWindow(window);
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

	const bool includeDepth = true;
	createRenderPass(includeDepth);
	createFrameBuffer(includeDepth);

	createShaders();

	createPipelineStateManagement();
}

void VulkanRenderer::prepare() {
	for each (VulkanDrawable* drawableObj in drawableList) {
		drawableObj->prepare();
	}
}

bool VulkanRenderer::render()
{
	if (glfwWindowShouldClose(window)){
		return false;
	}
	for each (VulkanDrawable* drawableObj in drawableList) {
		drawableObj->render();
	}
	
	glfwPollEvents();
	return true;
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

void VulkanRenderer::createDepthImage()
{

	VkResult  result;
	bool  pass;

	VkImageCreateInfo imageInfo = {};

	// If the depth format is undefined, use fallback as 16-byte value
	if (Depth.format == VK_FORMAT_UNDEFINED) {
		Depth.format = VK_FORMAT_D16_UNORM;
	}

	const VkFormat depthFormat = Depth.format;

	VkFormatProperties props;
	vkGetPhysicalDeviceFormatProperties(*deviceObj->gpu, depthFormat, &props);
	if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	else if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	}
	else {
		std::cout << "Unsupported Depth Format, try other Depth formats.\n";
		exit(-1);
	}

	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.pNext = NULL;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = depthFormat;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = NUM_SAMPLES;
	imageInfo.queueFamilyIndexCount = 0;
	imageInfo.pQueueFamilyIndices = NULL;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.flags = 0;

	// User create image info and create the image objects
	result = vkCreateImage(deviceObj->device, &imageInfo, NULL, &Depth.image);
	assert(result == VK_SUCCESS);

	// Get the image memory requirements
	VkMemoryRequirements memRqrmnt;
	vkGetImageMemoryRequirements(deviceObj->device, Depth.image, &memRqrmnt);

	VkMemoryAllocateInfo memAlloc = {};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAlloc.pNext = NULL;
	memAlloc.allocationSize = 0;
	memAlloc.memoryTypeIndex = 0;
	memAlloc.allocationSize = memRqrmnt.size;
	// Determine the type of memory required with the help of memory properties
	pass = deviceObj->memoryTypeFromProperties(memRqrmnt.memoryTypeBits, 0, /* No requirements */ &memAlloc.memoryTypeIndex);
	assert(pass);

	// Allocate the memory for image objects
	result = vkAllocateMemory(deviceObj->device, &memAlloc, NULL, &Depth.mem);
	assert(result == VK_SUCCESS);

	// Bind the allocated memeory
	result = vkBindImageMemory(deviceObj->device, Depth.image, Depth.mem, 0);
	assert(result == VK_SUCCESS);


	VkImageViewCreateInfo imgViewInfo = {};
	imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imgViewInfo.pNext = NULL;
	imgViewInfo.image = VK_NULL_HANDLE;
	imgViewInfo.format = depthFormat;
	imgViewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
	imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	imgViewInfo.subresourceRange.baseMipLevel = 0;
	imgViewInfo.subresourceRange.levelCount = 1;
	imgViewInfo.subresourceRange.baseArrayLayer = 0;
	imgViewInfo.subresourceRange.layerCount = 1;
	imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imgViewInfo.flags = 0;

	if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT ||
		depthFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
		depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) {
		imgViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	// Use command buffer to create the depth image. This includes -
	// Command buffer allocation, recording with begin/end scope and submission.
	CommandBufferManager::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdDepthImage, NULL);
	CommandBufferManager::beginCommandBuffer(cmdDepthImage);
	{
		// Set the image layout to depth stencil optimal
		setImageLayout(Depth.image,
			imgViewInfo.subresourceRange.aspectMask,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, (VkAccessFlagBits)0, cmdDepthImage);
	}
	CommandBufferManager::endCommandBuffer(cmdDepthImage);
	CommandBufferManager::submitCommandBuffer(deviceObj->queue, &cmdDepthImage);

	// Create the image view and allow the application to use the images.
	imgViewInfo.image = Depth.image;
	result = vkCreateImageView(deviceObj->device, &imgViewInfo, NULL, &Depth.view);
	assert(result == VK_SUCCESS);
}

void VulkanRenderer::createRenderPass(bool includeDepth, bool clear)
{
	VkResult result;
	//Attach the color buffer and depth buffer as an attachment to render pass instance
	VkAttachmentDescription attachments[2];
	attachments[0].format = swapchainObj->scPublicVars.format;
	attachments[0].samples = NUM_SAMPLES;
	attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].flags = 0;

	if (includeDepth) {
		attachments[1].format = Depth.format;
		attachments[1].samples = NUM_SAMPLES;
		attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[1].flags = 0;
	}

	//Define the color buffer attachment binding point and layout info
	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Define the depth buffer attachment binding point and layout info
	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//Specify the attachments
	VkSubpassDescription subpassDesc = {};
	subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDesc.flags = 0;
	subpassDesc.inputAttachmentCount = 0;
	subpassDesc.pInputAttachments = NULL;
	subpassDesc.colorAttachmentCount = 1;
	subpassDesc.pColorAttachments = &colorReference;
	subpassDesc.pResolveAttachments = NULL;
	subpassDesc.pDepthStencilAttachment = includeDepth ? &depthReference : NULL;
	subpassDesc.preserveAttachmentCount = 0;
	subpassDesc.pPreserveAttachments = NULL;

	//Specify the attachment and subpass associate with render pass
	VkRenderPassCreateInfo rpInfo = {};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpInfo.pNext = NULL;
	rpInfo.attachmentCount = includeDepth ? 2 : 1;
	rpInfo.pAttachments = attachments;
	rpInfo.subpassCount = 1;
	rpInfo.pSubpasses = &subpassDesc;
	rpInfo.dependencyCount = 0;
	rpInfo.pDependencies = NULL;

	//Create the render pass object
	result = vkCreateRenderPass(deviceObj->device, &rpInfo, NULL, &renderPass);
	assert(result == VK_SUCCESS);
}

void VulkanRenderer::createFrameBuffer(bool includeDepth, bool clear)
{
	// Dependency on createDepthBuffer(), createRenderPass() and recordSwapChain()
	VkResult  result;
	VkImageView attachments[2];
	attachments[1] = Depth.view;

	VkFramebufferCreateInfo fbInfo = {};
	fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbInfo.pNext = NULL;
	fbInfo.renderPass = renderPass;
	fbInfo.attachmentCount = includeDepth ? 2 : 1;
	fbInfo.pAttachments = attachments;
	fbInfo.width = width;
	fbInfo.height = height;
	fbInfo.layers = 1;

	uint32_t i;

	framebuffers.clear();
	framebuffers.resize(swapchainObj->scPublicVars.swapchainImageCount);
	for (i = 0; i < swapchainObj->scPublicVars.swapchainImageCount; i++) {
		attachments[0] = swapchainObj->scPublicVars.colorBuffer[i].view;
		result = vkCreateFramebuffer(deviceObj->device, &fbInfo, NULL, &framebuffers.at(i));
		assert(result == VK_SUCCESS);
	}
}

void VulkanRenderer::destroyFramebuffer()
{
	for (uint32_t i = 0; i < swapchainObj->scPublicVars.swapchainImageCount; i++) {
		vkDestroyFramebuffer(deviceObj->device, framebuffers.at(i), NULL);
	}
	framebuffers.clear();
}

void VulkanRenderer::destroyRenderpass()
{
	vkDestroyRenderPass(deviceObj->device, renderPass, NULL);
}

void VulkanRenderer::destroyDrawableVertexBuffer()
{
	for each (VulkanDrawable* drawableObj in drawableList)
	{
		drawableObj->destroyVertexBuffer();
	}
}

void VulkanRenderer::destroyDrawableCommandBuffer()
{
	for each (VulkanDrawable* drawableObj in drawableList)
	{
		drawableObj->destroyCommandBuffer();
	}
}

void VulkanRenderer::destroyDrawableSynchronizationObjects()
{
	for each (VulkanDrawable* drawableObj in drawableList)
	{
		drawableObj->destroySynchronizationObjects();
	}
}

void VulkanRenderer::destroyDepthBuffer()
{
	vkDestroyImageView(deviceObj->device, Depth.view, NULL);
	vkDestroyImage(deviceObj->device, Depth.image, NULL);
	vkFreeMemory(deviceObj->device, Depth.mem, NULL);
}

void VulkanRenderer::destroyCommandBuffer()
{
	VkCommandBuffer cmdBufs[] = { cmdDepthImage, cmdVertexBuffer };
	vkFreeCommandBuffers(deviceObj->device, cmdPool, sizeof(cmdBufs) / sizeof(VkCommandBuffer), cmdBufs);
}

void VulkanRenderer::destroyCommandPool()
{
	VulkanDevice* deviceObj = application->deviceObj;

	vkDestroyCommandPool(deviceObj->device, cmdPool, NULL);
}

void VulkanRenderer::buildSwapChainAndDepthImage()
{
	// Get the appropriate queue to submit the command into
	deviceObj->getDeviceQueue();

	// Create swapchain and get the color image
	swapchainObj->createSwapchain(cmdDepthImage);

	// Create the depth image
	createDepthImage();
}

void VulkanRenderer::createVertexBuffer()
{
	CommandBufferManager::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdVertexBuffer);
	CommandBufferManager::beginCommandBuffer(cmdVertexBuffer);

	for each (VulkanDrawable* drawableObj in drawableList)
	{
		drawableObj->createVertexBuffer(triangleData, sizeof(triangleData), sizeof(triangleData[0]), false);
	}
	CommandBufferManager::endCommandBuffer(cmdVertexBuffer);
	CommandBufferManager::submitCommandBuffer(deviceObj->queue, &cmdVertexBuffer);
}

void VulkanRenderer::createShaders()
{
	void* vertShaderCode, *fragShaderCode;
	size_t sizeVert, sizeFrag;

	vertShaderCode = readFile("../Shaders/Draw-vert.spv", &sizeVert);
	fragShaderCode = readFile("../Shaders/Draw-frag.spv", &sizeFrag);

	shaderObj.buildShaderModuleWithSPV((uint32_t*)vertShaderCode, sizeVert, (uint32_t*)fragShaderCode, sizeFrag);
}

void VulkanRenderer::createPipelineStateManagement()
{
	// UNIFROM Buffer variable initialization starts here
	pipelineObj.createPipelineCache();

	const bool depthPresent = true;
	for each (VulkanDrawable* drawableObj in drawableList)
	{
		VkPipeline* pipeline = (VkPipeline*)malloc(sizeof(VkPipeline));
		if (pipelineObj.createPipeline(drawableObj, pipeline, &shaderObj, depthPresent))
		{
			pipelineList.push_back(pipeline);
			drawableObj->setPipeline(pipeline);
		}
		else
		{
			free(pipeline);
			pipeline = NULL;
		}
	}
}

void VulkanRenderer::setImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits srcAccessMask, const VkCommandBuffer& cmd)
{
	// Dependency on cmd
	assert(cmd != VK_NULL_HANDLE);

	// The deviceObj->queue must be initialized
	assert(deviceObj->queue != VK_NULL_HANDLE);

	VkImageMemoryBarrier imgMemoryBarrier = {};
	imgMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imgMemoryBarrier.pNext = NULL;
	imgMemoryBarrier.srcAccessMask = srcAccessMask;
	imgMemoryBarrier.dstAccessMask = 0;
	imgMemoryBarrier.oldLayout = oldImageLayout;
	imgMemoryBarrier.newLayout = newImageLayout;
	imgMemoryBarrier.image = image;
	imgMemoryBarrier.subresourceRange.aspectMask = aspectMask;
	imgMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imgMemoryBarrier.subresourceRange.levelCount = 1;
	imgMemoryBarrier.subresourceRange.layerCount = 1;

	if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	}

	switch (newImageLayout)
	{
		// Ensure that anything that was copying from this image has completed
		// An image in this layout can only be used as the destination operand of the commands
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
	case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		imgMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;

		// Ensure any Copy or CPU writes to image are flushed
		// An image in this layout can only be used as a read-only shader resource
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		imgMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imgMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		break;

		// An image in this layout can only be used as a framebuffer color attachment
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		imgMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		break;

		// An image in this layout can only be used as a framebuffer depth/stencil attachment
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		imgMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		break;
	}

	VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

	vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgMemoryBarrier);
}

// Destroy each pipeline object existing in the renderer
void VulkanRenderer::destroyPipeline()
{
	for each (VkPipeline* pipeline in pipelineList)
	{
		vkDestroyPipeline(deviceObj->device, *pipeline, NULL);
		free(pipeline);
	}
	pipelineList.clear();
}