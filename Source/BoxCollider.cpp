#include "..\Headers\BoxCollider.h"
#include "SphereCollider.h"


BoxCollider::BoxCollider()
{
}


BoxCollider::~BoxCollider()
{
}

collisionInfo BoxCollider::checkCollision(SphereCollider * collider)
{
	//AABB only atm
	glm::vec3 closest_point = glm::vec3(0);
	if (collider->position.x < position.x - scale.x / 2) {
		closest_point.x = position.x - scale.x / 2;
	}
	else if (collider->position.x > position.x + scale.x / 2) {
		closest_point.x = position.x + scale.x / 2;
	}
	else {
		closest_point.x = collider->position.x;
	}

	if (collider->position.y < position.y - scale.y / 2) {
		closest_point.y = position.y - scale.y / 2;
	}
	else if (collider->position.y > position.y + scale.y / 2) {
		closest_point.y = position.y + scale.y / 2;
	}
	else {
		closest_point.y = collider->position.y;
	}

	if (collider->position.z < position.z - scale.z / 2) {
		closest_point.z = position.z - scale.z / 2;
	}
	else if (collider->position.z > position.z + scale.z / 2) {
		closest_point.z = position.z + scale.z / 2;
	}
	else {
		closest_point.z = collider->position.z;
	}

	if (glm::length(closest_point - collider->position) < collider->radius) {
		collisionInfo info = collisionInfo();
		info.collision = true;
		info.collisionObject = collider->object;
		info.collisionPosition = closest_point;
		return info;
	}
	return collisionInfo();
}

collisionInfo BoxCollider::checkCollision(BoxCollider * collider)
{
	//AABB only atm
	glm::vec3 closest_point = glm::vec3(0);
	if (collider->position.x + collider->scale.x/2 < position.x - scale.x / 2) {
		closest_point.x = position.x - scale.x / 2;
	}
	else if (collider->position.x - collider->scale.x/2 > position.x + scale.x / 2) {
		closest_point.x = position.x + scale.x / 2;
	}
	else {
		closest_point.x = -99;
	}

	if (collider->position.y + collider->scale.y / 2 < position.y - scale.y / 2) {
		closest_point.y = position.y - scale.y / 2;
	}
	else if (collider->position.y - collider->scale.y / 2 > position.y + scale.y / 2) {
		closest_point.y = position.y + scale.y / 2;
	}
	else {
		closest_point.y = -99;
	}

	if (collider->position.z + collider->scale.z / 2 < position.z - scale.z / 2) {
		closest_point.z = position.z - scale.z / 2;
	}
	else if (collider->position.z - collider->scale.z / 2 < position.z + scale.z / 2) {
		closest_point.z = position.z + scale.z / 2;
	}
	else {
		closest_point.z = -99;
	}

	//TODO fix this
	if (closest_point.x != -99 && closest_point.y != -99 && closest_point.z != -99) {
		collisionInfo info = collisionInfo();
		info.collision = true;
		info.collisionObject = collider->object;
		info.collisionPosition = closest_point;
		return info;
	}
	return collisionInfo();
}

collisionInfo BoxCollider::checkCollision(MeshCollider * collider)
{
	return collisionInfo();
}
