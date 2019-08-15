/*
	Class to build a vulkan mesh component
*/

#pragma once
#include "Vulkan/V_Components.h"
#include "Vulkan/V_Instance.h"
#include "Vulkan/V_CommandPool.h"


class V_MeshFactory
{
public:
	//Static builder functions
	static void buildVertexBuffer(v_mesh & mesh, std::vector<vertex> &vertices, V_CommandPool * transferPool, V_Device * device);
	static void buildSkinnedVertexBuffer(vk_skinned_mesh& mesh, std::vector<skinned_vertex>& vertices, V_CommandPool* transferPool, V_Device* device);
	static void buildIndexBuffer(v_mesh &mesh, std::vector<uint16_t> &indices, V_CommandPool* transferPool, V_Device * device);
	static void buildIndexBuffer(vk_skinned_mesh& mesh, std::vector<uint16_t>& indices, V_CommandPool* transferPool, V_Device* device);
	static void buildSkinnedIndexBuffer(vk_skinned_mesh& mesh, std::vector<uint16_t>& indices, V_CommandPool* transferPool, V_Device* device);
	static void loadFromFile(const char * fileName, v_mesh & mesh, configurationStructure &config, AABB * bounds);
	static void loadSkinnedMeshFromFile(const char* fileName, vk_skinned_mesh& mesh, configurationStructure& config, AABB* bounds, bool onGPU);

	V_MeshFactory();
	~V_MeshFactory();
};

