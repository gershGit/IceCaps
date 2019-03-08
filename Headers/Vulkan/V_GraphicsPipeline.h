/*
	Subclass of the Pipeline class
	Holds information about a graphics pipeline in vulkan
	This should be used in conjunction with each material type (PBR material type for example will have its own pipeline)
*/

#pragma once
#include "V_Pipeline.h"
#include "V_Swapchain.h"
#include "Vulkan_Headers.h"

class V_GraphicsPipeline :
	public V_Pipeline
{
private:
	//Pipeline / material type
	material_type type;

	//References to other vulkan classes
	V_Swapchain* swapchain;
	std::vector<VkFramebuffer> frameBuffers;

	//Vulkan sub objects
	VkRenderPass renderPass;

	//Binding and attribute descriptions
	VkVertexInputBindingDescription bindingDescription = {};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};

public:
	//Light and camera buffers bound to the descriptor sets
	int max_lights;
	std::vector<VkBuffer> lightViewBuffers;
	std::vector<VkDeviceMemory> lightViewBufferVRAMs;
	std::vector<VkDescriptorSet> lightViewDescriptorSets = std::vector<VkDescriptorSet>();

	//Constructor, requires SPIR-V vertex and frag shaders
	V_GraphicsPipeline(V_Device * device_in, V_Swapchain * swapchain_in);

	//Creation of other vulkan objects
	void createRenderPass();
	void createFrameBuffers();
	void createDescriptorSetLayout(material_type m_type);
	void createBindingDescription(material_type m_type);

	//Initialization functions
	void initialize(const std::string & vertex_shader, const std::string & frag_shader);
	void initialize(material_type mType);
	void buildLightCameraBuffers(int max_lights_in, int swapchainBuffering);

	//Resets the graphics pipeline when the swapchain is reset
	void swapchainReset();

	//Getter functions
	VkRenderPass getRenderPass() { return renderPass; };
	material_type getType() { return type; };
	std::vector<VkDescriptorSet> *getDescriptorSets() { return &lightViewDescriptorSets; };
	std::vector<VkFramebuffer>* getFramebuffers() { return &frameBuffers; };

	//Cleanup functions
	void cleanupShaderModules(VkShaderModule vertShader, VkShaderModule fragShader);
	void cleanup();
	~V_GraphicsPipeline();
};

