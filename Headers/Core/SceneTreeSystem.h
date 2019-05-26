#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"

class SceneTreeSystem : public EntitySystem {
public:
	bool usingVulkan = false;
	MappedManager<int> * nodeIdManager = nullptr;
	ArrayManager<uint64_t> * entityTags = nullptr;
	ArrayManager<AABB> * boundsManager = nullptr;
	SceneTreeSystem() { systemType = SCENE_TREE_SYSTEM; };
	~SceneTreeSystem() {};

	//TODO move dynamic objects to new positions in vulkan map if needed
	//TODO Rebuild light buffers for each scene_node that has lost or gained a light
	void initialize();
	void start();

	void onUpdate();
	bool attemptPlace(SceneNode* scene_node, int entityID);
	bool attemptPlaceParent(SceneNode* scene_node, int entityID);
	void moveObjects(SceneNode* scene_node);
	void moveLights();
};
