/*
	Manager that stores all transforms using an array
*/

#pragma once
#include "ComponentManager.h"
#include "EntityComponents.h"

class TransformManager :
	public ComponentManager
{
public:
	//Array and its size
	int size;
	transform *transformArray;
	glm::vec3 xAxis = glm::vec3(1, 0, 0);
	glm::vec3 yAxis = glm::vec3(0, 1, 0);
	glm::vec3 zAxis = glm::vec3(0, 0, 1);

//Functions ---------------------------------	
	TransformManager() { type = TRANSFORM; }; //Sets the type to transform components

	void setSize(int size_in); //Allocates the array
	void addComponent(int id) {};	//Reallocate the array to a larger size
	bool hasEntity(int id) { return id < size; };	//Special has entity function using the array size
	void setTransform(int id, transform transform_in) { transformArray[id] = transform_in; };	//Sets the transform using the entity id as index
	glm::mat4 getTransformMatrix(int entityID);	//Returns a matrix from a transform component
	void cleanup() { free(transformArray); };	//Frees the allocated memory
	~TransformManager();
};