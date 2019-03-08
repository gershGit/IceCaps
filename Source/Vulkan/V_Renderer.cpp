#include "V_Renderer.h"
#include "V_Instance.h"
#include "ManagersFactories.h"
#include <limits>
#include <thread>

//Constructor sets information
V_Renderer::V_Renderer()
{
	operatesOn = { TRANSFORM, V_MESH, V_MATERIAL, CAMERA, LIGHT_COMPONENT, PREFAB_COMPONENT };
	entityListRequiredComponents = { {V_MESH, V_MATERIAL}, {PREFAB_COMPONENT} };

	beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	clearValues[0].color = { 0.0f, 0.5f, 0.5f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
}

//Initializes the renderer by creating necessary objects
void V_Renderer::initialize()
{
	createSubmitInfos();
	createLightArray();
}

//Creates submit structures for use in submitting command buffers
//Creates present structures for use in presenting rendered images
void V_Renderer::createSubmitInfos()
{
	submitInfos.resize(config->swapchainBuffering);
	presentInfos.resize(config->swapchainBuffering);
	waitSemaphores.resize(config->swapchainBuffering);
	signalSemaphores.resize(config->swapchainBuffering);
	imageIndices.resize(config->swapchainBuffering);
	swapchainKHR = { config->apiInfo.v_Instance->getSwapchain()->getSwapchain() };
	for (int i = 0; i < config->swapchainBuffering; i++) {
		waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		waitSemaphores[i] = { config->apiInfo.v_Instance->imageAvailableSemaphores[i] };	
		signalSemaphores[i] = { config->apiInfo.v_Instance->renderFinishedSemaphores[i] };
		imageIndices[i] = i;

		//Structure to hold information of the draw call we are submitting for each frame in flights
		submitInfos[i].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfos[i].waitSemaphoreCount = 1;
		submitInfos[i].pWaitSemaphores = &waitSemaphores[i];
		submitInfos[i].pWaitDstStageMask = &waitStages;
		submitInfos[i].commandBufferCount = 1;	
		submitInfos[i].signalSemaphoreCount = 1;
		submitInfos[i].pSignalSemaphores = &signalSemaphores[i];

		//Structure to hold information on presentation of a finished render
		presentInfos[i].sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfos[i].waitSemaphoreCount = 1;
		presentInfos[i].pWaitSemaphores = &signalSemaphores[i];
		presentInfos[i].swapchainCount = 1;
		presentInfos[i].pSwapchains = &swapchainKHR;
		presentInfos[i].pImageIndices = &imageIndices[i];
		presentInfos[i].pResults = nullptr;
	}
}

//Creates an array large enough to hold all lights needed in a render
void V_Renderer::createLightArray()
{
	int most_max_lights = 0;
	graphicsPipelines = config->apiInfo.v_Instance->getGraphicsPipelines();
	for (V_GraphicsPipeline* pipeline : *graphicsPipelines) {
		if (pipeline->max_lights > most_max_lights) {
			most_max_lights = pipeline->max_lights;
		}
	}
	for (int i = 0; i < config->cpu_info->coreCount; i++) {
		lightsData[i] = (LightObject*)malloc(sizeof(LightObject) * most_max_lights);
	}
}

//When configuration has been changed, reset the swapchain if necessary
void V_Renderer::onConfigurationChange()
{
	if (swapchainKHR != config->apiInfo.v_Instance->getSwapchain()->getSwapchain()) {
		swapchainKHR = config->apiInfo.v_Instance->getSwapchain()->getSwapchain();
	}
}

//Adds an entity into the trees
void V_Renderer::addEntity(int entityID)
{
	material_type entityPipelineType = getVulkanMaterialManager(*managers)->getEntity(entityID)->matType;
	for (int i = 0; i < pipelineTypes->size(); i++) {
		if (entityPipelineType == pipelineTypes->at(i)) {
			renderTrees->at(i).push_back(entityID);
			return;
		}
	}

	pipelineTypes->push_back(entityPipelineType);
	renderTrees->push_back(std::vector<int>());
	renderTrees->at(renderTrees->size() - 1).push_back(entityID);
}

//System on update call
void V_Renderer::onUpdate()
{
	V_Instance* instance = config->apiInfo.v_Instance;
	//Wait for the inflight fence associated with the current frame to ensure we don't overfill the gpu with cpu commands
	vkWaitForFences(instance->getPrimaryDevice()->getLogicalDevice(), 1, &instance->inFlightFences[instance->currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	acquireImage();
	updateCameraBuffers();
	cullMeshes();
	buildCommandBuffers();
	presentRender();
	config->apiInfo.v_Instance->incrementFrame();	
}

//Gets the image it will present
void V_Renderer::acquireImage()
{
	V_Instance* instance = config->apiInfo.v_Instance;
	//Acquires the next image using a semaphore and stores the index into imageIndex
	VkResult result = vkAcquireNextImageKHR(instance->getPrimaryDevice()->getLogicalDevice(),
		instance->getSwapchain()->getSwapchain(), std::numeric_limits<uint64_t>::max(),
		instance->imageAvailableSemaphores[instance->currentFrame], VK_NULL_HANDLE, &imageIndices[instance->currentFrame]);

	//Check if the swawpchain needs to be rebuilt
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		instance->recreateSwapchain();
		onConfigurationChange();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}
}

//Culls meshes from the trees for rendering
void V_Renderer::cullMeshes()
{
}

//Updates the camera buffers for the current frame
void V_Renderer::updateCameraBuffers()
{
	ViewPersp viewPersp = ViewPersp();
	if (activeCamera == nullptr) {
		activeCamera = &getCameraManager(*managers)->getComponents()->at(0);
		activeCameraID = getCameraManager(*managers)->getEntityAt(0);
	}

	glm::vec3 cameraPos = getTransformManager(*managers)->transformArray[activeCameraID].pos;
	viewPersp.view = glm::lookAt(cameraPos, cameraPos + activeCamera->lookDirection, activeCamera->upDirection);
	viewPersp.persp = glm::perspective(glm::radians((float) activeCamera->fov), swapchain->getExtent().width / (float)swapchain->getExtent().height, activeCamera->near, activeCamera->far);

	void* data;
	vkMapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), 
		config->apiInfo.v_Instance->getGraphicsPipeline(PBR)->lightViewBufferVRAMs[config->apiInfo.v_Instance->currentFrame],
		0, 
		sizeof(viewPersp), 
		0, 
		&data);
	memcpy(data, &viewPersp, sizeof(viewPersp));
	vkUnmapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), 
		config->apiInfo.v_Instance->getGraphicsPipeline(PBR)->lightViewBufferVRAMs[config->apiInfo.v_Instance->currentFrame]);
}

//Returns the index of which light is to be replaced in the light array
int V_Renderer::replacedLightIndex(int lightCount, transform objectTrans, float currentLightDistance, int coreID) {
	float maxLightInArray = 0.0f;
	int maxLightIndex = -1;
	for (int i = 0; i < lightCount; i++) {
		float tempDistance = glm::length(glm::vec3(lightsData[coreID][i].position) - objectTrans.pos);
		if (tempDistance > currentLightDistance && tempDistance > maxLightInArray) {
			maxLightIndex = i;
			maxLightInArray = tempDistance;
		}
	}
	return maxLightIndex;
}

//Updates the light buffer with the closes lightCount lights
void V_Renderer::updateLightBuffers(int lightCount, transform objectTrans, int coreID)
{
	float maxDistance = std::numeric_limits<float>::infinity();
	int lightsFound = 0;

	for (int lightID : *getLightManager(*managers)->getEntities()) {
		transform lightTrans = getTransformManager(*managers)->transformArray[lightID];
		float currentLightDistance = glm::length(lightTrans.pos - objectTrans.pos);
		if (currentLightDistance < maxDistance) {
			if (lightsFound < lightCount) {
				lightsData[coreID][lightsFound] = { glm::vec4(lightTrans.pos,getLightManager(*managers)->getComponent(lightID).lType), getLightManager(*managers)->getComponent(lightID).color };
				lightsFound++;
				if (lightsFound == lightCount) {
					maxDistance = currentLightDistance;
				}
			}
			else {
				lightsData[coreID][replacedLightIndex(lightCount, objectTrans, currentLightDistance, coreID)] = { glm::vec4(lightTrans.pos,getLightManager(*managers)->getComponent(lightID).lType), getLightManager(*managers)->getComponent(lightID).color };
			}
		}
	}
	if (lightsFound < lightCount) {
		for (int i = lightsFound; i < lightCount; i++) {
			lightsData[coreID][i] = nullLight;
		}
	}

	void* data;
	vkMapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(),
		config->apiInfo.v_Instance->getGraphicsPipeline(PBR)->lightViewBufferVRAMs[config->apiInfo.v_Instance->currentFrame],
		sizeof(ViewPersp),
		sizeof(LightObject) * lightCount,
		0,
		&data);
	memcpy(data, &lightsData[coreID], sizeof(LightObject) * lightCount);
	vkUnmapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(),
		config->apiInfo.v_Instance->getGraphicsPipeline(PBR)->lightViewBufferVRAMs[config->apiInfo.v_Instance->currentFrame]);
}

//Builds and submits render command buffers
void V_Renderer::buildCommandBuffers()
{
	int pipelineIndex = 0;
	for (pipelineIndex = 0; pipelineIndex < renderTrees->size(); pipelineIndex++) {
		int entitiesPerCore = (int)renderTrees[pipelineIndex].size() / config->cpu_info->coreCount;
		int sentEntities = 0;
		for (unsigned int coreID = 0; coreID < config->cpu_info->coreCount; coreID++) {
			config->cpu_info->threads[coreID] = std::thread(&V_Renderer::renderEntities, this, sentEntities, entitiesPerCore, pipelineIndex, config->apiInfo.v_Instance->getGraphicsPipeline(pipelineTypes->at(pipelineIndex)), coreID);
			sentEntities += entitiesPerCore;
		}
		//Handles odd numbers of entities
		if (sentEntities < (int)renderTrees[pipelineIndex].size()) {
			renderEntities(sentEntities, (int)renderTrees[pipelineIndex].size() - sentEntities, pipelineIndex, config->apiInfo.v_Instance->getGraphicsPipeline(pipelineTypes->at(pipelineIndex)), 0);
		}
		for (unsigned int i = 0; i < config->cpu_info->coreCount; i++) {
			config->cpu_info->threads[i].join();
		}
	}	
}

//Renders a subset of entities on a single core
void V_Renderer::renderEntities(int entitiesStart, int entityCount, int renderTreeIndex, V_GraphicsPipeline* pipeline, int coreID)
{
	if (entityCount > 0) {
		std::vector<int> * renderTree = &renderTrees->at(renderTreeIndex);
		int frameID = (int)config->apiInfo.v_Instance->currentFrame;
		VkCommandBuffer thisBuffer = config->apiInfo.v_Instance->getGraphicsCommandPool(coreID)->commandBuffers[frameID];
		vkResetCommandBuffer(thisBuffer, 0);
		vkBeginCommandBuffer(thisBuffer, &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pipeline->getRenderPass();
		renderPassInfo.framebuffer = pipeline->getFramebuffers()->at(frameID);
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = swapchain->getExtent();
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(thisBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());

		int limit = entitiesStart + entityCount;
		for (entitiesStart; entitiesStart < limit; entitiesStart++) {
			VkBuffer vertexBuffers[] = { getVulkanMeshManager(*managers)->getComponent(renderTree->at(entitiesStart)).vBuffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(thisBuffer, getVulkanMeshManager(*managers)->getComponent(renderTree->at(entitiesStart)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);

			updateLightBuffers(pipeline->max_lights, getTransformManager(*managers)->transformArray[renderTree->at(entitiesStart)], coreID);

			vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &pipeline->lightViewDescriptorSets[frameID], 0, nullptr);
			vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 1, 1, &getVulkanMaterialManager(*managers)->getComponents()->at(entitiesStart).descriptorSet, 0, nullptr);

			glm::mat4 transformPush = getTransformManager(*managers)->getTransformMatrix(renderTree->at(entitiesStart));
			vkCmdPushConstants(thisBuffer, pipeline->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, static_cast<size_t>(sizeof(glm::mat4)), &transformPush);

			vkCmdDrawIndexed(thisBuffer, getVulkanMeshManager(*managers)->getComponent(renderTree->at(entitiesStart)).indicesCount, 1, 0, 0, 0); //Draw call
		}

		vkCmdEndRenderPass(thisBuffer);

		vkEndCommandBuffer(thisBuffer);

		submitCommandBuffer(thisBuffer, coreID);
	}
}

//Submits a command buffer to a graphics queue
void V_Renderer::submitCommandBuffer(VkCommandBuffer &buffer, int coreID)
{
	submitInfos[config->apiInfo.v_Instance->currentFrame].pCommandBuffers = &buffer;

	//Lock the fence that was open to allow for us to use this image
	vkResetFences(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), 1, &config->apiInfo.v_Instance->inFlightFences[config->apiInfo.v_Instance->currentFrame]);

	//Submit the command to the GPU to draw the mesh
	VkResult res = vkQueueSubmit(config->apiInfo.v_Instance->getPrimaryDevice()->getGraphicsQueue(coreID), 
		1, 
		&submitInfos[config->apiInfo.v_Instance->currentFrame], 
		config->apiInfo.v_Instance->inFlightFences[config->apiInfo.v_Instance->currentFrame]);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
}

//Presents the final rendered image
void V_Renderer::presentRender()
{
	if (recreatedSwapchain) {
		VkSwapchainKHR swapchains[] = { config->apiInfo.v_Instance->getSwapchain()->getSwapchain() };
		for (int i = 0; i < config->swapchainBuffering; i++) {
			presentInfos[config->apiInfo.v_Instance->currentFrame].pSwapchains = swapchains;
		}
	}

	VkResult res = vkQueuePresentKHR(config->apiInfo.v_Instance->getPrimaryDevice()->getPresentQueue(), &presentInfos[config->apiInfo.v_Instance->currentFrame]);
	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || config->apiInfo.v_Instance->framebufferResized) {
		config->apiInfo.v_Instance->framebufferResized = false;
		config->apiInfo.v_Instance->recreateSwapchain();
	}
	else if (res != VK_SUCCESS) {
		throw std::runtime_error("Could not present image");
	}
}

V_Renderer::~V_Renderer()
{
}