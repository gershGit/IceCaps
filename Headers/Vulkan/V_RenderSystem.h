#pragma once
#include "Core/EntitySystem.h"
#include "Core/ThreadPool.h"
#include "Vulkan/V_Components.h"
#include "Core/ManagersFactories.h"
#include "Vulkan/V_GraphicsPipeline.h"


class V_RenderSystem : public EntitySystem
{
private:
	//References to necessary objects
	V_Swapchain* swapchain;
	v_camera* activeCamera;
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

	V_Instance* mInstance;

	frustum frus;
	ViewPersp viewPersp = ViewPersp();
	std::vector<std::vector<VkCommandBuffer>> buffersToSubmit;
	ArrayManager<AABB>* bManager;
	std::vector<NodeManager<VulkanSceneNode>*> * renderNodes;
	std::vector<material_type> * pipelineTypes = new std::vector<material_type>();

	std::mutex cullMutex;
	std::vector<int> visibleNodes = std::vector<int>();

public:
	//Descriptor sets that reference camera and light entities
	std::vector<VkDescriptorSet> descriptorSets;

	//Creation and intialization functions
	V_RenderSystem();
	void initialize();
	void createSubmitInfos();
	void onConfigurationChange();

	//Per call update functions
	void onUpdate();
	void doSomething(int& someInt);
	void cullNode(std::vector<int>& visible, SceneNode& node, frustum& frus, configurationStructure& config);
	void cullNodes();
	void renderAllNodes();
	void renderSinglePipeline(V_GraphicsPipeline* gPipeline, NodeManager<VulkanSceneNode>* nodeManager);
	void renderEntities(V_GraphicsPipeline& gPipeline, std::vector<int> entityIDs, VulkanSceneNode& node);
	void acquireImage();
	void updateCameraBuffers();
	VkCommandBuffer generateBuffer(std::vector<int> entities, V_GraphicsPipeline * pipeline, int coreID, int scene_nodeID);
	void submitCommandBuffer(VkCommandBuffer &buffer, int coreID);
	void presentRender();
	
	void setActiveCamera(int id, v_camera * cam) { activeCameraID = id; activeCamera = cam; }
	void setSwapchain(V_Swapchain* swapchain_in) { swapchain = swapchain_in; };
	void setRenderNodes(std::vector<NodeManager<VulkanSceneNode>*> *renderNodes_in) { renderNodes = renderNodes_in; };

	void cleanup();
	~V_RenderSystem();
};

