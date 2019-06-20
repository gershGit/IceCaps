/*
	Tree system that updates entity and light locations in the scene tree every frame
	This is used after the objects have been moved but prior to rendering so they can be easily culled
*/

#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"
#include "Vulkan/Vulkan_Headers.h"
#include "Vulkan/V_Components.h"

class SceneTreeSystem : public EntitySystem {
public:
	//Managers that may be needed as well as the render nodes for a vulkan scene
	bool usingVulkan = false;
	MappedManager<int> * nodeIdManager = nullptr;
	ArrayManager<transform>* tManager = nullptr;
	ArrayManager<uint64_t> * entityTags = nullptr;
	ArrayManager<AABB> * boundsManager = nullptr;
	MappedManager<light>* lightsManager = nullptr;
	MappedManager<v_material>* vMaterialManager = nullptr;
	std::vector<NodeManager<VulkanSceneNode>* >* renderNodes = nullptr;

	SceneTreeSystem();
	~SceneTreeSystem() {};

	//Entity System functions
	void initialize();
	void start();
	void onUpdate();

	//Functions called for updating the scene
	int attemptPlace(SceneNode* scene_node, int entityID);
	int attemptPlaceParent(SceneNode* scene_node, int entityID);
	void moveInVulkanScene(int oldID, int newID, int entityID);
	void rebuildLightBuffer(VulkanSceneNode* node);
	void removeLightInVulkan(int oldID, int lightID);
	void addLightInVulkan(int newID, int lightID);
	void moveObjects(SceneNode* scene_node);
	void moveLights(SceneNode* scene_node, int lightID);

	//Setter function for the render nodes to access the vulkan scene
	void setRenderNodes(std::vector<NodeManager<VulkanSceneNode>*>* renderNodes_in) { renderNodes = renderNodes_in; };
};
