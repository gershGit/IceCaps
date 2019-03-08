/*
* Parent class for all component managers
*/
#include "EntityComponents.h"

#pragma once
class ComponentManager
{
public:
	component_type type = NO_TYPE;
	ComponentManager();

	//Required functions for subclasses to override
	virtual void addComponent(int id) {};
	virtual bool hasEntity(int id) { return false; };
	virtual void cleanup() {};
	~ComponentManager();
};