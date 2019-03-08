#include "MeshManager.h"

void V_MeshManager::setComponent(int index, v_mesh mesh)
{
	meshes->at(getEntityIndex(index)) = mesh;
}

//Destroys vulkan objects in each mesh
void V_MeshManager::cleanup(V_Device* device_in)
{
	device = device_in;
	for (v_mesh mesh : *meshes) {
		vkDestroyBuffer(device->getLogicalDevice(), mesh.indexBuffer, nullptr);
		vkFreeMemory(device->getLogicalDevice(), mesh.iBufferVRAM, nullptr);
		vkDestroyBuffer(device->getLogicalDevice(), mesh.vBuffer, nullptr);
		vkFreeMemory(device->getLogicalDevice(), mesh.vBufferVRAM, nullptr);
	}
}

V_MeshManager::~V_MeshManager()
{
}


GL_MeshManager::~GL_MeshManager()
{
}