#include "..\Headers\SphereCollider.h"
#include "BoxCollider.h"

SphereCollider::SphereCollider()
{
}

SphereCollider::~SphereCollider()
{
}

//Sphere-Sphere collision
collisionInfo SphereCollider::checkCollision(SphereCollider * collider)
{
	glm::vec3 closest_point = collider->position + glm::normalize(position - collider->position)*collider->radius;
	glm::vec3 vector_from_edge = closest_point - position;
	float distance_from_edge = glm::length(vector_from_edge);
	if (distance_from_edge < radius) {
		collisionInfo collision;
		collision.collision = true;
		collision.collisionObject = collider->object;
		std::cout << "Collision!!!" << std::endl;
		return collision;
	}
	return collisionInfo();
}

//Sphere-AABB collision
collisionInfo SphereCollider::checkCollision(BoxCollider * collider)
{
	//AABB only atm
	glm::vec3 closest_point = glm::vec3(0);
	if (position.x < collider->position.x - collider->scale.x / 2) {
		closest_point.x = collider->position.x - collider->scale.x / 2;
	}
	else if (position.x > collider->position.x + collider->scale.x / 2) {
		closest_point.x = collider->position.x + collider->scale.x / 2;
	}
	else {
		closest_point.x = position.x;
	}

	if (position.y < collider->position.y - collider->scale.y / 2) {
		closest_point.y = collider->position.y - collider->scale.y / 2;
	}
	else if (position.y > collider->position.y + collider->scale.y / 2) {
		closest_point.y = collider->position.y + collider->scale.y / 2;
	}
	else {
		closest_point.y = position.y;
	}

	if (position.z < collider->position.z - collider->scale.z / 2) {
		closest_point.z = collider->position.z - collider->scale.z / 2;
	}
	else if (position.z > collider->position.z + collider->scale.z / 2) {
		closest_point.z = collider->position.z + collider->scale.z / 2;
	}
	else {
		closest_point.z = position.z;
	}

	if (glm::length(closest_point - position) < radius) {
		collisionInfo info = collisionInfo();
		info.collision = true;
		info.collisionObject = collider->object;
		info.collisionPosition = closest_point;
		return info;
	}
	return collisionInfo();
}

//Sphere-Mesh collision
collisionInfo SphereCollider::checkCollision(MeshCollider * collider)
{
	return collisionInfo();
}