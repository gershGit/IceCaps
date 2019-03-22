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

glm::mat4 getTransformationMatrix(transform transform_component)
{
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), transform_component.scale);
	glm::mat4 rotMat = glm::toMat4(glm::quat(transform_component.rot));
	glm::mat4 transMat = glm::translate(glm::mat4(1), transform_component.pos);

	return transMat * rotMat * scaleMat;
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