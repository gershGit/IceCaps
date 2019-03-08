#include "Vulkan/V_DescriptorPool.h"
#include <array>


V_DescriptorPool::V_DescriptorPool()
{
	//std::cout << "???" << std::endl;
}

//Initializes a descriptor pool based on the buffers needed
void V_DescriptorPool::initialize(int uniformBuffers, int imageBuffers, int drawableCount)
{
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(uniformBuffers);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(imageBuffers);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(drawableCount);

	VkResult res = vkCreateDescriptorPool(device->getLogicalDevice(), &poolInfo, nullptr, &pool);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

//Allocates sets from the pool to a setComponent using a pipeline's set layout
void V_DescriptorPool::allocateSets(VkDescriptorSetLayout layout, std::vector<VkDescriptorSet> &descriptorSets, int setsCount)
{
	VkDescriptorSetLayout* layouts;
	layouts = (VkDescriptorSetLayout*)malloc(sizeof(VkDescriptorSetLayout) * setsCount);
	for (int i = 0; i < setsCount; i++) {
		layouts[i] = layout;
	}

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(setsCount);
	allocInfo.pSetLayouts = layouts;

	descriptorSets.resize(setsCount);
	VkResult res = vkAllocateDescriptorSets(device->getLogicalDevice(), &allocInfo, descriptorSets.data());
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

//Configures descriptor sets to use 1 buffer split into 2 bindings
void V_DescriptorPool::configureVulkanPipelineDescriptorSets(V_GraphicsPipeline * pipeline)
{
	for (uint32_t i = 0; i < pipeline->lightViewDescriptorSets.size(); i++) {
		VkDescriptorBufferInfo viewPerspBufferInfo = {};
		viewPerspBufferInfo.buffer = pipeline->lightViewBuffers[i];
		viewPerspBufferInfo.offset = 0;
		viewPerspBufferInfo.range = sizeof(ViewPersp);

		VkDescriptorBufferInfo lightBufferInfo = {};
		lightBufferInfo.buffer = pipeline->lightViewBuffers[i];
		lightBufferInfo.offset = sizeof(ViewPersp);
		lightBufferInfo.range = sizeof(LightObject) * pipeline->max_lights;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = pipeline->lightViewDescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &viewPerspBufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = pipeline->lightViewDescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &lightBufferInfo;

		vkUpdateDescriptorSets(device->getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

//Allocates a descriptor set for textures based on a material layout and vulkan material reference
void V_DescriptorPool::allocateTextureSet(VkDescriptorSetLayout layout, v_material &material) {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkResult res = vkAllocateDescriptorSets(device->getLogicalDevice(), &allocInfo, &material.descriptorSet);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

//Configures a descriptor set to bind to the texture views using a sampler
void V_DescriptorPool::configureTextureSet(v_material & material, VkSampler &sampler)
{
	std::vector<VkDescriptorImageInfo> imageInfos = std::vector<VkDescriptorImageInfo>(material.textures.size());
	for (unsigned int i = 0; i < material.textures.size(); i++) {
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = material.textureViews[i];
		imageInfo.sampler = sampler;
		imageInfos[i] = imageInfo;
	}

	VkWriteDescriptorSet descriptorWrite = {};

	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = material.descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount =static_cast<uint32_t>( material.textures.size() );
	descriptorWrite.pImageInfo = imageInfos.data();

	vkUpdateDescriptorSets(device->getLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
}

//Setters
void V_DescriptorPool::setDevice(V_Device * device_in)
{
	device = device_in;
}
void V_DescriptorPool::setSwapchain(V_Swapchain * swapchain_in)
{
	swapchain = swapchain_in;
}

//Cleans up a descriptor pool
void V_DescriptorPool::cleanup()
{
	vkDestroyDescriptorPool(device->getLogicalDevice(), pool, nullptr);
}

V_DescriptorPool::~V_DescriptorPool()
{
}