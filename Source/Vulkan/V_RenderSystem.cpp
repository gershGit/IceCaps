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
	usesScene = true;

	systemType = RENDER_SYSTEM;
	operatesOn = { TRANSFORM, V_MESH, V_MATERIAL, V_CAMERA, LIGHT_COMPONENT, PREFAB_COMPONENT, V_SKINNED_MESH };
	entityListRequiredComponents = { {V_MESH, V_MATERIAL}, {V_SKINNED_MESH, V_MATERIAL} };

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
	mInstance = config->apiInfo.v_Instance;
	createSubmitInfos();
	graphicsPipelines = new std::vector<V_GraphicsPipeline*>();
	for (int i = 0; i < mInstance->getGraphicsPipelines()->size(); i++) {
		graphicsPipelines->push_back(mInstance->getGraphicsPipelines()->at(i));
	}
	skinManager = dynamic_cast<MappedManager<vk_skinned_mesh>*>(getCManager<vk_skinned_mesh>(*managers, V_SKINNED_MESH));
	meshManager = dynamic_cast<MappedManager<v_mesh>*>(getCManager<v_mesh>(*managers, V_MESH));
	matManager = dynamic_cast<MappedManager<v_material>*>(getCManager<v_material>(*managers, V_MATERIAL));
	tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
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
		//TODO figure out how to signal using fences and semaphores properly
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

//When configuration has been changed, reset the swapchain if necessary
void V_RenderSystem::onConfigurationChange()
{
	if (swapchainKHR != config->apiInfo.v_Instance->getSwapchain()->getSwapchain()) {
		swapchainKHR = config->apiInfo.v_Instance->getSwapchain()->getSwapchain();
	}
}

//System on update call
void V_RenderSystem::onUpdate()
{
	//Wait for the inflight fence associated with the current frame to ensure we don't overfill the gpu with cpu commands
	vkWaitForFences(mInstance->getPrimaryDevice()->getLogicalDevice(), 1, &mInstance->inFlightFences[mInstance->currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	acquireImage();
	updateCameraBuffers();

	cullNodes(); //create a list of visible scene_nodes
	renderSingleThread();
	//renderAllNodes();
	//TODO renderUI() --> UI is not contained in the scene tree so it needs its own render function
	presentRender();
	config->apiInfo.v_Instance->incrementFrame();	
}

//Determines if a node is visible or not
void V_RenderSystem::cullNode(std::vector<int> &visible, SceneNode &node, frustum &frus, configurationStructure &config) {
	if (isVisible(node.bounds, &frus)) {
		cullMutex.lock();
		visible.push_back(node.id);
		cullMutex.unlock();
		if (!node.isLeaf) {
			for (int i = 0; i < config.sceneChildren; i++) {
				auto cullNodeFunc = std::bind(&V_RenderSystem::cullNode, this, std::ref(visible), std::ref(node.children[i]), std::ref(frus), std::ref(config));
				ThreadPool::submitJob(cullNodeFunc);
			}
		}
	}
}

//Find the nodes that are visible
void V_RenderSystem::cullNodes() {
	visibleNodes.clear();
	cullNode(visibleNodes, *scene, frus, *config);
	ThreadPool::workToComplete();
}

//Uses a single thread for command buffer generation ensuring a working renderer as the multi-thread version is revised
void V_RenderSystem::renderSingleThread() {
	V_GraphicsPipeline gPipeline = *graphicsPipelines->at(0);

	int frameID = (int)config->apiInfo.v_Instance->currentFrame;
	VkCommandBuffer thisBuffer = renderNodes->at(0)->getComponent(0).dynamicBuffers[frameID];
	vkResetCommandBuffer(thisBuffer, 0);
	vkBeginCommandBuffer(thisBuffer, &beginInfo);
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = gPipeline.getRenderPass();
	renderPassInfo.framebuffer = gPipeline.getFramebuffers()->at(frameID);
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapchain->getExtent();
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(thisBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipeline());

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < visibleNodes.size(); j++) {
			if (renderNodes->at(i)->hasEntity(visibleNodes.at(j))) {
				VulkanSceneNode* tempNode = renderNodes->at(i)->getComponentAddress(visibleNodes.at(j));
				std::vector<int> entityIDs = tempNode->dynamicEntities;
				if (tempNode->dynamicEntities.size() > 0) {
					for (int k = 0; k < entityIDs.size(); k++) {
						//If it is a cpu based skinned mesh then get the proper mesh info from the skinned mesh manager
						VkBuffer vertexBuffers[1];
						if (skinManager->hasEntity(entityIDs.at(k))) {
							vertexBuffers[0] = { skinManager->getComponent(entityIDs.at(k)).vBuffer };
						}
						else {
							vertexBuffers[0] = { meshManager->getComponent(entityIDs.at(k)).vBuffer };
						}
						VkDeviceSize offsets[] = { 0 };

						vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

						if (skinManager->hasEntity(entityIDs.at(k))) {
							vkCmdBindIndexBuffer(thisBuffer, skinManager->getComponent(entityIDs.at(k)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);
						}
						else {
							vkCmdBindIndexBuffer(thisBuffer, meshManager->getComponent(entityIDs.at(k)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);
						}

						vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipelineLayout(), 0, 1, &tempNode->camLightDescSets[frameID], 0, nullptr);
						vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipelineLayout(), 1, 1, &matManager->getComponentAddress(entityIDs.at(k))->descriptorSet, 0, nullptr);

						glm::mat4 transformPush = getTransformationMatrix(tManager->getComponent(entityIDs.at(k)));
						vkCmdPushConstants(thisBuffer, gPipeline.getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, static_cast<size_t>(sizeof(glm::mat4)), &transformPush);

						if (skinManager->hasEntity(entityIDs.at(k))) {
							vkCmdDrawIndexed(thisBuffer, skinManager->getComponent(entityIDs.at(k)).indicesCount, 1, 0, 0, 0);
						}
						else {
							vkCmdDrawIndexed(thisBuffer, meshManager->getComponent(entityIDs.at(k)).indicesCount, 1, 0, 0, 0); //Draw call
						}
					}
				}
			}
		}
	}
	vkCmdEndRenderPass(thisBuffer);
	vkEndCommandBuffer(thisBuffer);
	submitCommandBuffer(thisBuffer, 0);
}

//Renders all the visible nodes in order by pipeline
void V_RenderSystem::renderAllNodes() {
	for (int i = 0; i < graphicsPipelines->size(); i++) {
		renderSinglePipeline(graphicsPipelines->at(i), renderNodes->at(i));
	}
	ThreadPool::workToComplete();
}

//Renders a pipeline scene node
void V_RenderSystem::renderSinglePipeline(V_GraphicsPipeline* gPipeline, NodeManager<VulkanSceneNode> * nodeManager) {
	for (int i = 0; i < visibleNodes.size(); i++) {
		if (nodeManager->hasEntity(visibleNodes.at(i))) {
			VulkanSceneNode* tempNode = nodeManager->getComponentAddress(visibleNodes.at(i));
			if (tempNode->dynamicEntities.size() > 0) {
				auto renderFunc = std::bind(&V_RenderSystem::renderEntities, this, std::ref(*gPipeline), tempNode->dynamicEntities, std::ref(*tempNode));
				ThreadPool::submitJob(renderFunc);
			}
			if (tempNode->hasCommandBuffer) {
				submitCommandBuffer(tempNode->staticDrawCommands, 0); //TODO figure out how to do this per core
			}
		}
	}
}

//Renders a list of entities
void V_RenderSystem::renderEntities(V_GraphicsPipeline& gPipeline, std::vector<int> entityIDs, VulkanSceneNode& node) {
	int coreID = 0; //TODO make this per core
	int frameID = (int) config->apiInfo.v_Instance->currentFrame;

	VkCommandBuffer thisBuffer = node.dynamicBuffers[frameID];
	vkResetCommandBuffer(thisBuffer, 0);
	vkBeginCommandBuffer(thisBuffer, &beginInfo);

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = gPipeline.getRenderPass();
	renderPassInfo.framebuffer = gPipeline.getFramebuffers()->at(frameID);
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapchain->getExtent();
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(thisBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipeline());

	for (int i=0; i < entityIDs.size(); i++) {
		//If it is a cpu based skinned mesh then get the proper mesh info from the skinned mesh manager
		VkBuffer vertexBuffers[1];
		if (skinManager->hasEntity(entityIDs.at(i))) {
			vertexBuffers[0] = { skinManager->getComponent(entityIDs.at(i)).vBuffer };
		}
		else {
			vertexBuffers[0] = { meshManager->getComponent(entityIDs.at(i)).vBuffer };
		}
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

		if (skinManager->hasEntity(entityIDs.at(i))) {
			vkCmdBindIndexBuffer(thisBuffer, skinManager->getComponent(entityIDs.at(i)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		}
		else {
			vkCmdBindIndexBuffer(thisBuffer, meshManager->getComponent(entityIDs.at(i)).indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		}

		vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipelineLayout(), 0, 1, &node.camLightDescSets[frameID], 0, nullptr);
		vkCmdBindDescriptorSets(thisBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.getPipelineLayout(), 1, 1, &matManager->getComponentAddress(entityIDs.at(i))->descriptorSet, 0, nullptr);

		glm::mat4 transformPush = getTransformationMatrix(tManager->getComponent(entityIDs.at(i)));
		vkCmdPushConstants(thisBuffer, gPipeline.getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, static_cast<size_t>(sizeof(glm::mat4)), &transformPush);

		if (skinManager->hasEntity(entityIDs.at(i))) {
			vkCmdDrawIndexed(thisBuffer, skinManager->getComponent(entityIDs.at(i)).indicesCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDrawIndexed(thisBuffer, meshManager->getComponent(entityIDs.at(i)).indicesCount, 1, 0, 0, 0); //Draw call
		}
	}

	vkCmdEndRenderPass(thisBuffer);

	vkEndCommandBuffer(thisBuffer);

	submitCommandBuffer(thisBuffer, coreID);
}

//Gets the image it will present
void V_RenderSystem::acquireImage()
{
	V_Instance* instance = config->apiInfo.v_Instance;
	//Acquires the next image using a semaphore and stores the index into imageIndex
	//TODO signal imageAvailableSemaphore if necessessary instead of VK_NULL_HANDLE, or use a fence so it can be a 1 time CPU check
	VkResult result = vkAcquireNextImageKHR(instance->getPrimaryDevice()->getLogicalDevice(),
		instance->getSwapchain()->getSwapchain(), std::numeric_limits<uint64_t>::max(),
		mInstance->imageAvailableSemaphores[mInstance->currentFrame], VK_NULL_HANDLE, &imageIndices[instance->currentFrame]);

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
		activeCamera->camVRAM[config->apiInfo.v_Instance->currentFrame],
		0, 
		sizeof(viewPersp), 
		0, 
		&data);
	memcpy(data, &viewPersp, sizeof(viewPersp));
	vkUnmapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), 
		activeCamera->camVRAM[config->apiInfo.v_Instance->currentFrame]);

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

//Used for generating static command buffers that are able to render static geometry
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
		VkBuffer vertexBuffers[] = { getCManager<v_mesh>(*managers,V_MESH)->getComponent(entity).vBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(thisBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(thisBuffer, getCManager<v_mesh>(*managers, V_MESH)->getComponent(entity).indexBuffer, 0, VK_INDEX_TYPE_UINT16);

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
	//TODO fences should only worry about the image at the end
	//TODO Rebuild submit infos per frame? 
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
	for (int i = 0; i < renderNodes->size(); i++) {
		std::vector<int> renderNodesInPipeline = renderNodes->at(i)->getEntities();
		for (int j = 0; j < renderNodesInPipeline.size(); j++) {
			for (int k = 0; k < config->swapchainBuffering; k++) {
				vkDestroyBuffer(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(),
					renderNodes->at(i)->getComponent(renderNodesInPipeline.at(j)).lightBuffers[k],
					nullptr);
				vkFreeMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(),
					renderNodes->at(i)->getComponent(renderNodesInPipeline.at(j)).lightBufferVRAM[k],
					nullptr);
			}
		}
	}
	delete renderNodes;
}

V_RenderSystem::~V_RenderSystem()
{
}