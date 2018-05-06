#pragma once
#include "../Headers/Headers.h"

// Shader class managing the shader conversion, compilation, linking
class VulkanShader
{
public:
	// Vk structure storing vertex & fragment shader information
	VkPipelineShaderStageCreateInfo shaderStages[2];

	// Constructor
	VulkanShader() {}

	// Destructor
	~VulkanShader() {}

	// Use .spv and build shader module
	void buildShaderModuleWithSPV(uint32_t *vertShaderText, size_t vertexSPVSize, uint32_t *fragShaderText, size_t fragmentSPVSize);

	// Kill the shader when not required
	void destroyShaders();
};