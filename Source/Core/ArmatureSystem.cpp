#include "Core/ArmatureSystem.h"
#include "Core/GameTimer.h"

void ArmatureSystem::initialize()
{
	armManager = dynamic_cast<MappedManager<armature>*>(getCManager<armature>(*managers, ARMATURE_COMPONENT));
	if (usingVulkan) {
		vSkinManager = dynamic_cast<MappedManager<vk_skinned_mesh>*>(getCManager<vk_skinned_mesh>(*managers, V_SKINNED_MESH));
	}
	else {
		skinManager = dynamic_cast<MappedManager<skinned_mesh>*>(getCManager<skinned_mesh>(*managers, SKINNED_MESH));
	}
}

void ArmatureSystem::start()
{
}

void ArmatureSystem::setVulkan(bool vulkanOn = true)
{
	usingVulkan = vulkanOn;
	if (vulkanOn) {
		operatesOn = { ARMATURE_COMPONENT, V_SKINNED_MESH }; entityListRequiredComponents = { { V_SKINNED_MESH, ARMATURE_COMPONENT } };
	}
	else {
		operatesOn = { ARMATURE_COMPONENT, SKINNED_MESH }; entityListRequiredComponents = { { SKINNED_MESH, ARMATURE_COMPONENT } };
	}
}

//CPU based armature animation
void ArmatureSystem::updateVulkanCurrentVertexLocation(vk_skinned_mesh& mesh, armature& arm)
{
	std::vector<vertex> newVertices = std::vector<vertex>(mesh.originalVertices.size());
	for (int i = 0; i < mesh.originalVertices.size(); i++) {
		newVertices.at(i) = mesh.originalVertices.at(i).mVertex;

		skinned_vertex originalVertex = mesh.originalVertices.at(i);
		glm::vec4 localPos = glm::vec4(0);

		for (int i = 0; i < arm.maxWeights; i++) {
			int boneID = originalVertex.bone_id[i];
			glm::mat4 boneTransform = arm.finalTransforms.at(boneID);
			glm::vec4 originalPos = glm::vec4(originalVertex.mVertex.position, 1.0f);
			glm::vec4 posePosition = boneTransform * originalPos;
			float bone_weight = originalVertex.bone_weight[i];
			localPos += posePosition * bone_weight;
		}
		newVertices.at(i).position = localPos;
	}

	//Skinned meshes use slower host visible buffers as they need to be updated every frame
	VkDeviceSize bufferSize = sizeof(vertex) * newVertices.size();

	void* data;
	vkMapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), mesh.vBufferVRAM, 0, bufferSize, 0, &data);
	memcpy(data, newVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(config->apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice(), mesh.vBufferVRAM);
}

void ArmatureSystem::applyTransformToBone(int boneID, armature& arm, glm::mat4 parentTransform)
{
	//Recursively apply transforms
	glm::mat4 pose = getTransformationMatrix( arm.bones.at(boneID).poseTransform );
	glm::mat4 currentTrans = parentTransform * pose;

	for (int i = 0; i < arm.bones.at(boneID).childrenIDs.size(); i++) {
		applyTransformToBone(arm.bones.at(boneID).childrenIDs.at(i), arm, currentTrans);
	}

	glm::mat4 finalTrans = currentTrans * arm.bones.at(boneID).inverseBindTransform;
	arm.finalTransforms.at(boneID) = finalTrans;
}

void ArmatureSystem::onUpdate()
{
	for (int i = 0; i < entities->size(); i++) {
		applyTransformToBone(0, *armManager->getComponentAddress(entities->at(i)), glm::mat4(1));
	}

	for (int i = 0; i < entities->size(); i++) {
		bool onGpu = armManager->getComponent(entities->at(i)).onGPU;
		if (!onGpu && usingVulkan) {
			updateVulkanCurrentVertexLocation(*vSkinManager->getComponentAddress(entities->at(i)), *armManager->getComponentAddress(entities->at(i)));
		}
	}
}
