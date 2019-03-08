#include "Core/ManagersFactories.h"

//Manager retrieval ------------------------------------------------------------------------

TransformManager* getTransformManager(std::vector<ComponentManager*>& managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == TRANSFORM) {
			return dynamic_cast<TransformManager*>(manager);
		}
	}
	return nullptr;
}
V_MeshManager* getVulkanMeshManager(std::vector<ComponentManager*>& managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == V_MESH) {
			return dynamic_cast<V_MeshManager*>(manager);
		}
	}
	return nullptr;
}
V_MaterialManager* getVulkanMaterialManager(std::vector<ComponentManager*>& managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == V_MATERIAL) {
			return dynamic_cast<V_MaterialManager*>(manager);
		}
	}
	return nullptr;
}
PrefabManager* getPrefabManager(std::vector<ComponentManager*> &managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == PREFAB_COMPONENT) {
			return dynamic_cast<PrefabManager*>(manager);
		}
	}
	return nullptr;
}
CameraManager* getCameraManager(std::vector<ComponentManager*> &managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == CAMERA) {
			return dynamic_cast<CameraManager*>(manager);
		}
	}
	return nullptr;
}
LightManager* getLightManager(std::vector<ComponentManager*> &managers) {
	for (ComponentManager* manager : managers) {
		if (manager->type == LIGHT_COMPONENT) {
			return dynamic_cast<LightManager*>(manager);
		}
	}
	return nullptr;
}