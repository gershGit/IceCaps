/*
	Large class wrapping a vulkan instance
	Contains all other vulkan classes to be interfaced with
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include "Vulkan/V_Device.h"
#include "Vulkan/V_Swapchain.h"
#include "Vulkan/V_GraphicsPipeline.h"
#include <optional>

class V_CommandPool;
class V_DescriptorPool;

class V_Instance {
private:
	//Structure holding all configuration info for the instance
	configurationStructure* config;

	//Instance Variable
	VkInstance instance;

	//Debug callback handle
	VkDebugUtilsMessengerEXT callback;

	//Swapchain for the application
	V_Swapchain* swapchain;

	//Devices for this instance and the primary device
	std::vector<V_Device*> devices;
	V_Device* primaryDevice;

	//Graphics pipeline for rendering
	std::vector<V_GraphicsPipeline*> *graphicsPipelines = new std::vector<V_GraphicsPipeline*>();
	//std::vector<V_ComputePipeline*> *computePipelines = new std::vector<V_ComputePipeline*>();

	//Command buffer pools
	std::vector<V_CommandPool*> graphicsCommandPools;
	std::vector<V_CommandPool*> computeCommandPools;
	V_CommandPool* transferCommandPool;
	V_CommandPool* imageOperationsPool;

	//Descriptor pools for carving
	V_DescriptorPool* staticObjectsDescriptorPool;	//All static and dynamic objects known at scene load time
	std::vector<V_DescriptorPool*> descriptorPools; //Dynamically added

	//Surface to associate the vulkan instance with
	VkSurfaceKHR vulkanSurface;

	//Validation layer names
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	//Required extensions for this application (namely that it can use a swapchain)
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};


#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

public:
	//Sampler for textures
	VkSampler textureSampler;

	//Semaphores for rendering and frame information
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0; //Move to game timer?
	bool framebufferResized = false;

	//Constructors
	V_Instance() {};
	V_Instance(configurationStructure * config_in);

	//Initialization functions
	std::vector<const char*> getRequiredExtensions();
	bool setupValidationLayers();
	void enumerateExtensions();
	void setupDebugCallback();
	void initialize(configurationStructure &config);
	void createSurface();
	void attachDevices(unsigned int coreCount);
	void createTransferCommandPools();
	void createSwapChain();
	void createCommandPools(unsigned int cores);
	void createSampler();
	void createSyncObjects();

	//Lifetime functions
	void addGraphicsPipeline(material_type mType, int max_lights);
	void createStaticDescriptorPool(int uniformBuffers, int imageBuffers, int drawableCount);
	void incrementFrame();
	void recreateSwapchain();

	//Cleans up all still living vulkan handles
	~V_Instance();
	void cleanup();

	//Getter functions
	V_Swapchain* getSwapchain() { return swapchain; };
	V_Device* getPrimaryDevice() { return primaryDevice; };
	V_CommandPool* getGraphicsCommandPool(int pool_id);
	V_CommandPool* getComputeCommandPool(int pool_id);
	V_CommandPool* getTransferCommandPool() { if (transferCommandPool != nullptr) { return transferCommandPool; } else { return graphicsCommandPools[0]; } };
	V_CommandPool* getImageOperationsPool() { if (imageOperationsPool != nullptr) { return imageOperationsPool; } else { return graphicsCommandPools[0]; } };
	V_DescriptorPool* getStaticDescriptorPool() { return staticObjectsDescriptorPool; };
	VkDescriptorSetLayout getPipelineDescriptorSetLayout(material_type pipelineType, desc_set_layout_pos index);
	V_GraphicsPipeline* getGraphicsPipeline(material_type pipeline_type);
	std::vector<V_GraphicsPipeline*> * getGraphicsPipelines() { return graphicsPipelines; };
	VkSampler getSampler() {return textureSampler;};
};