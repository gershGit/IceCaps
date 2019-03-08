/*
	Header file for combining all managers and factories for building components
	Includes getter functions to return managers from a vector
*/

#pragma once

#include "Core/ComponentManager.h"
#include "Core/MeshManager.h"
#include "Core/MaterialManager.h"
#include "Core/TransformManager.h"
#include "Core/PrefabManager.h"
#include "Core/LightManager.h"
#include "Core/CameraManager.h"

#include "Vulkan/V_MeshFactory.h"
#include "Vulkan/V_MaterialFactory.h"

//Manager Retrieval Functions ---------

TransformManager* getTransformManager(std::vector<ComponentManager*>& managers);
V_MeshManager* getVulkanMeshManager(std::vector<ComponentManager*>& managers);
V_MaterialManager* getVulkanMaterialManager(std::vector<ComponentManager*>& managers);
PrefabManager* getPrefabManager(std::vector<ComponentManager*> &managers);
CameraManager* getCameraManager(std::vector<ComponentManager*> &managers);
LightManager* getLightManager(std::vector<ComponentManager*> &managers);