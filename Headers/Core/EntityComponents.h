#pragma once
#include "Core/Main_Headers.h"
#include "glm/glm.hpp"
#include <vector>

//Enumerations
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
						COLLISION, NO_TYPE};
enum system_type {RENDER_SYSTEM, RIGID_BODY_SYSTEM, COLLISION_SYSTEM, NO_SYSTEM_TYPE = -1};
enum collision_state {COLLISION_ENTER, COLLISION_CONTINUE, COLLISION_EXIT};

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
	std::vector<bone> bones;
};

struct skinned_mesh {
	std::vector<skinned_vertex> originalVertices;
};

struct light {
	light_type lType;
	glm::vec4 color;
};