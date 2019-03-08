#include "V_Device.h"
#include <optional>
#include <set>

V_Device::V_Device()
{
}

//Creates the logical device
void V_Device::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies;
	if (indices.computeFamily.has_value()) {
		uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value(), indices.computeFamily.value() };
	}
	else {
		uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamiles.data());
	printQueueFamilies(queueFamiles, physicalDevice, surface);

	float queuePriority[] = { 1.0f, 1.0f, 1.0f };
	if (!indices.computeFamily.has_value()) {
		if (indices.graphicsFamily.value() == indices.presentFamily.value()) {
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
			if (unsigned int(indices.graphicsFamilyQueues) < config->cpu_info->coreCount) {
				queueInfo.queueCount = indices.graphicsFamilyQueues;
				graphicsQueues.resize(indices.graphicsFamilyQueues);
			}
			else {
				queueInfo.queueCount = config->cpu_info->coreCount;
				graphicsQueues.resize(config->cpu_info->coreCount);
			}
			queueInfo.pQueuePriorities = &queuePriority[0];
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else if (indices.graphicsFamily.value() != indices.computeFamily.value()) {
		if (indices.graphicsFamily.value() == indices.presentFamily.value()) {
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
			if (unsigned int (indices.graphicsFamilyQueues) < config->cpu_info->coreCount) {
				queueInfo.queueCount = indices.graphicsFamilyQueues;
				graphicsQueues.resize(indices.graphicsFamilyQueues);
			}
			else {
				queueInfo.queueCount = config->cpu_info->coreCount;
				graphicsQueues.resize(config->cpu_info->coreCount);
			}
			queueInfo.pQueuePriorities = &queuePriority[0];
			queueCreateInfos.push_back(queueInfo);
		}
		
		//Compute queues creation
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = indices.computeFamily.value();
		if (unsigned int (indices.computeFamilyQueues) < config->cpu_info->coreCount) {
			queueInfo.queueCount = indices.computeFamilyQueues;
			computeQueues.resize(indices.computeFamilyQueues);
		}
		else {
			queueInfo.queueCount = config->cpu_info->coreCount;
			computeQueues.resize(config->cpu_info->coreCount);
		}
		queueInfo.pQueuePriorities = queuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	else if (indices.computeFamily.value() == indices.graphicsFamily.value()) {
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
		if (unsigned int(indices.graphicsFamilyQueues) < config->cpu_info->coreCount) {
			queueInfo.queueCount = indices.graphicsFamilyQueues;
			graphicsQueues.resize(indices.graphicsFamilyQueues);
			computeQueues.resize(indices.computeFamilyQueues);
		}
		else {
			queueInfo.queueCount = config->cpu_info->coreCount;
			graphicsQueues.resize(config->cpu_info->coreCount);
			computeQueues.resize(config->cpu_info->coreCount);
		}
		queueInfo.pQueuePriorities = &queuePriority[0];
		queueCreateInfos.push_back(queueInfo);
	}

	if (indices.transferFamily.has_value() && ! (indices.transferFamily.value() == indices.graphicsFamily.value() || indices.transferFamily.value() == indices.computeFamily.value())) {
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = indices.transferFamily.value();
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority[0];
		queueCreateInfos.push_back(queueInfo);
	}

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.pEnabledFeatures = &config->featuresRequired;
	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	deviceInfo.ppEnabledExtensionNames = extensions.data();

	if (config->debugOn) {
		deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		deviceInfo.enabledLayerCount = 0;
	}
	VkResult result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &logicalDevice);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create a logical device from valid physical device");
	}
	std::cout << "Logical device created" << std::endl;

	//Queue creation
	std::cout << "Getting as many queues as possible..." << std::endl;
	for (int i = 0; i < graphicsQueues.size(); i++) {
		vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), i, &graphicsQueues[i]);
	}
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &imageOperationsQueue);
	for (int i = 0; i < computeQueues.size(); i++) {
		vkGetDeviceQueue(logicalDevice, indices.computeFamily.value(), i, &computeQueues[i]);
	}
	if (indices.transferFamily.has_value()) {
		vkGetDeviceQueue(logicalDevice, indices.transferFamily.value(), 0, &transferQueue);
	}
	std::cout << "Queues on device created" << std::endl;
}
void V_Device::initialize(VkPhysicalDevice physicalDeviceIn, VkSurfaceKHR surfaceIn, configurationStructure* config_in)
{
	config = config_in;
	physicalDevice = physicalDeviceIn;
	surface = surfaceIn;
	createLogicalDevice();
	for (auto queue : graphicsQueues) {
		std::cout << "\tGraphics queue index:\t " << queue << std::endl;
	}
	if (presentQueue != NULL) {
		std::cout << "\tPresent queue index:\t " << presentQueue << std::endl;
	}
	if (presentQueue != NULL) {
		std::cout << "\tImage Operations queue index:\t " << imageOperationsQueue << std::endl;
	}
	for (auto queue : computeQueues) {
		std::cout << "\tCompute queue index:\t " << queue << std::endl;
	}
}

//Getters and setters
VkPhysicalDevice V_Device::getPhysicalDevice()
{
	return physicalDevice;
}
VkDevice V_Device::getLogicalDevice()
{
	return logicalDevice;
}
VkQueue V_Device::getGraphicsQueue()
{
	return graphicsQueues[0];
}
VkQueue V_Device::getGraphicsQueue(int coreIndex)
{
	return graphicsQueues[coreIndex % graphicsQueues.size()];
}
VkQueue V_Device::getImageOperationsQueue()
{
	return imageOperationsQueue;
}
VkQueue V_Device::getPresentQueue()
{
	return presentQueue;
}
VkQueue V_Device::getTransferQueue()
{
	return transferQueue;
}
VkQueue V_Device::getComputeQueueWithIndex(int index)
{
	return computeQueues[index];
}
VkQueue V_Device::getGraphicsQueueWithIndex(int index)
{
	return graphicsQueues[index];
}
void V_Device::setPhysicalDevice(VkPhysicalDevice physicalDeviceIn) {
	physicalDevice = physicalDeviceIn;
}
void V_Device::setSurface(VkSurfaceKHR surfaceIn)
{
	surface = surfaceIn;
}

V_Device::~V_Device()
{
}

//Cleans up the device
void V_Device::cleanup()
{
	std::cout << "\tDestroying Logical Device" << std::endl;
	vkDestroyDevice(logicalDevice, nullptr);
}