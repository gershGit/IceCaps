#pragma once
#include "Core/Main_Headers.h"
#include "glm/glm.hpp"
#include <vector>

//Enumerations
enum component_type {TRANSFORM, PARENT, AABB_COMPONENT,
						PREFAB_COMPONENT,
						PHYSICS, COLLIDER, RIGID_BODY,
						CAMERA, V_CAMERA, DX12_CAMERA, 
						SCRIPT,
						MESH_COMPONENT, MATERIAL_COMPONENT, SKINNED_MESH, V_SKINNED_MESH,
						V_MESH, V_MATERIAL, VULKAN_SCENE_NODE_COMPONENT,
						GL_MESH, GL_MATERIAL,
						V_DESCRIPTOR, DX12_DESCRIPTOR,
						LIGHT_COMPONENT,
						ANIMATION_COMPONENT, ARMATURE_COMPONENT, V_ARMATURE_COMPONENT, ARMATURE_ANIMATION,
						TAGS_COMPONENT,
						COLLISION, NO_TYPE};
enum system_type {RENDER_SYSTEM, RIGID_BODY_SYSTEM, COLLISION_SYSTEM, ANIMATION_SYSTEM, 
	INPUT_SYSTEM, CLIENT_NET_SYSTEM, SERVER_NET_SYSTEM, PEER_NET_SYSTEM, SCENE_TREE_SYSTEM, 
	BOUNDS_SYSTEM, ARMATURE_SYSTEM, NO_SYSTEM_TYPE = -1};
enum collision_state {COLLISION_ENTER, COLLISION_CONTINUE, COLLISION_EXIT};
enum anim_state {ANIMATION_PLAYING, ANIMATION_PAUSED, ANIMATION_OVER};

#define ICE_TAG_INPUT 0b001
#define ICE_TAG_FPS 0b010
#define ICE_TAG_MOVED 0b100

//Entity
struct entity {
	unsigned int id;
};

//Components
struct prefab {
	int baseEntity;
};

struct transform {
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rot = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);
};

struct bone {
	int id;
	transform poseTransform;
	glm::mat4 localBindTransform;
	glm::mat4 inverseBindTransform;
	bone* parent;
	std::vector<int> childrenIDs;
};

struct parentEntity {
	entity* parent;
};

struct rigid_body {
	float mass;
	glm::vec3 COM;
	bool is_grounded = false;
	glm::vec3 lastPosition;
	glm::vec3 lastVelocity;
	glm::vec3 lastRotation;
	glm::vec3 rotationalVelocity;
	float elasticity = 0.5f;
	bool isStatic = false;
};

struct accelerations {
	std::vector<glm::vec3> accelerationVectors;
};

struct aabb {
	glm::vec3 min;
	glm::vec3 max;
};

struct mesh_data {
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
};

struct box {
	glm::vec3 pointA;
	glm::vec3 pointB;
};

struct collider {
	collider_type type;
	bool CCD; //Is continuous ccd on
	float radius;
	aabb * boundingBox;
	box * hitBox;
	mesh_data * mesh;
};

struct sphere_collider : collider {
	float radius;
};

struct collision {
	bool collision;
	collision_state state;
	int entityA;
	int entityB;
	glm::vec3 collisionPointA;
	glm::vec3 collisionPointB;
	glm::vec3 collisionNormalA;
	glm::vec3 collisionNormalB;
};

struct camera {
	float fov;
	float near;
	float far;
	glm::vec3 lookDirection;
	glm::vec3 upDirection;
};

struct armature {
	bool onGPU = false;
	int maxWeights = 0;
	std::vector<glm::mat4> finalTransforms;
	std::vector<bone> bones;
};

struct vk_armature : armature {
	VkBuffer boneBuffer;
	VkDeviceMemory boneBufMem;
};

struct skinned_mesh {
	std::vector<skinned_vertex> originalVertices;
};

struct light {
	light_type lType;
	glm::vec4 color;
	float range;
};

struct snapshot {
	glm::vec3 deltaPosition;
	glm::vec3 deltaRotation;
	glm::vec3 deltaScale;
};

struct key_frame {
	double t;
	unsigned int index;
	snapshot snap;
};

struct armature_key {
	double t = 0.0;
	unsigned int index = 0;
	int* boneIDs = nullptr;  //Every bone affected by the animation must have a snapshot pre-interpolated for every key frame
	snapshot* snaps = nullptr;
};

struct animation {
	std::string name;
	anim_state state;
	float animationWeight;
	bool repeat;	
	bool playOnStartup = true;
	double startTime;
	double length;
	unsigned int frameCount;
	key_frame* frames; //Array of frames
	key_frame* lastFrameStart;
	key_frame* lastFrameEnd;
	key_frame lastCalulatedFrame;
};

struct armature_animation {
	std::string name;
	anim_state state;
	float animationWeight;
	bool repeat;
	bool playOnStartup = true;
	double startTime;
	double length;
	unsigned int frameCount;
	std::vector<int> boneIDs;
	armature_key* frames; //Array of frames
	armature_key* lastFrameStart;
	armature_key* lastFrameEnd;
	armature_key lastCalulatedFrame;
};

struct gl_material {

};

struct entity_tags {
	uint64_t tags; //Bitwise tags
};