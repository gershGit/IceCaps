#include "Core/SceneTreeSystem.h"
#include "Core/ManagersFactories.h"
#include "Core/StringTranslation.h"
#include "Core/Global_Callbacks.h"
#include "Core/ThreadPool.h"

#define NO_LIGHT -1

//Constructor fills out entity system information
SceneTreeSystem::SceneTreeSystem()
{
	operatesOn = { TRANSFORM, PREFAB_COMPONENT, AABB_COMPONENT, TAGS_COMPONENT, V_MATERIAL, LIGHT_COMPONENT };
	entityListRequiredComponents = { {TRANSFORM, AABB_COMPONENT} };
	systemType = SCENE_TREE_SYSTEM;
}

//Set the managers and flag that it uses the scene tree
void SceneTreeSystem::initialize()
{
	entityTags = dynamic_cast<ArrayManager<uint64_t>*>(getCManager<uint64_t>(*managers, TAGS_COMPONENT) );
	boundsManager = dynamic_cast<ArrayManager<AABB>*>(getCManager<AABB>(*managers, AABB_COMPONENT));
	lightsManager = dynamic_cast<MappedManager<light>*>(getCManager<light>(*managers, LIGHT_COMPONENT));
	tManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	if (usingVulkan) {
		vMaterialManager = dynamic_cast<MappedManager<v_material>*>(getCManager<v_material>(*managers, V_MATERIAL));
	}
	usesScene = true;
}

//Empty start function
void SceneTreeSystem::start()
{
}

//On update move objects and lights to the correct scene nodes
void SceneTreeSystem::onUpdate()
{
	//Move all flagged objects
	moveObjects(*scene);

	//Check to see if any lights have been moved
	std::vector<int> lights = lightsManager->getEntities();
	for (int i = 0; i < lights.size(); i++) {
		if (hasTag(entityTags->getComponent(lights.at(i)), static_cast<uint64_t>(ICE_TAG_MOVED))) {
			auto lightFunc = std::bind(&SceneTreeSystem::moveLights, this, std::ref(*scene), lights.at(i));
			ThreadPool::submitJob(lightFunc);
		}
	}

	ThreadPool::workToComplete();
}

//Attempt to place an entity into a ndoe
int SceneTreeSystem::attemptPlace(SceneNode * scene_node, int entityID)
{
	//Check if the entity is inside the node
	if (isInside(&scene_node->bounds, boundsManager->getComponentAddress(entityID))) {
		//Recursively check if the entity is inside the nodes children
		if (!scene_node->isLeaf) {
			for (int i = 0; i < config->sceneChildren; i++) {
				if (attemptPlace(&scene_node->children[i], scene_node->dynamicEntities->at(i))) {
					return scene_node->children[i].id;
				}
			}
		}
		//Add the entity to the node
		scene_node->dynamicEntities->push_back(entityID);
		return scene_node->id;
	}
	return -1;
}

//Attempt to place in the parent of the called node
int SceneTreeSystem::attemptPlaceParent(SceneNode * scene_node, int entityID)
{
	//Check to see if this is the root of the tree
	if (scene_node == nullptr) {
		std::cout << "No longer in scene area" << std::endl;
		return -1;
	}
	//If in parent check if in children
	if (isInside(&scene_node->bounds, boundsManager->getComponentAddress(entityID))) {
		if (!scene_node->isLeaf) {
			for (int i = 0; i < config->sceneChildren; i++) {
				if (attemptPlace(&scene_node->children[i], entityID)) {
					return scene_node->children[i].id;
				}
			}
		}
		scene_node->dynamicEntities->push_back(entityID);
		return scene_node->id;
	}
	else {
		//Move even higher in the tree
		attemptPlaceParent(getParentNode(scene, scene_node->id, config->sceneChildren), entityID);
	}
	
	return -1;
}

//Removes the entity from the old node and adds it to the new node, creating a node if necessary
void SceneTreeSystem::moveInVulkanScene(int oldID, int newID, int entityID) {
	//Move in the vulkan scene as well
	if (usingVulkan && vMaterialManager->hasEntity(entityID)) {
		material_type mType = vMaterialManager->getComponent(entityID).matType;
		for (int i = 0; i < renderNodes->size(); i++) {
			if (renderNodes->at(i)->pipelineType == mType) {
				//Remove the entity from the old node
				VulkanSceneNode* nodeWithEntity = renderNodes->at(i)->getComponentAddress(oldID);
				std::vector<int>* currentNodeVector = &nodeWithEntity->dynamicEntities;
				std::vector<int>::iterator it = std::find(currentNodeVector->begin(), currentNodeVector->end(), entityID);
				currentNodeVector->erase(it);

				//Add the entity to the new vulkan scene node
				renderNodes->at(i)->getComponentAddress(newID)->dynamicEntities.push_back(entityID);
			}
		}
	}
}

//Refreshes the GPU with the new light information
void SceneTreeSystem::rebuildLightBuffer(VulkanSceneNode* node) {
	LightObject* tempLights = (LightObject*)malloc(sizeof(LightObject) * node->lightMax);
	VkDeviceSize lightBufferSize = sizeof(LightObject) * node->lightMax;
	LightObject tempLight;
	for (int i = 0; i < node->lightMax; i++) {
		if (node->lightIDs[i] == -1) {
			tempLight.color = glm::vec4(0.0);
			tempLight.position = glm::vec4(0.0);
		}
		else {
			tempLight.color = lightsManager->getComponent(node->lightIDs[i]).color;
			tempLight.position = glm::vec4(tManager->getComponent(node->lightIDs[i]).pos, 1.0f);
		}
		tempLights[i] = tempLight;
	}

	for (int k = 0; k < config->apiInfo.v_Instance->currentFrame; k++) {
		void* data;
		vkMapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), node->lightBufferVRAM[k], 0, lightBufferSize, 0, &data);
		memcpy(data, &tempLights, lightBufferSize);
		vkUnmapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), node->lightBufferVRAM[k]);
	}
	free(tempLights);
}

//Removes a lightID from a vulkan scene node
void SceneTreeSystem::removeLightInVulkan(int oldID, int lightID) {
	//Search for the nodes that need to remove this light
	for (int i=0; i<renderNodes->size(); i++) {
		VulkanSceneNode* node = renderNodes->at(i)->getComponentAddress(oldID);

		//Find the index of the light in the array and set the value to -1 (No light)
		int index = findInt(node->lightIDs, lightID, node->lightMax);
		if (index != NO_LIGHT) {
			node->lightCountCurrent--;
			node->lightIDs[i] = NO_LIGHT;
		}
		rebuildLightBuffer(node);
	}	
}

//Adds a light id to a vulkan node in only if a free position exists in the array
void SceneTreeSystem::addLightInVulkan(int newID, int lightID) {
	//Go through all pipelines that could need to add the id to
	for (int i = 0; i < renderNodes->size(); i++) {
		VulkanSceneNode* n = renderNodes->at(i)->getComponentAddress(newID);

		if (n->lightCountCurrent < n->lightMax) {
			for (int i = n->lightMax - 1; i >= 0; i--) {
				if (n->lightIDs[i] == NO_LIGHT) { //The position is open in the array
					n->lightIDs[i] = lightID;
					n->lightCountCurrent++;
					rebuildLightBuffer(n);
				}
			}
		}		
	}
}

//Go through all objects flagged as moved and see if they need to be in a new node
void SceneTreeSystem::moveObjects(SceneNode& scene_node)
{
	//Loop through all movable entities in the node
	for (int i = 0; i < scene_node.dynamicEntities->size(); i++) {
		int eID = scene_node.dynamicEntities->at(i);
		uint64_t currentTag = entityTags->getComponent(eID);
		//Check if the object has moved, if not move on
		if (hasTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED))) {
			//If the object is still inside this node then check if it can be pushed into a child node
			if (isInside(&scene_node.bounds, 
				boundsManager->getComponentAddress(eID)) ) {
				if (!scene_node.isLeaf) {
					for (int j = 0; j < config->sceneChildren; j++) {
						if (attemptPlace(&scene_node.children[j], eID)) {
							//If the object is succesfully placed into a child it can be removed from the parent
							scene_node.dynamicEntities->erase(scene_node.dynamicEntities->begin() + i);
							if (usingVulkan) {
								moveInVulkanScene(scene_node.id, scene_node.children[j].id, eID);
							}
							break;
						}
					}
				}
				//Remove the moved tage as the objects movement has been dealt with
				entityTags->setComponent(eID, removeTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED)));
				break;
			}
			//If the object is no longer in the same node, see which parent node it should now be in
			int foundNodeId = attemptPlaceParent(getParentNode(scene, scene_node.id, config->sceneChildren), scene_node.dynamicEntities->at(i));
			entityTags->setComponent(eID, removeTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED)));
			scene_node.dynamicEntities->erase(scene_node.dynamicEntities->begin() + i);
			if (usingVulkan && foundNodeId != -1) {
				moveInVulkanScene(scene_node.id, foundNodeId, eID);
			}
		}
	}
	//Recursively call the function to move all objects in nodes children as well
	if (!scene_node.isLeaf) {
		for (int i = 0; i < config->sceneChildren; i++) {
			auto moveFunc = std::bind(&SceneTreeSystem::moveObjects, this, std::ref(scene_node.children[i]));
			ThreadPool::submitJob(moveFunc);
		}
	}
}

//Go through all lights flagged as moved and see which nodes they now affect
void SceneTreeSystem::moveLights(SceneNode& scene_node, int lightID)
{
	light mLight = lightsManager->getComponent(lightID);
	LightObject tempLight;
	tempLight.position = glm::vec4(tManager->getComponent(lightID).pos, 1.0f);
	bool lightFound = false;
	for (int i = 0; i < scene_node.lightCount; i++) {	
		//If the light already affects this node then we don't need to worry about removal or addition
		if (scene_node.lightIDs->at(i) == lightID && lightAffects(scene_node.bounds, tempLight, mLight.range)) {
			lightFound = true;
			break;
		}
		else if (scene_node.lightIDs->at(i) == lightID) {
			scene_node.lightIDs->erase(scene_node.lightIDs->begin() + i);
			if (usingVulkan) {
				removeLightInVulkan(scene_node.id, lightID);
			}
			break;
		}
	}
	//If the light wasn't currently in the node but now is then it needs to be added
	if (!lightFound && lightAffects(scene_node.bounds, tempLight, mLight.range)) {
		scene_node.lightIDs->push_back(lightID);

		if (usingVulkan) {
			addLightInVulkan(scene_node.id, lightID);
		}
	}
	//Recursively call the function on children for granularity on nodes
	if (!scene_node.isLeaf) {
		for (int i = 0; i < config->sceneChildren; i++) {
			auto mLightsF = std::bind(&SceneTreeSystem::moveLights, this, std::ref(scene_node.children[i]), lightID);
			ThreadPool::submitJob(mLightsF);
		}
	}
}
