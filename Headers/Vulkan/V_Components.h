/*
	Components specific to vulkan
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include "Core/EntityComponents.h"

//Mesh component with all the necessary buffers to render
struct v_mesh {
	VkBuffer vBuffer;
	VkDeviceMemory vBufferVRAM;
	VkBuffer indexBuffer;
	VkDeviceMemory iBufferVRAM;
	uint32_t indicesCount;
};

struct vk_skinned_mesh : skinned_mesh, v_mesh {
	
};

struct vk_gpu_skinned_mesh : v_mesh {
	VkBuffer boneBuffer;
	VkDeviceMemory boneBufVRAM;
};

//Material component containing a type and all the textures with a descriptor for them
struct v_material {
	material_type matType;
	std::vector<VkImage> textures;
	std::vector<VkDeviceMemory> textureMemories;
	std::vector<VkImageView> textureViews;
	VkDescriptorSet descriptorSet = NULL;
};

struct v_camera : camera {
	VkDeviceMemory* camVRAM;
	VkBuffer* camBuffers;
};