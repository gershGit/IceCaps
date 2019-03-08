/*
	Parent class for all vulkan pipelines, comput and graphics
*/

#pragma once
#include "Vulkan_Headers.h"
#include "V_Device.h"
#include <vector>

//Enumerations for selecting the descriptor layout set
enum desc_set_layout_pos { LIGHT_CAM_SET, MATERIAL_SET };

class V_Pipeline
{
protected:
	//Configuration file
	configurationStructure* config;

	//Vulkan objects
	VkPipeline pipeline;
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
	VkPipelineLayout pipelineLayout;

	//Reference to a device used for creation of vulkan objects
	V_Device* device;

public:
	//Constructor
	V_Pipeline();

	//Read and creates shaders from files
	static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice logicalDevice);
	static std::vector<char> readShader(const std::string& filename);
	
	//Setterse and getters
	void setDescriptorSetLayout(VkDescriptorSetLayout layout_in, desc_set_layout_pos index) { descriptorSetLayouts[index] = layout_in; };
	void setConfig(configurationStructure *config_in) { config = config_in; };
	VkDescriptorSetLayout getDescriptorSetLayout(desc_set_layout_pos layout_type) { return descriptorSetLayouts[layout_type]; };
	VkPipeline getPipeline() { return pipeline; };
	VkPipelineLayout getPipelineLayout() { return pipelineLayout; };
	~V_Pipeline();
};

