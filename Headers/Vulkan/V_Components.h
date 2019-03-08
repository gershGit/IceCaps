/*
	Components specific to vulkan
*/

#pragma once
#include "Vulkan_Headers.h"
#include "EntityComponents.h"

//Mesh component with all the necessary buffers to render
struct v_mesh {
	VkBuffer vBuffer;
	VkDeviceMemory vBufferVRAM;
	VkBuffer indexBuffer;
	VkDeviceMemory iBufferVRAM;
	uint32_t indicesCount;
};

struct v_skinned_mesh : skinned_mesh, v_mesh {
	
};

//Material component containing a type and all the textures with a descriptor for them
struct v_material {
	material_type matType;
	std::vector<VkImage> textures;
	std::vector<VkDeviceMemory> textureMemories;
	std::vector<VkImageView> textureViews;
	VkDescriptorSet descriptorSet;
};