/*
	Header file for combining all managers and factories for building components
	Includes getter functions to return managers from a vector
*/

#pragma once

#include "ComponentManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "TransformManager.h"
#include "PrefabManager.h"
#include "LightManager.h"
#include "CameraManager.h"

#include "V_MeshFactory.h"
#include "V_MaterialFactory.h"

//Manager Retrieval Functions ---------

TransformManager* getTransformManager(std::vector<ComponentManager*>& managers);
V_MeshManager* getVulkanMeshManager(std::vector<ComponentManager*>& managers);
V_MaterialManager* getVulkanMaterialManager(std::vector<ComponentManager*>& managers);
PrefabManager* getPrefabManager(std::vector<ComponentManager*> &managers);
CameraManager* getCameraManager(std::vector<ComponentManager*> &managers);
LightManager* getLightManager(std::vector<ComponentManager*> &managers);