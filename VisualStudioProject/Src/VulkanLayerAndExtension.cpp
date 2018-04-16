#include "..\Headers\VulkanLayerAndExtension.h"
#include "..\Headers\VulkanApplication.h"


VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu) {
	VkResult result;

	//Query all the extensions for each layer and store it
	std::cout << "Device extensions" << std::endl;
	std::cout << "======================" << std::endl;
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	std::vector<LayerProperties>* instanceLayerProp = &appObj->GetInstance()->instanceObj.layerExtension.layerPropertyList;
	for (auto globalLayerProp : *instanceLayerProp) {
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp.properties;

		if (result = getExtensionProperties(layerProps, gpu)) {
			continue;
		}

		std::cout << "\n" << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
		layerPropertyList.push_back(layerProps);

		if (layerProps.extensions.size()) {
			for (auto j : layerProps.extensions) {
				std::cout << "\t\t|\n\t\t|---[Device Extension]--> " << j.extensionName << "\n";
			}
		}
		else {
			std::cout << "\t\t|\n\t\t|---[Device Extension]--> No extension found \n";
		}
	}
	return result;
}

VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties & layerProps, VkPhysicalDevice * gpu)
{
	// Stores number of extension per layer
	uint32_t extensionCount;
	VkResult result;
	// Name of the layer
	char* layerName = layerProps.properties.layerName;
	do {
		//Get the total number of extensions in this layer
		if (gpu) {
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, NULL);
		}
		else {
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, NULL);
		}
		if (result || extensionCount == 0) {
			continue;
		}
		layerProps.extensions.resize(extensionCount);
		//Gather all extension properties
		if (gpu) {
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
		}
		else {
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
		}
	} while (result == VK_INCOMPLETE);
	return VkResult();
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	//Store the number of instance layers
	uint32_t instanceLayerCount;

	//Vector to store layer properties
	vector<VkLayerProperties> layerProperties;

	//Check Vulkan API result status
	VkResult result;

	//Actually Query all the layers
	do {
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);
		if (result) {
			return result;
		} 
		if (instanceLayerCount == 0) {
			return VK_INCOMPLETE;
		}
		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	//Query all the extensions for each layer and store it
	std::cout << "\nInstanced Layers" << std::endl;
	std::cout << "===================" << std::endl;
	for (auto globalLayerProp : layerProperties) {
		// Print layer name and its description
		std::cout << "\n" << globalLayerProp.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.layerName << "\n";
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp;
		//Get instance level extensions for corresponding layer properties
		result = getExtensionProperties(layerProps);
		if (result) {
			continue;
		}

		layerPropertyList.push_back(layerProps);

		//Print extension name for each instance layer
		for (auto j : layerProps.extensions) {
			std::cout << "\t\t|\n\t\t|---[Layer Extension]--> " << j.extensionName << "\n";
		}
	}
	return result;
}

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
}
