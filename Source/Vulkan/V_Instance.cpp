#include "V_Instance.h"
#include <set>
#include "StringTranslation.h"
#include <array>


V_Instance::V_Instance(configurationStructure* config_in)
{
	config = config_in;
}

//Returns a vector of the extensions required for the program to run
std::vector<const char*> V_Instance::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	std::cout << "Required extensions are: " << std::endl;
	for (const char* name : extensions) {
		std::cout << "\t" << name << std::endl;
	}

	return extensions;
}

//Sets up validation layers based on a vector in the h file if in debug mode
bool V_Instance::setupValidationLayers() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector <VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	std::cout << "Validation layers found:" << std::endl;
	for (const auto& layerProperties : availableLayers) {
		std::cout << "\t" << layerProperties.layerName << std::endl;
	}

	for (const char * layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				std::cout << "Enabled " << layerProperties.layerName << std::endl;
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			return false;
		}
	}
	return true;
}

//Prints out all of the extensions available
void V_Instance::enumerateExtensions() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "The following extensions are supported on this system: " << std::endl;
	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

//Links the debugCallback function to the messenger
void V_Instance::setupDebugCallback()
{
	if (enableValidationLayers) {
		VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};
		messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messengerInfo.pfnUserCallback = vulkan_debugCallback;
		messengerInfo.pUserData = nullptr;

		VkResult result = CreateDebugUtilsMessengerEXT(instance, &messengerInfo, nullptr, &callback);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
	}
}

//Initialized vilkan including adding the correct extensions and validation layers
void V_Instance::initialize(configurationStructure &config)
{
	//Attempt to setup validation layers if requested
	if (enableValidationLayers && !setupValidationLayers()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = config.appName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(config.appVersion[0], config.appVersion[1], config.appVersion[2]);
	appInfo.pEngineName = "Icecaps";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 5);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	//Add validation layers if in debug mode to the instance object
	if (enableValidationLayers) {
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		instanceInfo.enabledLayerCount = 0;
	}

	//Add the necessary extensions to the instance
	std::vector<const char*> extensions = getRequiredExtensions();
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

//Creates a surface that can be sent to windows
void V_Instance::createSurface()
{
	VkResult res = glfwCreateWindowSurface(instance, config->window, nullptr, &vulkanSurface);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}

//Checks if the device supports necessary device extensions for this application
bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> &deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	std::cout << "Device Extensions found: " << std::endl;
	for (const auto& extension : availableExtensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

//Checks if supported features match required features
bool featuresSupported(configurationStructure * config, VkPhysicalDeviceFeatures supportedFeatures) {
	if (config->featuresRequired.alphaToOne && !supportedFeatures.alphaToOne) {
		return false;
	}
	else if (config->featuresRequired.depthBiasClamp && !supportedFeatures.depthBiasClamp) {
		return false;
	}
	else if (config->featuresRequired.depthBounds && !supportedFeatures.depthBounds) {
		return false;
	}
	else if (config->featuresRequired.depthClamp && !supportedFeatures.depthClamp) {
		return false;
	}
	else if (config->featuresRequired.drawIndirectFirstInstance && !supportedFeatures.drawIndirectFirstInstance) {
		return false;
	}
	else if (config->featuresRequired.dualSrcBlend && !supportedFeatures.dualSrcBlend) {
		return false;
	}
	else if (config->featuresRequired.fillModeNonSolid && !supportedFeatures.fillModeNonSolid) {
		return false;
	}
	else if (config->featuresRequired.fragmentStoresAndAtomics && !supportedFeatures.fragmentStoresAndAtomics) {
		return false;
	}
	else if (config->featuresRequired.fullDrawIndexUint32 && !supportedFeatures.fullDrawIndexUint32) {
		return false;
	}
	else if (config->featuresRequired.geometryShader && !supportedFeatures.geometryShader) {
		return false;
	}
	else if (config->featuresRequired.imageCubeArray && !supportedFeatures.imageCubeArray) {
		return false;
	}
	else if (config->featuresRequired.independentBlend && !supportedFeatures.independentBlend) {
		return false;
	}
	else if (config->featuresRequired.inheritedQueries && !supportedFeatures.inheritedQueries) {
		return false;
	}
	else if (config->featuresRequired.largePoints && !supportedFeatures.largePoints) {
		return false;
	}
	else if (config->featuresRequired.logicOp && !supportedFeatures.logicOp) {
		return false;
	}
	else if (config->featuresRequired.multiDrawIndirect && !supportedFeatures.multiDrawIndirect) {
		return false;
	}
	else if (config->featuresRequired.multiViewport && !supportedFeatures.multiViewport) {
		return false;
	}
	else if (config->featuresRequired.occlusionQueryPrecise && !supportedFeatures.occlusionQueryPrecise) {
		return false;
	}
	else if (config->featuresRequired.pipelineStatisticsQuery && !supportedFeatures.pipelineStatisticsQuery) {
		return false;
	}
	else if (config->featuresRequired.robustBufferAccess && !supportedFeatures.robustBufferAccess) {
		return false;
	}
	else if (config->featuresRequired.samplerAnisotropy && !supportedFeatures.samplerAnisotropy) {
		return false;
	}
	else if (config->featuresRequired.sampleRateShading && !supportedFeatures.sampleRateShading) {
		return false;
	}
	else if (config->featuresRequired.shaderClipDistance && !supportedFeatures.shaderClipDistance) {
		return false;
	}
	else if (config->featuresRequired.shaderCullDistance && !supportedFeatures.shaderCullDistance) {
		return false;
	}
	else if (config->featuresRequired.shaderFloat64 && !supportedFeatures.shaderFloat64) {
		return false;
	}
	else if (config->featuresRequired.shaderImageGatherExtended && !supportedFeatures.shaderImageGatherExtended) {
		return false;
	}
	else if (config->featuresRequired.shaderInt16 && !supportedFeatures.shaderInt16) {
		return false;
	}
	else if (config->featuresRequired.shaderInt64 && !supportedFeatures.shaderInt64) {
		return false;
	}
	else if (config->featuresRequired.shaderResourceMinLod && !supportedFeatures.shaderResourceMinLod) {
		return false;
	}
	else if (config->featuresRequired.shaderResourceResidency && !supportedFeatures.shaderResourceResidency) {
		return false;
	}
	else if (config->featuresRequired.shaderSampledImageArrayDynamicIndexing && !supportedFeatures.shaderSampledImageArrayDynamicIndexing) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageBufferArrayDynamicIndexing && !supportedFeatures.shaderStorageBufferArrayDynamicIndexing) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageImageArrayDynamicIndexing && !supportedFeatures.shaderStorageImageArrayDynamicIndexing) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageImageExtendedFormats && !supportedFeatures.shaderStorageImageExtendedFormats) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageImageMultisample && !supportedFeatures.shaderStorageImageMultisample) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageImageReadWithoutFormat && !supportedFeatures.shaderStorageImageReadWithoutFormat) {
		return false;
	}
	else if (config->featuresRequired.shaderStorageImageWriteWithoutFormat && !supportedFeatures.shaderStorageImageWriteWithoutFormat) {
		return false;
	}
	else if (config->featuresRequired.shaderTessellationAndGeometryPointSize && !supportedFeatures.shaderTessellationAndGeometryPointSize) {
		return false;
	}
	else if (config->featuresRequired.shaderUniformBufferArrayDynamicIndexing && !supportedFeatures.shaderUniformBufferArrayDynamicIndexing) {
		return false;
	}
	else if (config->featuresRequired.sparseBinding && !supportedFeatures.sparseBinding) {
		return false;
	}
	else if (config->featuresRequired.sparseResidency16Samples && !supportedFeatures.sparseResidency16Samples) {
		return false;
	}
	else if (config->featuresRequired.sparseResidency2Samples && !supportedFeatures.sparseResidency2Samples) {
		return false;
	}
	else if (config->featuresRequired.sparseResidency4Samples && !supportedFeatures.sparseResidency4Samples) {
		return false;
	}
	else if (config->featuresRequired.sparseResidency8Samples && !supportedFeatures.sparseResidency8Samples) {
		return false;
	}
	else if (config->featuresRequired.sparseResidencyAliased && !supportedFeatures.sparseResidencyAliased) {
		return false;
	}
	else if (config->featuresRequired.sparseResidencyBuffer && !supportedFeatures.sparseResidencyBuffer) {
		return false;
	}
	else if (config->featuresRequired.sparseResidencyImage2D && !supportedFeatures.sparseResidencyImage2D) {
		return false;
	}
	else if (config->featuresRequired.sparseResidencyImage3D && !supportedFeatures.sparseResidencyImage3D) {
		return false;
	}
	else if (config->featuresRequired.tessellationShader && !supportedFeatures.tessellationShader) {
		return false;
	}
	else if (config->featuresRequired.textureCompressionASTC_LDR && !supportedFeatures.textureCompressionASTC_LDR) {
		return false;
	}
	else if (config->featuresRequired.textureCompressionBC && !supportedFeatures.textureCompressionBC) {
		return false;
	}
	else if (config->featuresRequired.textureCompressionETC2 && !supportedFeatures.textureCompressionETC2) {
		return false;
	}
	else if (config->featuresRequired.variableMultisampleRate && !supportedFeatures.variableMultisampleRate) {
		return false;
	}
	else if (config->featuresRequired.vertexPipelineStoresAndAtomics && !supportedFeatures.vertexPipelineStoresAndAtomics) {
		return false;
	}
	else if (config->featuresRequired.wideLines && !supportedFeatures.wideLines) {
		return false;
	}
	return true;
}

//Scores devices depending on how good they are for the engine (0 if it wont work)
int isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> &deviceExtensions, configurationStructure* config) {
	int score = 0;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	std::cout << "\t" << deviceProperties.deviceName << std::endl;
	std::cout << "\tMax Push Constant Size: " << deviceProperties.limits.maxPushConstantsSize << std::endl;

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 2; }
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) { score += 1; }
	QueueFamilyIndices indices = findQueueFamilies(device, surface);

	bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

	bool swapchainAdequete = false;
	if (extensionsSupported) {
		SwapchainSupportDetails swapchainSupport = querySwapchainSupport(surface, device);
		swapchainAdequete = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
	displayFeatures(supportedFeatures);

	if (!indices.isComplete() || !extensionsSupported || !swapchainAdequete || !featuresSupported(config, supportedFeatures)) {
		score = 0;
	}
	else if (!indices.isCompleteWithCompute() && config->computeRequired) {
		score = 0;
	}

	return score;
}

//Attaches V_Device objects to the V_Instance
void V_Instance::attachDevices(unsigned int coreCount)
{
	//Enumerate the physical devices detected by the driver
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	std::vector<int> scores(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	VkPhysicalDevice selectedPrimaryPhysicalDevice = VK_NULL_HANDLE;
	int maxScore = 0;

	std::cout << devices.size() << " devices found on system: " << std::endl;
	//Choose the device with best score
	for (const auto& device : devices) {
		int score = isDeviceSuitable(device, vulkanSurface, deviceExtensions, config);
		if (score > maxScore) {
			selectedPrimaryPhysicalDevice = device;
			maxScore = score;
		}
	}

	if (config->gpuSelected != -1 && config->gpuSelected < devices.size()) {
		selectedPrimaryPhysicalDevice = devices[config->gpuSelected];
	}

	if (selectedPrimaryPhysicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	else {
		primaryDevice = new V_Device();
		primaryDevice->initialize(selectedPrimaryPhysicalDevice, vulkanSurface, config);
	}
}

//Creates a command pool only for transfer commands using (hopefully) a unique queue family
void V_Instance::createTransferCommandPools()
{
	transferCommandPool = new V_CommandPool();
	transferCommandPool->createCommandPool(TRANSFER, primaryDevice, vulkanSurface);
	imageOperationsPool = new V_CommandPool();
	imageOperationsPool->createCommandPool(IMAGE_OPERATIONS, primaryDevice, vulkanSurface);
}

//Creates and attaches a swapchain to the instance
void V_Instance::createSwapChain()
{
	swapchain = new V_Swapchain(vulkanSurface, primaryDevice);
	swapchain->setConfig(config);
	swapchain->setCommandPoolForImageTransfers(imageOperationsPool);
	swapchain->initialize();
	swapchain->createColorImage();
	swapchain->createDepthImage();
}

//Adds a graphics pipeline to support a material given a number of maximum lights
void V_Instance::addGraphicsPipeline(material_type mType, int max_lights)
{
	int index = static_cast<int>(graphicsPipelines->size());
	graphicsPipelines->push_back(new V_GraphicsPipeline(primaryDevice, swapchain));
	(*graphicsPipelines)[index]->setConfig(config);
	(*graphicsPipelines)[index]->initialize(mType);
	(*graphicsPipelines)[index]->buildLightCameraBuffers(max_lights, config->swapchainBuffering);
}

//Creates a texture sampler
void V_Instance::createSampler()
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = (float) config->anisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VkResult res = vkCreateSampler(primaryDevice->getLogicalDevice(), &samplerInfo, nullptr, &textureSampler);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

//Creates command pools for each core
void V_Instance::createCommandPools(unsigned int cores)
{
	graphicsCommandPools.resize(cores);
	for (unsigned int i = 0; i < cores; i++) {
		graphicsCommandPools[i] = new V_CommandPool();
		graphicsCommandPools[i]->createCommandPool(GRAPHICS, primaryDevice, vulkanSurface);
		graphicsCommandPools[i]->allocateCommandBuffers(config->swapchainBuffering);
	}
	
	computeCommandPools.resize(cores);
	for (unsigned int i = 0; i < cores; i++) {
		computeCommandPools[i] = new V_CommandPool();
		computeCommandPools[i]->createCommandPool(COMPUTE, primaryDevice, vulkanSurface);
		computeCommandPools[i]->allocateCommandBuffers(config->swapchainBuffering);
	}
}

//Creates a descriptor pool for objects at scene load time
void V_Instance::createStaticDescriptorPool(int uniformBuffers, int imageBuffers, int drawableCount)
{
	staticObjectsDescriptorPool = new V_DescriptorPool();
	staticObjectsDescriptorPool->setDevice(primaryDevice);
	staticObjectsDescriptorPool->setSwapchain(swapchain);
	staticObjectsDescriptorPool->setConfig(config);
	staticObjectsDescriptorPool->initialize(uniformBuffers, imageBuffers, drawableCount);
}

//Creates objects for syncing frames
void V_Instance::createSyncObjects()
{
	imageAvailableSemaphores.resize(config->swapchainBuffering);
	renderFinishedSemaphores.resize(config->swapchainBuffering);
	inFlightFences.resize(config->swapchainBuffering);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < config->swapchainBuffering; i++) {
		VkResult res1 = vkCreateSemaphore(primaryDevice->getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
		VkResult res2 = vkCreateSemaphore(primaryDevice->getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
		VkResult res3 = vkCreateFence(primaryDevice->getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]);

		if (res1 != VK_SUCCESS || res2 != VK_SUCCESS || res3 != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects!");
		}
	}
}

//Increments the frame in flight index
void V_Instance::incrementFrame()
{
	currentFrame = (currentFrame + 1) % config->swapchainBuffering;
}

//Recreates the swapchain if necessary
void V_Instance::recreateSwapchain()
{
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(config->window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(primaryDevice->getLogicalDevice());
	
	glfwGetFramebufferSize(config->window, &width, &height);
	if (height != config->videoSettings->height || width != config->videoSettings->width) {
		std::cout << "\n\nRecreating swapchain -------------\n\n" << std::endl;

		config->changed = true;
		config->videoSettings->width = width;
		config->videoSettings->height = height;
		swapchain->recreateSwapchain();
		for (V_GraphicsPipeline* pipeline :* graphicsPipelines) {
			pipeline->swapchainReset();
		}
	}
}

//Cleans up the V_Instance
void V_Instance::cleanup() {
#ifdef NDEBUG
#else
	std::cout << "Initializing vulkan cleanup" << std::endl;
#endif

	std::cout << "\tCleaning up command pools" << std::endl;
	for (auto commandPool : graphicsCommandPools) {
		commandPool->cleanup();
	}

	for (auto commandPool : computeCommandPools) {
		commandPool->cleanup();
	}

	transferCommandPool->cleanup();
	imageOperationsPool->cleanup();
	staticObjectsDescriptorPool->cleanup();

	//Destroy the callback messenger if it was loaded
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	}

	for (size_t i = 0; i < config->swapchainBuffering; i++) {
		vkDestroySemaphore(primaryDevice->getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(primaryDevice->getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(primaryDevice->getLogicalDevice(), inFlightFences[i], nullptr);
	}

	swapchain->cleanup();

	for (V_DescriptorPool* pool : descriptorPools) {
		pool->cleanup();
	}

	vkDestroySampler(primaryDevice->getLogicalDevice(), textureSampler, nullptr);
	primaryDevice->cleanup();
	vkDestroySurfaceKHR(instance, vulkanSurface, nullptr);

	std::cout << "\tDestroying Instance" << std::endl;
	vkDestroyInstance(instance, nullptr);
	std::cout << "Vulkan finished cleaning" << std::endl;
}

//Getters
V_CommandPool* V_Instance::getGraphicsCommandPool(int pool_id)
{
	return graphicsCommandPools[pool_id];
}
V_CommandPool * V_Instance::getComputeCommandPool(int pool_id)
{
	return computeCommandPools[pool_id];
}
VkDescriptorSetLayout V_Instance::getPipelineDescriptorSetLayout(material_type pipelineType, desc_set_layout_pos index)
{
	for (V_GraphicsPipeline* pipeline : *graphicsPipelines) {
		if (pipeline->getType() == pipelineType) {
			return pipeline->getDescriptorSetLayout(index);
		}
	}
	return VkDescriptorSetLayout();
}
V_GraphicsPipeline * V_Instance::getGraphicsPipeline(material_type pipeline_type)
{
	for (V_GraphicsPipeline * pipeline : *graphicsPipelines) {
		if (pipeline->getType() == pipeline_type) {
			return pipeline;
		}
	}
	return nullptr;
}