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

VkBool32 VulkanLayerAndExtension::areLayersSupported(vector<const char*>& layerNames)
{
	uint32_t checkCount = layerNames.size();
	uint32_t layerCount = layerPropertyList.size();
	std::vector<const char *> unsupportedLayerNames;

	for (uint32_t i = 0; i < checkCount; i++) {
		VkBool32 isSupported = 0;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (!strcmp(layerNames[i], layerPropertyList[j].properties.layerName)) {
				isSupported = 1;
			}
		}
		if (!isSupported) {
			std::cout << "No layer support found, removed from layer: " << layerNames[i] << std::endl;
			unsupportedLayerNames.push_back(layerNames[i]);
		}
		else {
			std::cout << "Layer supported: " << layerNames[i] << std::endl;
		}
	}

	for (auto i : unsupportedLayerNames) {
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end()) {
			layerNames.erase(it);
		}
	}
	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLayerAndExtension::debugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char *layerPrefix, const char *msg, void *userData) {

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] ERROR: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] WARNING: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] INFORMATION: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] PERFORMANCE: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] DEBUG: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else {
		return VK_FALSE;
	}

	fflush(stdout);
	return VK_TRUE;
}

VkResult VulkanLayerAndExtension::createDebugReportCallback() {
	VkResult result;
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	VkInstance* instance = &appObj->instanceObj.instance;

	//Get vkCreateDebugRepoerCallbackEXT API
	dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugReportCallbackEXT");
	if (!dbgCreateDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgCreateDebugReportCallback function\n";

	// Get vkDestroyDebugReportCallbackEXT API
	dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugReportCallbackEXT");
	if (!dbgDestroyDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function\n";

	// Define the debug report control structure, provide the reference of 'debugFunction'
	// , this function prints the debug information on the console.
	dbgReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	dbgReportCreateInfo.pfnCallback = debugFunction;
	dbgReportCreateInfo.pUserData = NULL;
	dbgReportCreateInfo.pNext = NULL;
	dbgReportCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	// Create the debug report callback and store the handle into 'debugReportCallback'
	result = dbgCreateDebugReportCallback(*instance, &dbgReportCreateInfo, NULL, &debugReportCallback);
	if (result == VK_SUCCESS) {
		std::cout << "Debug report callback object created successfully\n";
	}
	return result;
}

void VulkanLayerAndExtension::destroyDebugReportCallback()
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	VkInstance& instance = appObj->instanceObj.instance;
	dbgDestroyDebugReportCallback(instance, debugReportCallback, NULL);
}

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback = NULL;
	dbgDestroyDebugReportCallback = NULL;
	debugReportCallback = NULL;
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback = NULL;
	dbgDestroyDebugReportCallback = NULL;
	debugReportCallback = NULL;
}
