#pragma once
#include "Core/Main_Headers.h"
#include "glm/glm.hpp"
#include <vector>

//Enumerations
enum colliderType {SPHERE, AABB, BOX, CAPSULE, MESH};
enum light_type {POINT_LIGHT, DIRECTION_LIGHT, SPOT_LIGHT};
enum component_type {TRANSFORM, PARENT,
						PREFAB_COMPONENT,
						PHYSICS, COLLIDER, RIGID_BODY,
						CAMERA, V_CAMERA, DX12_CAMERA, 
						SCRIPT,
						MESH_COMPONENT, MATERIAL_COMPONENT,
						V_MESH, V_MATERIAL,
						GL_MESH, GL_MATERIAL,
						V_DESCRIPTOR, DX12_DESCRIPTOR,
						LIGHT_COMPONENT,
						NO_TYPE};
enum system_type {RENDER_SYSTEM, RIGID_BODY_SYSTEM};

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

struct rigid_body {
	float mass;
	glm::vec3 COM;
	bool is_grounded = false;
	glm::vec3 lastPosition;
	glm::vec3 lastVelocity;
	float elasticity = 0.5f;
};

struct accelerations {
	std::vector<glm::vec3> accelerationVectors;
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