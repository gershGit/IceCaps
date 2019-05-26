#include "Vulkan/V_RenderSystem.h"
#include "Vulkan/V_Instance.h"
#include "Core/ManagersFactories.h"
#include <limits>
#include <thread>

glm::vec4 clipSpace[8] = {
	glm::vec4(-1, -1, -1, 1),
	glm::vec4(-1, 1, -1, 1),
	glm::vec4(1, -1, -1, 1),
	glm::vec4(1, 1, -1, 1),
	glm::vec4(-1, -1, 1, 1),
	glm::vec4(-1, 1, 1, 1),
	glm::vec4(1, -1, 1, 1),
	glm::vec4(1, 1, 1, 1)
};

//Constructor sets information
V_RenderSystem::V_RenderSystem()
{
	systemType = RENDER_SYSTEM;
	operatesOn = { TRANSFORM, V_MESH, V_MATERIAL, CAMERA, LIGHT_COMPONENT, PREFAB_COMPONENT };
	entityListRequiredComponents = { {V_MESH, V_MATERIAL} };

	beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	clearValues[0].color = { 0.0f, 0.5f, 0.5f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
}

//Initializes the renderer by creating necessary objects
void V_RenderSystem::initialize()
{
	createSubmitInfos();
}

//Creates submit structures for use in submitting command buffers
//Creates present structures for use in presenting rendered images
void V_RenderSystem::createSubmitInfos()
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

//TODO update light info in another system

//When configuration has been changed, reset the swapchain if necessary
void V_RenderSystem::onConfigurationChange()
{
	if (swapchainKHR != config->apiInfo.v_Instance->getSwapchain()->getSwapchain()) {
		swapchainKHR = config->apiInfo.v_Instance->getSwapchain()->getSwapchain();
	}
}

//Adds an entity into the trees
//TODO entities are all fed from scene

//System on update call
void V_RenderSystem::onUpdate()
{
	//std::cout << "Render system update" << std::endl;
	V_Instance* instance = config->apiInfo.v_Instance;
	//Wait for the inflight fence associated with the current frame to ensure we don't overfill the gpu with cpu commands
	vkWaitForFences(instance->getPrimaryDevice()->getLogicalDevice(), 1, &instance->inFlightFences[instance->currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	acquireImage();
	updateCameraBuffers();

	//TODO cullNodes() --> create a list of visible scene_nodes
	//TODO renderNodes() --> renders all scene_nodes of a pipeline, then renders all scene_nodes of the next pipeline, etc until all pipelines have been rendered
	//	   TODO renderNode() [called in threadpool from renderNodes] somehow needs to know the thread in the pool that is trying to render
	//		could just pass incremental id's < corecount for mostly random queue submission though no thread will actually be bound to a queue
	//TODO renderUI() --> UI is not contained in the scene tree so it needs its own render function
	//renderNode(scene); //TODO thread pool call
	presentRender();
	config->apiInfo.v_Instance->incrementFrame();	
}

//Renders a scene scene_node and its children
void V_RenderSystem::renderNode(SceneNode * scene_node)
{
	int vis = isVisible(scene_node->bounds, &frus);
	if (vis == 0) {
		//Not visible
		return;
	}
	else if (vis == 2) {
		//Completely inside frustrum
		//renderLeaf(scene_node->children[i])
	}
	else if (vis == 1) {
		//Intersects
		for (int i = 0; i < config->sceneChildren; i++) {
			renderNode(&scene_node->children[i]); //TODO thread pool call
		}
	}

	//Render it's command buffer
	for (int i = 0; i < renderNodes->size(); i++) {
		if (renderNodes->at(i)->getComponent(scene_node->id).hasCommandBuffer) {
			VkCommandBuffer buf = renderNodes->at(i)->getComponent(scene_node->id).staticDrawCommands;
			//TODO add the buffer to the correct command buffer list
		}
	}

	//Render any dynamic objects
	std::vector<std::vector<int>> visibleEntities = std::vector<std::vector<int>>();
	visibleEntities.resize(pipelineTypes->size());
	for (int i = 0; i < scene_node->dynamicEntities->size(); i++) {
		if (isVisible(bManager->getComponent(scene_node->dynamicEntities->at(i)), &frus)) {
			//TODO visible entities at material type
			visibleEntities[0].push_back(scene_node->dynamicEntities->at(i));
		}
	}
	if (visibleEntities.size() > 0) {
		//TODO create command buffers based on pipeline and store in the list
		//commandBuffers->at(material_type)->push_back(generateBuffer(visibleEntities[i], pipelines[i], scene_node->id));
	}
}

//Gets the image it will present
void V_RenderSystem::acquireImage()
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

//Updates the camera buffers for the current frame
void V_RenderSystem::updateCameraBuffers()
{
	float screenRatio = swapchain->getExtent().width / (float)swapchain->getExtent().height;
	glm::vec3 cameraPos = getCManager<transform>(*managers,TRANSFORM)->getComponent(activeCameraID).pos;
	viewPersp.view = glm::lookAt(cameraPos, cameraPos + activeCamera->lookDirection, activeCamera->upDirection);
	viewPersp.persp = glm::perspective(glm::radians((float) activeCamera->fov), screenRatio, activeCamera->near, activeCamera->far);

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

	//Get the frustum normals
	glm::mat4 vp = viewPersp.persp * viewPersp.view;
	frus.axis[0] = activeCamera->lookDirection;
	frus.axis[1] = glm::vec3(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0]);
	frus.axis[2] = glm::vec3(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0]);
	frus.axis[3] = glm::vec3(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1]);
	frus.axis[4] = glm::vec3(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1]);

	//Get the frustum vertices
	glm::mat4 i_vp = glm::inverse(vp);
	for (int i = 0; i < 8; i++) {
		frus.points[i] = glm::vec3(i_vp * clipSpace[i]);
	}
}

//Updates the light buffer with the closes lightCount lights
//TODO light data is updated in scene scene_nodes and each scene_node has a descriptor for its buffer
/*
void V_RenderSystem::updateLightBuffers(int lightCount, transform objectTrans, int coreID)
{
	float maxDistance = std::numeric_limits<float>::infinity();
	int lightsFound = 0;

	for (int lightID : getCManager<light>(*managers, LIGHT_COMPONENT)->getEntities()) {
		transform lightTrans = getCManager<transform>(*managers, TRANSFORM)->getComponent(lightID);
		float currentLightDistance = glm::length(lightTrans.pos - objectTrans.pos);
		if (currentLightDistance < maxDistance) {
			if (lightsFound < lightCount) {
				lightsData[coreID][lightsFound] = { glm::vec4(lightTrans.pos,getCManager<light>(*managers, LIGHT_COMPONENT)->getComponent(lightID).lType),getCManager<light>(*managers, LIGHT_COMPONENT)->getComponent(lightID).color };
				lightsFound++;
				if (lightsFound == lightCount) {
					maxDistance = currentLightDistance;
				}
			}
			else {
				lightsData[coreID][replacedLightIndex(lightCount, objectTrans, currentLightDistance, coreID)] = { 
					glm::vec4(lightTrans.pos,getCManager<light>(*managers, LIGHT_COMPONENT)->getComponent(lightID).lType), 
					getCManager<light>(*managers, LIGHT_COMPONENT)->getComponent(lightID).color };
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
}*/

//Builds and submits render command buffers
//TODO build command buffers is done using specific entities from the scene_nodes
/*
void V_RenderSystem::buildCommandBuffers()
{
	int pipelineIndex = 0;
	for (pipelineIndex = 0; pipelineIndex < renderTrees->size(); pipelineIndex++) {
		int entitiesPerCore = (int)renderTrees[pipelineIndex].size() / config->cpu_info->coreCount;
		int sentEntities = 0;
		for (unsigned int coreID = 0; coreID < config->cpu_info->coreCount; coreID++) {
			//TODO thread pooling so threads aren't created and destroyed constantly

			//config->cpu_info->threads[coreID] = std::thread(&V_RenderSystem::renderEntities, this, sentEntities, entitiesPerCore, pipelineIndex, config->apiInfo.v_Instance->getGraphicsPipeline(pipelineTypes->at(pipelineIndex)), coreID);
			
			renderEntities(sentEntities, entitiesPerCore, pipelineIndex, config->apiInfo.v_Instance->getGraphicsPipeline(pipelineTypes->at(pipelineIndex)), coreID);
			sentEntities += entitiesPerCore;
		}
		//Handles odd numbers of entities
		if (sentEntities < (int)renderTrees[pipelineIndex].size()) {
			renderEntities(sentEntities, (int)renderTrees[pipelineIndex].size() - sentEntities, pipelineIndex, config->apiInfo.v_Instance->getGraphicsPipeline(pipelineTypes->at(pipelineIndex)), 0);
		}
		for (unsigned int i = 0; i < config->cpu_info->coreCount; i++) {
			//config->cpu_info->threads[i].join();
		}
	}	
}*/

//Renders a subset of entities on a single core
//TODO move logic to a renderEntities function that just uses entity ids and a pipeline
/*
void V_RenderSystem::renderEntities(int entitiesStart, int entityCount, int renderTreeIndex, V_GraphicsPipeline* pipeline, int coreID)
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
			VkBuffer vertexBuffers[] = { getCManager<v_mesh>(*managers,V_MESH)->getComponent(renderTree->at(entitiesStart)).vBuffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(thisBuffer, getCManager<v_mesh>(*managers,V_MESH)->getComponent(renderTree->at(entitiesStart)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);

			updateLightBuffers(pipeline->max_lights, getCManager<transform>(*managers,TRANSFORM)->getComponent(renderTree->at(entitiesStart)), coreID);

			vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &pipeline->lightViewDescriptorSets[frameID], 0, nullptr);
			vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 1, 1, &getCManager<v_material>(*managers,V_MATERIAL)->getComponentAddress(entitiesStart)->descriptorSet, 0, nullptr);

			glm::mat4 transformPush = getTransformationMatrix(getCManager<transform>(*managers,TRANSFORM)->getComponent(renderTree->at(entitiesStart)));
			vkCmdPushConstants(thisBuffer, pipeline->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, static_cast<size_t>(sizeof(glm::mat4)), &transformPush);

			vkCmdDrawIndexed(thisBuffer, getCManager<v_mesh>(*managers,V_MESH)->getComponent(renderTree->at(entitiesStart)).indicesCount, 1, 0, 0, 0); //Draw call
		}

		vkCmdEndRenderPass(thisBuffer);

		vkEndCommandBuffer(thisBuffer);

		submitCommandBuffer(thisBuffer, coreID);
	}
}
*/

VkCommandBuffer V_RenderSystem::generateBuffer(std::vector<int> entities, V_GraphicsPipeline* pipeline, int coreID, int scene_nodeID) {
	int frameID = (int)config->apiInfo.v_Instance->currentFrame;
	VkCommandBuffer thisBuffer = config->apiInfo.v_Instance->getGraphicsCommandPool(coreID)->commandBuffers[frameID*scene_nodeID];
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

	for (int entity : entities) {
		//TODO store managers not getCManager each time
		VkBuffer vertexBuffers[] = { getCManager<v_mesh>(*managers,V_MESH)->getComponent(entity).vBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(thisBuffer, getCManager<v_mesh>(*managers, V_MESH)->getComponent(entity).indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		//TODO buffer is updated in different system, this just needs to use the descriptor from the scene scene_node NOT the pipeline
		//updateLightBuffers(pipeline->max_lights, getCManager<transform>(*managers, TRANSFORM)->getComponent(entity), coreID);

		vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &pipeline->lightViewDescriptorSets[frameID], 0, nullptr);
		vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 1, 1, &getCManager<v_material>(*managers, V_MATERIAL)->getComponentAddress(entity)->descriptorSet, 0, nullptr);

		glm::mat4 transformPush = getTransformationMatrix(getCManager<transform>(*managers, TRANSFORM)->getComponent(entity));
		vkCmdPushConstants(thisBuffer, pipeline->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, static_cast<size_t>(sizeof(glm::mat4)), &transformPush);

		vkCmdDrawIndexed(thisBuffer, getCManager<v_mesh>(*managers, V_MESH)->getComponent(entity).indicesCount, 1, 0, 0, 0); //Draw call
	}

	vkCmdEndRenderPass(thisBuffer);

	vkEndCommandBuffer(thisBuffer);

	return thisBuffer;
}

//Submits a command buffer to a graphics queue
void V_RenderSystem::submitCommandBuffer(VkCommandBuffer &buffer, int coreID)
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
void V_RenderSystem::presentRender()
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

void V_RenderSystem::cleanup()
{
	delete renderNodes;
}

V_RenderSystem::~V_RenderSystem()
{
}