#include "MaterialManager.h"
#include "V_Device.h"

//Loops through materials and returns the total number of images in all
int V_MaterialManager::getImageCount()
{
	int count = 0;
	for (v_material material : *materials) {
		if (material.matType == PBR) {
			count += 5;
		}
	}
	return count;
}

//Destroys vulkan objects in each material
void V_MaterialManager::cleanup(V_Device * device_in)
{
	for (v_material material : *materials) {
		for (VkImageView view : material.textureViews) {
			vkDestroyImageView(device_in->getLogicalDevice(), view, nullptr);
		}
		for (VkImage image : material.textures) {
			vkDestroyImage(device_in->getLogicalDevice(), image, nullptr);
		}
		for (VkDeviceMemory mem : material.textureMemories) {
			vkFreeMemory(device_in->getLogicalDevice(), mem, nullptr);
		}
	}
}

V_MaterialManager::~V_MaterialManager()
{
}