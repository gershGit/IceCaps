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
#include "Core/RigidBodyManager.h"
#include "Core/ColliderManager.h"
#include "Core/CollisionManager.h"
#include "Core/AnimationManager.h"
#include "Core/TagManager.h"

#include "Vulkan/V_MeshFactory.h"
#include "Vulkan/V_MaterialFactory.h"
#include "Core/AnimationFactory.h"

//Manager Retrieval Functions ---------

TransformManager* getTransformManager(std::vector<ComponentManager*>& managers);
V_MeshManager* getVulkanMeshManager(std::vector<ComponentManager*>& managers);
V_MaterialManager* getVulkanMaterialManager(std::vector<ComponentManager*>& managers);
PrefabManager* getPrefabManager(std::vector<ComponentManager*> &managers);
CameraManager* getCameraManager(std::vector<ComponentManager*> &managers);
LightManager* getLightManager(std::vector<ComponentManager*> &managers);
RigidBodyManager* getRigidBodyManager(std::vector<ComponentManager*> &managers);
ColliderManager* getColliderManager(std::vector<ComponentManager*> &managers);
CollisionManager* getCollisionManager(std::vector<ComponentManager*> &managers);
AnimationManager* getAnimationManager(std::vector<ComponentManager*> &managers);
TagManager* getTagManager(std::vector<ComponentManager*> &managers);