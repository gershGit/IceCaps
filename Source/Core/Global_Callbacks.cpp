#pragma once
#include "Core/Global_Callbacks.h"
#include "Vulkan/V_GraphicsPipeline.h"
#include <algorithm>

//Prints the queue families available on a physical device
void printQueueFamilies(std::vector<VkQueueFamilyProperties> queueFamiles, VkPhysicalDevice device, VkSurfaceKHR surface) {
	std::cout << queueFamiles.size() << " queue families found on device" << std::endl;
	int i = 0;
	for (VkQueueFamilyProperties queueFamily : queueFamiles) {
		std::cout << "\tQueue Family: " << i << std::endl;
		std::cout << "\t\tContains " << queueFamily.queueCount << " Queues" << std::endl;
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			std::cout << "\t\tSUPPORTS GRAPHICS" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			std::cout << "\t\tSUPPORTS COMPUTE" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			std::cout << "\t\tSUPPORTS TRANSFER" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
			std::cout << "\t\tSUPPORTS SPARSE BINDING" << std::endl;
		}
		if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT) {
			std::cout << "\t\tSUPPORTS PROTECTED" << std::endl;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			std::cout << "\t\tSUPPORTS PRESENT" << std::endl;
		}
		i++;
	}
}

//Finds the queue families on a graphics card and returns the index of the family supporting graphics
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());
	
	//Attempt separate graphics and compute queue families
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			indices.graphicsFamilyQueues = queueFamily.queueCount;
		}
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			if (i != indices.graphicsFamily) {
				indices.computeFamily = i;
				indices.computeFamilyQueues = queueFamily.queueCount;
			}
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (!indices.presentFamily.has_value() && queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isCompleteWithCompute()) {
			break;
		}
		i++;
	}

	//Attempt to find separate transfer queue family
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (!indices.graphicsFamily.has_value() && !indices.computeFamily.has_value()) {
				indices.transferFamily = i;
			}
			else if (!indices.graphicsFamily.has_value() && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
			else if (!indices.computeFamily.has_value() && (i != indices.graphicsFamily)) {
				indices.transferFamily = i;
			}
			else if ((i != indices.graphicsFamily) && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
		}
		i++;
	}
	if (indices.transferFamily.has_value()) {
		return indices;
	}

	//Settle for a family being used for graphics or compute as well
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			indices.transferFamily = i;
		}
		i++;
	}

	if (!indices.computeFamily.has_value() && indices.graphicsFamily.has_value()) {
		if (queueFamiles[indices.graphicsFamily.value()].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			indices.computeFamily = indices.graphicsFamily.value();
			indices.computeFamilyQueues = indices.graphicsFamilyQueues;
		}
	}

	return indices;
}
QueueFamilyIndices findTransferFamily(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());

	//Attempt separate graphics and compute queue families
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			if (!indices.graphicsFamily.has_value() || (indices.graphicsFamily.has_value() && i != indices.graphicsFamily)) {
				indices.computeFamily = i;
			}
		}
		i++;
	}

	//Attempt to find separate transfer queue family
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (!indices.graphicsFamily.has_value() && !indices.computeFamily.has_value()) {
				indices.transferFamily = i;
			}
			else if (!indices.graphicsFamily.has_value() && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
			else if (!indices.computeFamily.has_value() && (i != indices.graphicsFamily)) {
				indices.transferFamily = i;
			}
			else if ((i != indices.graphicsFamily) && (i != indices.computeFamily)) {
				indices.transferFamily = i;
			}
		}
		i++;
	}
	if (indices.transferFamily.has_value()) {
		return indices;
	}

	//Settle for a family being used for graphics or compute as well
	i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			indices.transferFamily = i;
		}
		i++;
	}

	return indices;
}

//Returns a structure of the details on what is supported for the swapchain
SwapchainSupportDetails querySwapchainSupport(VkSurfaceKHR surface, VkPhysicalDevice device) {
	SwapchainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

//Creates the messenfer for debugs
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

//Destroys the messenger for debugs
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

//Finds a memory type
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

//Functions to translate between sampling information
int getMaxUsableSampleCount(VkPhysicalDevice physicalDevice) {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return 64; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return 32; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return 16; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return 8; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return 4; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return 2; }

	return 1;
}
VkSampleCountFlagBits getMaxUsableSampleCountFlags(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
VkSampleCountFlagBits intToSampleFlagBits(int bits) {
	if (bits == 64) { return VK_SAMPLE_COUNT_64_BIT; }
	if (bits == 32) { return VK_SAMPLE_COUNT_32_BIT; }
	if (bits == 16) { return VK_SAMPLE_COUNT_16_BIT; }
	if (bits == 8) { return VK_SAMPLE_COUNT_8_BIT; }
	if (bits == 4) { return VK_SAMPLE_COUNT_4_BIT; }
	if (bits == 2) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}

//Frustum culling calculations
glm::vec3 worldAxis[3] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1)
};
glm::vec2 project(AABB bounds, glm::vec3 axis) {
	float min = glm::dot(bounds.points[0], axis);
	float max = min;
	float val;
	for (int i = 1; i < 8; i++) {
		val = glm::dot(bounds.points[i], axis);
		if (val < min) {
			min = val;
		}
		else if (val > max) {
			max = val;
		}
	}
	return glm::vec2(min, max);
}
glm::vec2 project(frustum * frus, glm::vec3 axis) {
	float min = glm::dot(frus->points[0], axis);
	float max = min;
	float val;
	for (int i = 1; i < 8; i++) {
		val = glm::dot(frus->points[i], axis);
		if (val < min) {
			min = val;
		}
		else if (val > max) {
			max = val;
		}
	}
	return glm::vec2(min, max);
}
bool overlap(glm::vec2 a, glm::vec2 b) {
	return (a.y >= b.x && b.y >= a.x);
}
bool fullOverlap(glm::vec2 outer, glm::vec2 inner) {
	return outer.x <= inner.x && outer.y >= inner.y;
}

void initializeABBB(AABB * bounds)
{
	bounds->size = glm::vec3(0);
	bounds->pos = glm::vec3(0);
	/*
	   5-------7
	  /|      /|
	 / |     / |
	1--|----3  |
	|  4----|--6
	| /     | /
	0-------2
	*/
	bounds->points[0] = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
	bounds->points[1] = glm::vec3(-INFINITY, INFINITY, -INFINITY);
	bounds->points[2] = glm::vec3(INFINITY, -INFINITY, -INFINITY);
	bounds->points[3] = glm::vec3(INFINITY, INFINITY, -INFINITY);
	bounds->points[4] = glm::vec3(-INFINITY, -INFINITY, INFINITY);
	bounds->points[5] = glm::vec3(-INFINITY, INFINITY, INFINITY);
	bounds->points[6] = glm::vec3(INFINITY, -INFINITY, INFINITY);
	bounds->points[7] = glm::vec3(INFINITY, INFINITY, INFINITY);
}

//Compares two bounding boxes to see if an area completely encloses an object
bool isInside(AABB * area, AABB * object)
{
	if (fullOverlap(glm::vec2(area->points[0].x, area->points[7].x), glm::vec2(object->points[0].x, object->points[7].x))
		&& fullOverlap(glm::vec2(area->points[0].x, area->points[7].x), glm::vec2(object->points[0].x, object->points[7].x))
		&& fullOverlap(glm::vec2(area->points[0].x, area->points[7].x), glm::vec2(object->points[0].x, object->points[7].x))) {
		return true;
	}
	return false;
}
float getMin(AABB*a, AABB*b, int axis) {
	float min = INFINITY;
	for (int i = 0; i < 8; i++) {
		if (a->points[i][axis] < min) {
			min = a->points[i][axis];
		}
		if (b->points[i][axis] < min) {
			min = b->points[i][axis];
		}
	}
	return min;
}
float getMax(AABB*a, AABB*b, int axis) {
	float max = -INFINITY;
	for (int i = 0; i < 8; i++) {
		if (a->points[i][axis] > max) {
			max = a->points[i][axis];
		}
		if (b->points[i][axis] > max) {
			max = b->points[i][axis];
		}
	}
	return max;
}
AABB getMaxBounds(AABB * a, AABB * b)
{
	float minX= getMin(a, b, 0);
	float minY = getMin(a, b, 1);
	float minZ = getMin(a, b, 2);
	float maxX = getMax(a, b, 0);
	float maxY = getMax(a, b, 1);
	float maxZ = getMax(a, b, 2);
	AABB newBounds = AABB();
	newBounds.points[0] = glm::vec3(minX, minY, minZ);
	newBounds.points[1] = glm::vec3(minX, maxY, minZ);
	newBounds.points[2] = glm::vec3(maxX, minY, minZ);
	newBounds.points[3] = glm::vec3(maxX, maxY, minZ);
	newBounds.points[4] = glm::vec3(minX, minY, maxZ);
	newBounds.points[5] = glm::vec3(minX, maxY, maxZ);
	newBounds.points[6] = glm::vec3(maxX, minY, maxZ);
	newBounds.points[7] = glm::vec3(maxX, maxY, maxZ);
	return newBounds;
}
AABB getBounds(collider & col, glm::vec3 pos)
{
	AABB bounds = AABB();
	if (col.type == SPHERE_COLLIDER) {
		bounds.pos = pos;
		bounds.size.x = col.radius;
		bounds.size.y = col.radius;
		bounds.size.z = col.radius;
		bounds.points[0] = pos + glm::vec3(-col.radius, -col.radius, -col.radius);
		bounds.points[1] = pos + glm::vec3(-col.radius, col.radius, -col.radius);
		bounds.points[2] = pos + glm::vec3(col.radius, -col.radius, -col.radius);
		bounds.points[3] = pos + glm::vec3(col.radius, col.radius, -col.radius);
		bounds.points[4] = pos + glm::vec3(-col.radius, -col.radius, col.radius);
		bounds.points[5] = pos + glm::vec3(-col.radius, col.radius, col.radius);
		bounds.points[6] = pos + glm::vec3(col.radius, -col.radius, col.radius);
		bounds.points[7] = pos + glm::vec3(col.radius, col.radius, col.radius);
	}
	return bounds;
}
AABB getMeshBounds(AABB * bounds_in, glm::vec3 pos)
{
	AABB bounds = AABB();
	bounds.pos = pos;
	bounds.size = bounds_in->size;
	bounds.points[0] = pos + glm::vec3(-bounds.size.x, -bounds.size.y, -bounds.size.z);
	bounds.points[1] = pos + glm::vec3(-bounds.size.x, bounds.size.y, -bounds.size.z);
	bounds.points[2] = pos + glm::vec3(bounds.size.x, -bounds.size.y, -bounds.size.z);
	bounds.points[3] = pos + glm::vec3(bounds.size.x, bounds.size.y, -bounds.size.z);
	bounds.points[4] = pos + glm::vec3(-bounds.size.x, -bounds.size.y, bounds.size.z);
	bounds.points[5] = pos + glm::vec3(-bounds.size.x, bounds.size.y, bounds.size.z);
	bounds.points[6] = pos + glm::vec3(bounds.size.x, -bounds.size.y, bounds.size.z);
	bounds.points[7] = pos + glm::vec3(bounds.size.x, bounds.size.y, bounds.size.z);
	return bounds;
}
bool boundsIntersect(AABB & a, AABB & b)
{
	if (overlap(project(a, worldAxis[0]), project(b, worldAxis[0]))
		&& overlap(project(a, worldAxis[1]), project(b, worldAxis[1]))
		&& overlap(project(a, worldAxis[2]), project(b, worldAxis[2]))) {
		return true;
	}
	return false;
}
//TODO frustum points calculation, frustum axis calculations
bool isVisible(AABB bounds, frustum * frus)
{
	//Test frustrum axes
	glm::vec2 objProj;
	glm::vec2 frusProj;
	for (int i = 0; i < 5; i++) {
		objProj = project(bounds, frus->axis[i]);
		frusProj = project(frus, frus->axis[i]);
		if (!overlap(objProj, frusProj)) {
			return false;
		}
	}

	//Test AABB axes
	for (int i = 0; i < 3; i++) {
		objProj = project(bounds, worldAxis[i]);
		frusProj = project(frus, worldAxis[i]);
		if (!overlap(objProj, frusProj)) {
			return false;
		}
	}

	return true;
}

AABB getLightBounds(LightObject l, float r) {
	AABB bounds = AABB();
	bounds.pos = l.position;
	bounds.size.x = r;
	bounds.size.y = r;
	bounds.size.z = r;
	bounds.points[0] = bounds.pos + glm::vec3(-r, -r, -r);
	bounds.points[1] = bounds.pos + glm::vec3(-r, r, -r);
	bounds.points[2] = bounds.pos + glm::vec3(r, -r, -r);
	bounds.points[3] = bounds.pos + glm::vec3(r, r, -r);
	bounds.points[4] = bounds.pos + glm::vec3(-r, -r, r);
	bounds.points[5] = bounds.pos + glm::vec3(-r, r, r);
	bounds.points[6] = bounds.pos + glm::vec3(r, -r, r);
	bounds.points[7] = bounds.pos + glm::vec3(r, r, r);
	return bounds;
}
bool lightAffects(AABB bounds, LightObject lightObj, float range)
{
	AABB lightBounds = getLightBounds(lightObj, range);
	if (boundsIntersect(bounds,lightBounds)) {
		return true;
	}
	return false;
}

int getPipelineIndex(std::vector<NodeManager<VulkanSceneNode>*>* renderNodes, material_type mType)
{
	for (int i = 0; i < renderNodes->size(); i++) {
		if (renderNodes->at(i)->pipelineType == mType) {
			return i;
		}
	}
	return -1;
}

int countRenderNodeDescriptors(std::vector<NodeManager<VulkanSceneNode>*>* renderNodes)
{
	int count = 0;
	for (NodeManager<VulkanSceneNode> * pipelineMap : *renderNodes) {
		count += pipelineMap->getSize();
	}
	return count;
}

void printNode(SceneNode * scene_node, int childCount)
{
	printf("--------------------\n"); //20 dashes
	printf("|    ID: %.4d      |\n", scene_node->id);
	printf("|@ %p|\n", (void*)scene_node);
	printf("| Bounds Position: |\n");
	printf("|    x: %7.2lf    |\n", scene_node->bounds.pos.x);
	printf("|    y: %7.2lf    |\n", scene_node->bounds.pos.y);
	printf("|    z: %7.2lf    |\n", scene_node->bounds.pos.z);
	printf("| Bounds Size:     |\n");
	printf("|    x: %7.2lf    |\n", scene_node->bounds.size.x);
	printf("|    y: %7.2lf    |\n", scene_node->bounds.size.y);
	printf("|    z: %7.2lf    |\n", scene_node->bounds.size.z);
	if (scene_node->isLeaf) {
		printf("|       LEAF       |\n");
	}
	else {
		printf("|      BRANCH      |\n");
		printf("|Children:         |\n");
		for (int i = 0; i < childCount; i++) {
			printf("| %p |\n", (void*)&scene_node->children[i]);
		}
	}
	printf("--------------------\n");
}

void printNodeSimple(SceneNode * scene_node, int tabs)
{
	printf(" ....\n"); //6 dashes
	printf("|%.4d|\n", scene_node->id);
	printf(" ''''\n");
}

void printSimpleBranch(SceneNode* node, int tabs, bool last) {
	//TODO print | for all depths higher up in the tree so they stay unbroken
	for (int i = 0; i < tabs-1; i++) {
		printf("\t");
	}
	printf("   |     ....\n");
	for (int i = 0; i < tabs-1; i++) {
		printf("\t");
	}
	printf("   -----|%.4d|\n", node->id);
	if (!last) {
		for (int i = 0; i < tabs - 1; i++) {
			printf("\t");
		}
		printf("   |     ''''\n");
	}
	else {
		for (int i = 0; i < tabs; i++) {
			printf("\t");
		}
		printf(" ''''\n");
	}
}

void printNodeEntities(SceneNode * scene_node, int tabs)
{
	printf(" ....\n"); //6 dashes
	printf("|%.4d|<", scene_node->id);
	for (int i = 0; i < scene_node->dynamicEntities->size(); i++) {
		printf("%d", scene_node->dynamicEntities->at(i));
		if (i < scene_node->dynamicEntities->size()-1) {
			printf(",");
		}
	}
	printf("|");
	for (int i = 0; i < scene_node->staticEntityCount; i++) {
		printf("%d", scene_node->staticEntities[i]);
		if (i < scene_node->staticEntityCount-1) {
			printf(",");
		}
	}
	printf("|");
	for (int i = 0; i < scene_node->lightCount; i++) {
		printf("%d", scene_node->lightIDs->at(i));
		if (i < scene_node->lightIDs->size()-1) {
			printf(",");
		}
	}
	printf(">\n");
	printf(" ''''\n");
}

void printBranchEntities(SceneNode* node, int tabs, bool last) {
	for (int i = 0; i < tabs - 1; i++) {
		printf("\t");
	}
	printf("   |     ....\n");
	for (int i = 0; i < tabs - 1; i++) {
		printf("\t");
	}
	printf("   -----|%.4d|<", node->id);
	for (int i = 0; i < node->dynamicEntities->size(); i++) {
		printf("%d", node->dynamicEntities->at(i));
		if (i < node->dynamicEntities->size()-1) {
			printf(",");
		}
	}
	printf("|");
	for (int i = 0; i < node->staticEntityCount; i++) {
		printf("%d", node->staticEntities[i]);
		if (i < node->staticEntityCount-1) {
			printf(",");
		}
	}
	printf("|");
	for (int i = 0; i < node->lightCount; i++) {
		printf("%d", node->lightIDs->at(i));
		if (i < node->lightCount-1) {
			printf(",");
		}
	}
	printf(">\n");
	if (!last) {
		for (int i = 0; i < tabs - 1; i++) {
			printf("\t");
		}
		printf("   |     ''''\n");
	}
	else {
		for (int i = 0; i < tabs; i++) {
			printf("\t");
		}
		printf(" ''''\n");
	}
}

void printSceneTree(SceneNode * scene_node, int childCount, int depth, int maxDepth)
{
	if (depth != 0) {
		if (scene_node->id % childCount == 0) {
			printSimpleBranch(scene_node, depth, true);
		}
		else {
			printSimpleBranch(scene_node, depth, false);
		}
	}
	else {
		printNodeSimple(scene_node, depth);
	}
	if (!scene_node->isLeaf) {
		for (int i = 0; i < childCount; i++) {
			printSceneTree(&scene_node->children[i], childCount, depth + 1, maxDepth);
		}
	}
}

void printSceneTreeWithEntities(SceneNode * scene_node, int childCount, int depth, int maxDepth)
{
	if (depth != 0) {
		if (scene_node->id % childCount == 0) {
			printBranchEntities(scene_node, depth, true);
		}
		else {
			printBranchEntities(scene_node, depth, false);
		}
	}
	else {
		printNodeEntities(scene_node, depth);
	}
	if (!scene_node->isLeaf) {
		for (int i = 0; i < childCount; i++) {
			printSceneTreeWithEntities(&scene_node->children[i], childCount, depth + 1, maxDepth);
		}
	}
}

bool locateEntity(SceneNode * scene_node, int entityID, int childCount)
{
	for (int e : *scene_node->dynamicEntities) {
		if (e == entityID) {
			printf("Entity %d is in node %d\n", entityID, scene_node->id);
			return true;
		}
	}
	if (!scene_node->isLeaf) {
		for (int i = 0; i < childCount; i++) {
			if (locateEntity(&scene_node->children[i], entityID, childCount) == true) {
				return true;
			}
		}
	}
	return false;
}

SceneNode* getParentNode(SceneNode * scene_node, int nodeID, int childCount)
{
	if (scene_node->isLeaf) {
		return nullptr;
	}
	for (int i = 0; i < childCount; i++) {
		if (scene_node->children[i].id == nodeID) {
			return scene_node;
		}
	}
	SceneNode* tempNode = nullptr;
	for (int i = 0; i < childCount; i++) {
		tempNode = getParentNode(&scene_node->children[i], nodeID, childCount);
		if (tempNode != nullptr) {
			return tempNode;
		}
	}
	return nullptr;
}

int getMaxLights(std::vector<V_GraphicsPipeline*>* pipelines)
{
	int max = 0;
	for (V_GraphicsPipeline* pipe : *pipelines) {
		if (pipe->max_lights > max) {
			max = pipe->max_lights;
		}
	}
	return max;
}

LightObject toLightObject(light light_in, glm::vec3 pos_in)
{
	LightObject retObject = LightObject();
	retObject.position = glm::vec4(pos_in, 1.0);
	retObject.color = light_in.color;
	return retObject;
}
