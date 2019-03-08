/*
	Mapped manager for light components
*/

#pragma once
#include "MappedComponentManager.h"
#include "LightManager.h"

class LightManager :
	public MappedComponentManager
{
public:
	std::vector<light> *lights = new std::vector<light>();	//Vector of lights
	//Adds a light component based on entity ID
	void addComponent(int entityID) {
		entities->push_back(entityID);
		lights->push_back(light());
	}   
	//Sets a light component based on entity ID
	void setComponent(int entityID, light light_in) {
		lights->at(getEntityIndex(entityID)) = light_in;
	}
	std::vector<light> * getComponents() { return lights; };	//Returns the list of lights
	light getComponent(int entityID) { return lights->at(getEntityIndex(entityID)); };	//Returns the light component associated with the entity

	//Sets the type to the light component
	LightManager() {
		type = LIGHT_COMPONENT;
	};
	~LightManager();
};

