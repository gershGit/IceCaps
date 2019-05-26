#include "Core/SceneTreeSystem.h"
#include "Core/ManagersFactories.h"
#include "Core/StringTranslation.h"
#include "Core/Global_Callbacks.h"

void SceneTreeSystem::initialize()
{
	entityTags = dynamic_cast<ArrayManager<uint64_t>*>( getCManager<uint64_t>(*managers, TAGS_COMPONENT) );
	boundsManager = dynamic_cast<ArrayManager<AABB>*>(getCManager<AABB>(*managers, AABB_COMPONENT));
}

void SceneTreeSystem::start()
{
}

void SceneTreeSystem::onUpdate()
{
	moveObjects(scene);
}

bool SceneTreeSystem::attemptPlace(SceneNode * scene_node, int entityID)
{
	if (isInside(&scene_node->bounds, boundsManager->getComponentAddress(entityID))) {
		if (!scene_node->isLeaf) {
			for (int i = 0; i < config->sceneChildren; i++) {
				if (attemptPlace(&scene_node->children[i], scene_node->dynamicEntities->at(i))) {
					return true;
				}
			}
		}
		scene_node->dynamicEntities->push_back(entityID);
		return true;
	}
	return false;
}

bool SceneTreeSystem::attemptPlaceParent(SceneNode * scene_node, int entityID)
{
	//If in parent check if in children
	if (isInside(&scene_node->bounds, boundsManager->getComponentAddress(entityID))) {
		if (!scene_node->isLeaf) {
			for (int i = 0; i < config->sceneChildren; i++) {
				if (attemptPlace(&scene_node->children[i], scene_node->dynamicEntities->at(i))) {
					return true;
				}
			}
		}
		scene_node->dynamicEntities->push_back(entityID);
		return true;
	}
	else {
		//Move even higher in the tree
		attemptPlaceParent(getParentNode(scene, scene_node->id, config->sceneChildren), entityID);
	}
	
	return false;
}

void SceneTreeSystem::moveObjects(SceneNode* scene_node)
{
	for (int i = 0; i < scene_node->dynamicEntities->size(); i++) {
		uint64_t currentTag = entityTags->getComponent(scene_node->dynamicEntities->at(i));
		if (hasTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED))) {
			if (isInside(&scene_node->bounds, boundsManager->getComponentAddress(scene_node->dynamicEntities->at(i)))) {
				if (!scene_node->isLeaf) {
					for (int i = 0; i < config->sceneChildren; i++) {
						if (attemptPlace(&scene_node->children[i], scene_node->dynamicEntities->at(i))) {
							scene_node->dynamicEntities->erase(scene_node->dynamicEntities->begin() + i);
							break;
						}
					}
				}
				entityTags->setComponent(scene_node->dynamicEntities->at(i), removeTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED)));
				break;
			}
			attemptPlaceParent(getParentNode(scene, scene_node->id, config->sceneChildren), scene_node->dynamicEntities->at(i));
			scene_node->dynamicEntities->erase(scene_node->dynamicEntities->begin() + i);
			entityTags->setComponent(scene_node->dynamicEntities->at(i), removeTag(currentTag, static_cast<uint64_t>(ICE_TAG_MOVED)));
		}
	}
}
