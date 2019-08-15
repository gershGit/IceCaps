/*
	Class for loading scenes, primarily from .icap files
*/

#pragma once
#include "Vulkan/Vulkan_Headers.h"
#include "Core/ComponentManager.h"
#include "Core/ManagersFactories.h"
#include "Core/SystemsHeader.h"

#define BUF_SIZE 256

class SceneLoader
{
private:
	static char buffer[BUF_SIZE];
	static char keyString[BUF_SIZE];
	static char value[BUF_SIZE];
	static scene_key key;
	static char nT[1];
public:
	SceneLoader();

	static bool insertEntity(SceneNode * scene_node, int entityID, AABB * bounds, bool isDynamic, configurationStructure * config, int pipelineIndex, std::vector<NodeManager<VulkanSceneNode>*>* renderNodes, std::vector<std::vector<int>*>* staticTemp);
	static bool insertLight(SceneNode* scene_node, int entityID, LightObject light, float range, configurationStructure* config, int maxLights);
	static void allocateVulkanCommandObjects(std::vector<NodeManager<VulkanSceneNode>*>* renderNodes, configurationStructure* config);
	static void allocateLightBuffers(SceneNode* scene_node, int bufferSize, int childCount);
	static void placeLightsInScene(SceneNode* scene, MappedManager<light>* lManager, ArrayManager<transform>* tManager, configurationStructure* config, int maxLights);
	static void createChildren(SceneNode * scene_node, int currentDepth, int maxDepth, int childCount);
	static void setBounds(SceneNode * scene_node, int childCount);
	static void buildTree(configurationStructure & config, SceneNode * scene, FILE * fp);
	static void addManagers(configurationStructure & config, std::vector<ComponentManager*>& managers, FILE * fp);
	static void fillSystemWithManagers(EntitySystem * system, std::vector<ComponentManager*>& managers);
	static void fillSystemWithEntities(EntitySystem * system, std::vector<ComponentManager*>& managers, int entityCount);
	static void addSystems(configurationStructure & config, std::vector<EntitySystem*>& systems, std::vector<ComponentManager*>& managers, FILE * fp);
	static void addVulkanPipeline(configurationStructure & config, char * value, FILE * fp, std::vector<NodeManager<VulkanSceneNode>*>* renderNodes);
	static void addPipeline(configurationStructure & config, char * value, FILE * fp);
	static transform buildTransformComponent(FILE * fp);
	static v_mesh buildVulkanMeshComponent(FILE * fp, configurationStructure & config, AABB * bounds);
	static v_material buildVulkanMaterialComponent(FILE * fp, configurationStructure & config);
	static camera buildCamera(FILE * fp, configurationStructure & config);
	static v_camera buildVulkanCamera(FILE * fp, configurationStructure & config);
	static light buildLight(FILE * fp, configurationStructure & config);
	static rigid_body buildRigidBody(FILE * fp, configurationStructure & config);
	static collider buildCollider(FILE * fp, configurationStructure & config);
	static animation buildAnimation(FILE * fp, configurationStructure & config);
	static armature buildArmature(FILE* fp, configurationStructure& config);
	static armature_animation buildArmatureAnimation(FILE* fp, configurationStructure& config);
	static vk_skinned_mesh buildVulkanSkinnedMesh(FILE* fp, configurationStructure& config, AABB* bounds);
	static void loadVulkanEntity(int entityID, std::vector<ComponentManager*>& componentManagers, FILE * fp, configurationStructure & config, SceneNode * scene, std::vector<NodeManager<VulkanSceneNode>*>* renderNodes, std::vector<std::vector<int>*>* staticTemp);
	static void loadEntity(int entityID, configurationStructure & config, std::vector<ComponentManager*>& componentManagers, FILE * fp, SceneNode * scene);
	static void buildVulkanDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure & config, std::vector<NodeManager<VulkanSceneNode>*> * renderNodes);
	static void buildDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure & config);

	//Loads file info into component managers
	static void loadScene(int sceneIndex, configurationStructure &config, std::vector<ComponentManager*> &managers, std::vector<EntitySystem*> &systems, SceneNode* scene);
	//Unloads the scene from the managers and calls associated cleanup functions
	static void unloadScene(configurationStructure & config, std::vector<ComponentManager*>& managers, std::vector<EntitySystem*>& systems);

	~SceneLoader();
};

