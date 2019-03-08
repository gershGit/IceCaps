#pragma once
#include "Core/Main_Headers.h"
#include "glm/glm.hpp"
#include <vector>

//Enumerations
enum colliderType {SPHERE, AABB, BOX, CAPSULE, MESH};
enum light_type {POINT_LIGHT, DIRECTION_LIGHT, SPOT_LIGHT};
enum component_type {TRANSFORM, PARENT,
						PREFAB_COMPONENT,
						PHYSICS, COLLIDER, 
						CAMERA, V_CAMERA, DX12_CAMERA, 
						SCRIPT,
						MESH_COMPONENT, MATERIAL_COMPONENT,
						V_MESH, V_MATERIAL,
						GL_MESH, GL_MATERIAL,
						V_DESCRIPTOR, DX12_DESCRIPTOR,
						LIGHT_COMPONENT,
						NO_TYPE};
enum system_type {RENDERER};

//Entity
struct entity {
	unsigned int id;
};

//Components
struct prefab {
	int baseEntity;
};

struct transform {
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};

struct bone {
	transform mTransformation;
	bone* parent;
};

struct parentEntity {
	entity* parent;
};

struct physics {
	unsigned int mass;
	glm::mat4 COM;
};

struct collider {
	colliderType type;
	bool CCD; //Is continuous ccd on
};

struct camera {
	float fov;
	float near;
	float far;
	glm::vec3 lookDirection;
	glm::vec3 upDirection;
};

struct armature {
	std::vector<bone> bones;
};

struct skinned_mesh {
	std::vector<skinned_vertex> originalVertices;
};

struct light {
	light_type lType;
	glm::vec4 color;
};