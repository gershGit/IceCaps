/*
	Class to build a vulkan mesh component
*/

#pragma once
#include "V_Components.h"
#include "V_Instance.h"
#include "V_CommandPool.h"


class V_MeshFactory
{
public:
	//Static builder functions
	static void buildVertexBuffer(v_mesh & mesh, std::vector<vertex> &vertices, V_CommandPool * transferPool, V_Device * device);
	static void buildIndexBuffer(v_mesh &mesh, std::vector<uint16_t> &indices, V_CommandPool* transferPool, V_Device * device);
	static void loadFromFile(const char * fileName, v_mesh & mesh, configurationStructure &config);

	V_MeshFactory();
	~V_MeshFactory();
};

