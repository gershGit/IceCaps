/*
	Manages collider components using a map
*/

#pragma once
#include "Core/MappedComponentManager.h"

class ColliderManager :
	public MappedComponentManager
{
private:
	std::vector<collider>* components = new std::vector<collider>();
public:
	ColliderManager() { type = COLLIDER; };
	~ColliderManager() {};

	void addComponent(int entityID) { entities->push_back(entityID); components->push_back(collider()); };
	void setComponent(int entityID, collider componentIn) { components->at(getEntityIndex(entityID)) = componentIn; };
	collider getComponent(int entityID) { return components->at(getEntityIndex(entityID)); };
};