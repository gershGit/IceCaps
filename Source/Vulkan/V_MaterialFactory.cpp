#include "V_MaterialFactory.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "V_BufferHelper.h"
#include "V_Instance.h"
#include "StringTranslation.h"
#include <fstream>

//Sets v_material type to correct value based on string
void setMaterialType(v_material &material, std::string typeAsString) {
	if (strcmp(typeAsString.c_str(), "PBR") == 0) {
		material.matType = PBR;
		material.textures.resize(5);
		material.textureMemories.resize(5);
		material.textureViews.resize(5);
	}
}

//Creates the view for a specified texture type
void createTextureView(v_material &material, texture_type textureType, configurationStructure &config)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = material.textures[textureType];
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkResult res = vkCreateImageView(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), &viewInfo, nullptr, &material.textureViews[textureType]);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

//Adds a texture to the passed material
void V_MaterialFactory::loadTextureFromFile(std::string textureFile, v_material &material, texture_type textureType, configurationStructure &config) {
	std::cout << "\t\tAdding texture at " << textureFile << std::endl;

	//Read texture to array
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(textureFile.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	//Create a staging buffer for the texture
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	V_BufferHelper::createBuffer(config.apiInfo.v_Instance->getPrimaryDevice(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

	//Copy data into the staging buffer
	void* data;
	vkMapMemory(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), stagingMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), stagingMemory);

	//Free the data from the system RAM
	stbi_image_free(pixels);

	//Create an image on the GPU
	V_BufferHelper::createImage(config.apiInfo.v_Instance->getPrimaryDevice(), texWidth, texHeight, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, material.textures[textureType], material.textureMemories[textureType]);

	//Copy the image over from the staging buffer
	V_BufferHelper::transitionImageLayout(config.apiInfo.v_Instance->getImageOperationsPool(), material.textures[textureType], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	V_BufferHelper::copyBufferToImage(config.apiInfo.v_Instance->getImageOperationsPool(), stagingBuffer, material.textures[textureType], static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	V_BufferHelper::transitionImageLayout(config.apiInfo.v_Instance->getImageOperationsPool(), material.textures[textureType], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	//Cleanup of staging buffer
	vkDestroyBuffer(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), stagingMemory, nullptr);

	createTextureView(material, textureType, config);
}

//Loads a material from a file with calls to load texture or create texture
void V_MaterialFactory::loadMaterialFromFile(std::string fileName, v_material & material, configurationStructure & config)
{
	std::cout << "\tLoading Material from file: " << fileName << std::endl;
	std::string line, key, value;
	std::ifstream fileStream(fileName.c_str());
	while (std::getline(fileStream, line)) {
		key = getSubComponent(line);
		value = getValue(line);
		if (strcmp(key.c_str(), "TYPE") == 0) {
			setMaterialType(material, value);
		}
		else if (strcmp(key.c_str(), "TEXTURE") == 0) {
			texture_type tType = getTextureType(value);
			std::getline(fileStream, line);
			if (strcmp(getSubComponent(line).c_str(), "FILE_LOAD") == 0) {
				std::string textureFile = config.gamePath;
				textureFile.append(getValue(line));
				loadTextureFromFile(textureFile, material, tType, config);
			}
		}
	}
}

V_MaterialFactory::V_MaterialFactory()
{
}
V_MaterialFactory::~V_MaterialFactory()
{
}