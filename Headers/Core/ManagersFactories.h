/*
	Header file for combining all managers and factories for building components
	Includes getter functions to return managers from a vector
*/

#pragma once

#include "Core/ComponentManager.h"

#include "Vulkan/V_MeshFactory.h"
#include "Vulkan/V_MaterialFactory.h"
#include "Core/AnimationFactory.h"
#include "Core/ArmatureFactory.h"

//Manager Retrieval Functions ---------

template<typename CType>
inline CManager<CType>* getCManager(std::vector<ComponentManager*>& managers, component_type typeOf)
{
	for (ComponentManager* manager : managers) {
		if (manager->componentType == typeOf) {
			return dynamic_cast<CManager<CType>*>(manager);
		}
	}
	return nullptr;
}

//Special manager functions
int countImages(MappedManager<v_material> * vulkanMaterialManager);

//Manager Cleanup Functions

template<typename CType>
inline void managerCleanup(ArrayManager<CType>* manager) {
	free(manager->componentArray);
}

void managerCleanup(MappedManager<v_material>* manager, V_Device* device);
void managerCleanup(MappedManager<v_mesh>* manager, V_Device* device);
void managerCleanup(MappedManager<vk_skinned_mesh>* manager, V_Device* device);
void managerCleanup(MappedManager<v_camera>* manager, V_Device* device, int bufferCount);