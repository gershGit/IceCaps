#include "V_Pipeline.h"
#include <fstream>


V_Pipeline::V_Pipeline()
{
}

//Reads a shader to a vector of characters
std::vector<char> V_Pipeline::readShader(const std::string & filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open shader file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

//Creates a module from a logical device and shader code
VkShaderModule V_Pipeline::createShaderModule(const std::vector<char>& code, VkDevice logicalDevice)
{
	VkShaderModuleCreateInfo shaderModuleInfo = {};
	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = code.size();
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	VkResult res = vkCreateShaderModule(logicalDevice, &shaderModuleInfo, nullptr, &shaderModule);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	return shaderModule;
}

V_Pipeline::~V_Pipeline()
{
}