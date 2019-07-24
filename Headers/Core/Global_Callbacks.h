/*
* This class contains callback functions for vulkan and OpenGL
* These callbacks are necessary throughout the engine so must be globally accessed
*/

#pragma once
#include "Core/EntityComponents.h"
#include "Vulkan/Vulkan_Headers.h"
#include "Core/ManagersFactories.h"
#include <optional>

class V_GraphicsPipeline;

//Structure that holds the indices for various queue families
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	int graphicsFamilyQueues = 0;
	std::optional<uint32_t> computeFamily;
	int computeFamilyQueues = 0;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	//True when a graphics and present family have been identified
	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
	bool isCompleteWithCompute() {
		return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
	}
};

//Details on what is supported by the available swapchain
struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


//Finds the queue families on a graphics card and returns the index of the family supporting graphics
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices findTransferFamily(VkPhysicalDevice device, VkSurfaceKHR surface);

//Finds the details of what is supported by the swapchain
SwapchainSupportDetails querySwapchainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);

//Message callback function
static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}

//Creates a messenger for debug statements
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

//Destroys the messenger for debug statements
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

//Finds the memeory type based on a device with memory flags
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

//Prints a list of queue families available on a physical device
void printQueueFamilies(std::vector<VkQueueFamilyProperties> queueFamiles, VkPhysicalDevice device, VkSurfaceKHR surface);

//Functions for finding and tranlating the sampling available on a physical device
int getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
VkSampleCountFlagBits getMaxUsableSampleCountFlags(VkPhysicalDevice physicalDevice);
VkSampleCountFlagBits intToSampleFlagBits(int bits);


//Functions for AABB
void initializeABBB(AABB * bounds);
bool isInside(AABB * area, AABB * object);
bool isInside2D(AABB* area, AABB* object);
AABB getMaxBounds(AABB * a, AABB * b);

AABB getBounds(collider &col, glm::vec3 pos);
AABB getMeshBounds(AABB *bounds, glm::vec3 pos);
bool boundsIntersect(AABB &a, AABB &b);
bool isVisible(AABB bounds, frustum * frus);
bool lightAffects(AABB bounds, LightObject lightObj, float range);

//Functions for render scene_node information
int getPipelineIndex(std::vector<NodeManager<VulkanSceneNode>*> * renderNodes, material_type mType);
int countRenderNodeDescriptors(std::vector<NodeManager<VulkanSceneNode>*> * renderNodes);
void printNode(SceneNode* scene_node, int childCount);
void printSceneTree(SceneNode * scene_node, int childCount, int depth, int maxDepth);
void printSceneTreeWithEntities(SceneNode * scene_node, int childCount, int depth, int maxDepth);
void printSceneAsArea(int depth);
bool locateEntity(SceneNode* scene_node, int entityID, int childCount);
SceneNode* getParentNode(SceneNode* scene_node, int nodeID, int childCount);
int getMaxLights(std::vector<V_GraphicsPipeline*>* pipelines);
LightObject toLightObject(light light_in, glm::vec3 pos_in);

//Widely usable functions
int findInt(int arrayIn[], int value, int arraySize);

glm::mat4 getTransformationMatrix(transform transform_component);
std::vector<int> getIntVector(char* asString, int size);