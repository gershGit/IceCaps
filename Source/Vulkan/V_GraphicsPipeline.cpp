#include "Vulkan/V_GraphicsPipeline.h"
#include "Vulkan/V_BufferHelper.h"
#include <array>

//Sets the device and swapchain
V_GraphicsPipeline::V_GraphicsPipeline(V_Device* device_in, V_Swapchain* swapchain_in)
{
	device = device_in;
	swapchain = swapchain_in;
}

//Creates other vulkan objects needed for a pipeline
void V_GraphicsPipeline::createRenderPass()
{
	std::cout << "Creating render pass" << std::endl;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapchain->getFormat().format;
	colorAttachment.samples = intToSampleFlagBits(config->anti_aliasing_resolution);
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = config->depthFormat;
	depthAttachment.samples = intToSampleFlagBits(config->anti_aliasing_resolution);
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentResolve = {};
	colorAttachmentResolve.format = swapchain->getFormat().format;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentResolveRef = {};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkResult res = vkCreateRenderPass(device->getLogicalDevice(), &renderPassInfo, nullptr, &renderPass);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
	}
}
void V_GraphicsPipeline::createFrameBuffers()
{
	frameBuffers.resize(config->swapchainBuffering);
	for (int i = 0; i < config->swapchainBuffering; i++) {
		std::array<VkImageView, 3> attachments = {
			swapchain->getColorImageView(),
			swapchain->getDepthImageView(),
			swapchain->getImageView((int) i)
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchain->getExtent().width;
		framebufferInfo.height = swapchain->getExtent().height;
		framebufferInfo.layers = 1;

		VkResult res = vkCreateFramebuffer(device->getLogicalDevice(), &framebufferInfo, nullptr, &frameBuffers[i]);
		if (res != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

//Initialization based on shaders or a material
void V_GraphicsPipeline::initialize(const std::string& vertex_shader, const std::string& frag_shader)
{
	std::cout << "Building graphics pipeline" << std::endl;

	auto vertShaderCode = readShader(vertex_shader);
	auto fragShaderCode = readShader(frag_shader);

	std::cout << "\t" << vertShaderCode.size() << " bytes stored for vertex shader" << std::endl;
	std::cout << "\t" << fragShaderCode.size() << " bytes stored for fragment shader" << std::endl;

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device->getLogicalDevice());
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device->getLogicalDevice());

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
	std::cout << "\tprogrammable stages set" << std::endl;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; 
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapchain->getExtent().width;
	viewport.height = (float)swapchain->getExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = swapchain->getExtent();

	VkPipelineViewportStateCreateInfo viewportStateInfo = {};
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
	rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerInfo.depthClampEnable = VK_FALSE;
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerInfo.depthBiasEnable = VK_FALSE;
	rasterizerInfo.depthBiasConstantFactor = 0.0f;
	rasterizerInfo.depthBiasClamp = 0.0f;
	rasterizerInfo.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisamplingInfo = {};
	multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingInfo.sampleShadingEnable = VK_FALSE;
	multisamplingInfo.rasterizationSamples = intToSampleFlagBits(config->anti_aliasing_resolution);
	multisamplingInfo.minSampleShading = 1.0f;
	multisamplingInfo.pSampleMask = nullptr;
	multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
	colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingInfo.logicOpEnable = VK_FALSE;
	colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendingInfo.attachmentCount = 1;
	colorBlendingInfo.pAttachments = &colorBlendAttachment;
	colorBlendingInfo.blendConstants[0] = 0.0f;
	colorBlendingInfo.blendConstants[1] = 0.0f;
	colorBlendingInfo.blendConstants[2] = 0.0f;
	colorBlendingInfo.blendConstants[3] = 0.0f;

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.pDynamicStates = dynamicStates;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	//Push constant layout
	VkPushConstantRange pushConstantRange = VkPushConstantRange();
	pushConstantRange.size = sizeof(glm::mat4);
	pushConstantRange.offset = 0;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VkResult res = vkCreatePipelineLayout(device->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create a pipeline layout!");
	}
	std::cout << "\tconfigurable stages set" << std::endl;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportStateInfo;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisamplingInfo;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlendingInfo;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	res = vkCreateGraphicsPipelines(device->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	std::cout << "\tCreated pipeline" << std::endl;

	cleanupShaderModules(vertShaderModule, fragShaderModule);
}
void V_GraphicsPipeline::initialize(material_type mType)
{
	type = mType;
	std::string vertexShader, fragmentShader;
	if (mType == PBR) {
		vertexShader = "../../Source/Shaders/vbuffer.vert.spv";
		fragmentShader = "../../Source/Shaders/shader.frag.spv";
		descriptorSetLayouts.resize(2);
		createDescriptorSetLayout(mType);
		createBindingDescription(PBR);
		createRenderPass();
		createFrameBuffers();
	}

	initialize(vertexShader, fragmentShader);
}

//Creates vulkan objects related to the description of the pipeline
void V_GraphicsPipeline::createDescriptorSetLayout(material_type m_type)
{
	//Make descriptor sets match with the type of objects that will be rendered with the pipeline
	VkDescriptorSetLayoutBinding viewPerspBinding = {};
	viewPerspBinding.binding = 0;
	viewPerspBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	viewPerspBinding.descriptorCount = 1;
	viewPerspBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	viewPerspBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding lightsBinding = {};
	lightsBinding.binding = 1;
	lightsBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightsBinding.descriptorCount = 1;
	lightsBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	lightsBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding cameraSceneBindings[2];
	cameraSceneBindings[0] = viewPerspBinding;
	cameraSceneBindings[1] = lightsBinding;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	if (m_type == PBR) {
		samplerLayoutBinding.descriptorCount = 5;
	}
	else {
		samplerLayoutBinding.descriptorCount = 1;
	}
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	//One set for uniform buffers to be updated every frame
	VkDescriptorSetLayoutCreateInfo cameraSceneInfo = {};
	cameraSceneInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	cameraSceneInfo.bindingCount = 2;
	cameraSceneInfo.pBindings = cameraSceneBindings;
	VkResult res = vkCreateDescriptorSetLayout(device->getLogicalDevice(), &cameraSceneInfo, nullptr, &descriptorSetLayouts.at(0));
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	//One set for textures that should rarely if ever be updated
	VkDescriptorSetLayoutCreateInfo textureLayoutInfo = {};
	textureLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	textureLayoutInfo.bindingCount = 1;
	textureLayoutInfo.pBindings = &samplerLayoutBinding;
	res = vkCreateDescriptorSetLayout(device->getLogicalDevice(), &textureLayoutInfo, nullptr, &descriptorSetLayouts.at(1));
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}
void V_GraphicsPipeline::createBindingDescription(material_type m_type)
{
	if (m_type == PBR) {
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		attributeDescriptions.resize(4);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(vertex, tangent);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(vertex, uv);
	}
}

//Builds the buffers for the lights and cameras
void V_GraphicsPipeline::buildLightCameraBuffers(int max_lights_in, int bufferCount)
{
	max_lights = max_lights_in;
	//Get the size for a single mesh uniform buffer
	VkDeviceSize lightsBufferSize = sizeof(ViewPersp) + sizeof(LightObject) * max_lights;

	//Make enough room in the mesh for all the buffers
	lightViewBuffers.resize(bufferCount);
	lightViewBufferVRAMs.resize(bufferCount);

	//Create the buffers on VRAM
	for (size_t i = 0; i < bufferCount; i++) {
		V_BufferHelper::createBuffer(device, lightsBufferSize, 
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			lightViewBuffers[i], lightViewBufferVRAMs[i]);
	}
}

//Clean up functions
void V_GraphicsPipeline::cleanupShaderModules(VkShaderModule vertShader, VkShaderModule fragShader)
{
	vkDestroyShaderModule(device->getLogicalDevice(), fragShader, nullptr);
	vkDestroyShaderModule(device->getLogicalDevice(), vertShader, nullptr);
}
void V_GraphicsPipeline::cleanup()
{
	std::cout << "\tDestroying light buffers" << std::endl;
	for (int i = 0; i < (int) lightViewBuffers.size(); i++) {
		vkDestroyBuffer(device->getLogicalDevice(), lightViewBuffers[i], nullptr);
		vkFreeMemory(device->getLogicalDevice(), lightViewBufferVRAMs[i], nullptr);
	}
	
	std::cout << "\tDestroying Render Pass" << std::endl;
	vkDestroyRenderPass(device->getLogicalDevice(), renderPass, nullptr);
	std::cout << "\tDestroying Frame Buffers" << std::endl;
	for (VkFramebuffer framebuffer : frameBuffers) {
		vkDestroyFramebuffer(device->getLogicalDevice(), framebuffer, nullptr);
	}
	std::cout << "\tDestroying Graphics Pipeline" << std::endl;
	vkDestroyPipeline(device->getLogicalDevice(), pipeline, nullptr);
	std::cout << "\tDestroying Graphics Pipeline Layout" << std::endl;
	vkDestroyPipelineLayout(device->getLogicalDevice(), pipelineLayout, nullptr);
	std::cout << "\tDestroying Descriptor Set Layouts" << std::endl;
	vkDestroyDescriptorSetLayout(device->getLogicalDevice(), descriptorSetLayouts[0], nullptr);
	vkDestroyDescriptorSetLayout(device->getLogicalDevice(), descriptorSetLayouts[1], nullptr);
}

//Called when the swapchain has been reset
void V_GraphicsPipeline::swapchainReset()
{
	//Cleaning up old version
	std::cout << "\tDestroying Render Pass" << std::endl;
	vkDestroyRenderPass(device->getLogicalDevice(), renderPass, nullptr);
	std::cout << "\tDestroying Frame Buffers" << std::endl;
	for (VkFramebuffer framebuffer : frameBuffers) {
		vkDestroyFramebuffer(device->getLogicalDevice(), framebuffer, nullptr);
	}
	std::cout << "\tDestroying Graphics Pipeline" << std::endl;
	vkDestroyPipeline(device->getLogicalDevice(), pipeline, nullptr);
	std::cout << "\tDestroying Graphics Pipeline Layout" << std::endl;
	vkDestroyPipelineLayout(device->getLogicalDevice(), pipelineLayout, nullptr);
	std::cout << "\tDestroying Descriptor Set Layouts" << std::endl;
	vkDestroyDescriptorSetLayout(device->getLogicalDevice(), descriptorSetLayouts[0], nullptr);
	vkDestroyDescriptorSetLayout(device->getLogicalDevice(), descriptorSetLayouts[1], nullptr);

	initialize(type);
}

V_GraphicsPipeline::~V_GraphicsPipeline()
{
}