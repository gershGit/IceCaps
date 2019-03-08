#pragma once
#include "Core/EntitySystem.h"
#include "Vulkan/V_Components.h"
#include "Core/ManagersFactories.h"
#include "Vulkan/V_GraphicsPipeline.h"


class V_RenderSystem : public EntitySystem
{
private:
	//References to necessary objects
	V_Swapchain* swapchain;
	camera* activeCamera;
	int activeCameraID = 0;

	//Vulkan objects used for every draw call
	VkCommandBufferBeginInfo beginInfo;
	VkRenderPassBeginInfo renderPassInfo;
	VkClearValue clearValues[2];

	//Image currently being operated on by the renderer
	std::vector<uint32_t> imageIndices;

	//Records if a new swapchain was created
	bool recreatedSwapchain = false;

	//Submit information for the renderer
	VkPipelineStageFlags waitStages;
	std::vector<VkSemaphore> waitSemaphores;
	std::vector<VkSemaphore> signalSemaphores;
	std::vector<VkSubmitInfo> submitInfos;
	VkSwapchainKHR swapchainKHR;
	std::vector<VkPresentInfoKHR> presentInfos;

	//Graphics pipelines for this renderer to use
	std::vector<V_GraphicsPipeline*> *graphicsPipelines;
	std::vector<LightObject*> lightsData;
	LightObject nullLight = { glm::vec4(0), glm::vec4(0) };

	std::vector<std::vector<int>>* renderTrees = new std::vector<std::vector<int>>();
	std::vector<material_type> * pipelineTypes = new std::vector<material_type>();

public:
	//Descriptor sets that reference camera and light entities
	std::vector<VkDescriptorSet> descriptorSets;

	//Creation and intialization functions
	V_RenderSystem();
	void initialize();
	void createSubmitInfos();
	void createLightArray();
	void onConfigurationChange();
	void addEntity(int entityID);

	//Per call update functions
	void onUpdate();
	void acquireImage();
	void cullMeshes();
	void updateCameraBuffers();
	int replacedLightIndex(int lightCount, transform objectTrans, float currentLightDistance, int coreID);
	void updateLightBuffers(int lightCount, transform objectPos, int coreID);
	void updateLightBuffers(int lightCount, transform objectPos, std::vector<light*> reservedLights);
	void buildCommandBuffers();
	void renderEntities(int entitiesStart, int entityCount, int renderTreeIndex, V_GraphicsPipeline* pipeline, int coreID);
	void submitCommandBuffer(VkCommandBuffer &buffer, int coreID);
	void submitCommandBuffers(std::vector<VkCommandBuffer> &buffers, int coreID);
	void presentRender();

	void setSwapchain(V_Swapchain* swapchain_in) { swapchain = swapchain_in; };
	~V_RenderSystem();
};

