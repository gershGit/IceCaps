#include "Core/ArmatureSystem.h"

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

	//TODO split into threadpool calls?
	for (int i = 0; i < mesh.originalVertices.size(); i++) {
		glm::vec4 localPos = glm::vec4(0);

		for (int i = 0; i < arm.maxWeights; i++) {
			glm::mat4 boneTransform = arm.finalTransforms.at( mesh.originalVertices.at(i).bone_id[i] );
			glm::vec4 posePosition = boneTransform * glm::vec4(mesh.originalVertices.at(i).mVertex.position, 1.0f);
			localPos += posePosition * mesh.originalVertices.at(i).bone_weight[i];
		}
		newVertices.at(i).position = localPos;
		newVertices.at(i).normal = mesh.originalVertices.at(i).mVertex.normal;
		newVertices.at(i).tangent = mesh.originalVertices.at(i).mVertex.tangent;
		newVertices.at(i).uv = mesh.originalVertices.at(i).mVertex.uv;
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
