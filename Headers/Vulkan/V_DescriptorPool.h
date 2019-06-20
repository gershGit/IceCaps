/*
	A wrapper class for a vulkan descriptor pool
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include "Vulkan/V_Device.h"
#include "Vulkan/V_Swapchain.h"
#include "Vulkan/V_Components.h"
#include "Vulkan/V_GraphicsPipeline.h"

class V_DescriptorPool
{
private:
	//Reference to the configuration being used
	configurationStructure *config;

	//References to vulkan classes and wrappes
	V_Device* device;
	V_Swapchain* swapchain;
	VkDescriptorPool pool;

public:
	V_DescriptorPool();
	
	void initialize(int uniformBuffers, int imageBuffers, int drawableCount);

	void allocateSetsInMap(VkDescriptorSetLayout layout, NodeManager<VulkanSceneNode>* scene_nodeMap, int setsCount);

	//Allocation functions to carve sets out of the pool

	//Configures information about sets
	void configureNodeSets(NodeManager<VulkanSceneNode>* scene_nodeMap, VkDescriptorBufferInfo * viewPerspInfos, int max_lights, int frames);
	void allocateTextureSet(VkDescriptorSetLayout layout, v_material * material);

	void configureTextureSet(v_material * material, VkSampler & sampler);

	//Getters and setters
	void setConfig(configurationStructure * config_in) { config = config_in; };
	void setDevice(V_Device* device_in);
	void setSwapchain(V_Swapchain* swapchain_in);
	VkDescriptorPool getDescriptorPool() { return pool; };

	//Cleans up the pool
	void cleanup();

	~V_DescriptorPool();
};

