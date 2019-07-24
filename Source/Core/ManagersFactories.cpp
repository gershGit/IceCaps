#include "Core/ManagersFactories.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

int countImages(MappedManager<v_material>* vulkanMaterialManager)
{
	int count = 0;
	for (std::pair<int, v_material> material : vulkanMaterialManager->componentMap) {
		if (material.second.matType == PBR) {
			count += 5;
		}
	}
	return count;
}

void cleanup(MappedManager<v_material>* manager)
{

}

void managerCleanup(MappedManager<v_material>* manager, V_Device * device) {
	for (std::pair<int, v_material> material : manager->componentMap) {
		for (VkImageView view : material.second.textureViews) {
			vkDestroyImageView(device->getLogicalDevice(), view, nullptr);
		}
		for (VkImage image : material.second.textures) {
			vkDestroyImage(device->getLogicalDevice(), image, nullptr);
		}
		for (VkDeviceMemory mem : material.second.textureMemories) {
			vkFreeMemory(device->getLogicalDevice(), mem, nullptr);
		}
	}
}

void managerCleanup(MappedManager<v_mesh>* manager, V_Device * device) {
	for (std::pair<int, v_mesh> mesh : manager->componentMap) {
		vkDestroyBuffer(device->getLogicalDevice(), mesh.second.indexBuffer, nullptr);
		vkFreeMemory(device->getLogicalDevice(), mesh.second.iBufferVRAM, nullptr);
		vkDestroyBuffer(device->getLogicalDevice(), mesh.second.vBuffer, nullptr);
		vkFreeMemory(device->getLogicalDevice(), mesh.second.vBufferVRAM, nullptr);
	}
}

void managerCleanup(MappedManager<v_camera>* manager, V_Device* device, int bufferCount) {
	for (std::pair<int, v_camera> cam : manager->componentMap) {
		for (int i = 0; i < bufferCount; i++) {
			vkDestroyBuffer(device->getLogicalDevice(), cam.second.camBuffers[i], nullptr);
			vkFreeMemory(device->getLogicalDevice(), cam.second.camVRAM[i], nullptr);
		}
	}
}