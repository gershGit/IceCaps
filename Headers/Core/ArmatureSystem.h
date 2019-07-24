/*
	System to run animations and update entities accordingly
*/

#pragma once
#include "Core/EntitySystem.h"
#include "Core/ManagersFactories.h"

class ArmatureSystem : public EntitySystem {
private:
	MappedManager<armature>* armManager = nullptr;
	MappedManager<vk_skinned_mesh>* vSkinManager = nullptr;
	MappedManager<skinned_mesh>* skinManager = nullptr;
public:
	bool usingVulkan = false;
	ArmatureSystem() { operatesOn = { ARMATURE_COMPONENT, V_SKINNED_MESH }; entityListRequiredComponents = { { SKINNED_MESH, ARMATURE_COMPONENT}, { V_SKINNED_MESH, ARMATURE_COMPONENT } }; systemType = ARMATURE_SYSTEM; };
	~ArmatureSystem() {};
	void initialize();
	void start();

	void setVulkan(bool vulkanOn);
	void updateCurrentVertexLocation(skinned_mesh& mesh, armature& arm);
	void updateVulkanCurrentVertexLocation(vk_skinned_mesh& mesh, armature& arm);
	void applyTransformToBone(int boneID, armature& arm, glm::mat4 parentTransform);
	void applyTransformToBone(bone& currentBone, armature& arm, glm::mat4 parentTransform);
	void onUpdate();
};