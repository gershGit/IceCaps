/*
	Parent class for all entity systems
	Custom systems derive from this class
*/

#pragma once
#include "Core/Main_Headers.h"
#include <vector>
#include "Core/EntityComponents.h"
#include "Core/ComponentManager.h"
#include <iostream>

class EntitySystem
{
protected:
	//Vector that stores all the entity IDs that will be processed by the system
	std::vector<int> *entities = new std::vector<int>();
	configurationStructure * config = nullptr;
	SceneNode* scene = nullptr;
public:
	system_type systemType = NO_SYSTEM_TYPE;
	bool usesScene = false;

	std::vector<ComponentManager*> *managers = new std::vector<ComponentManager*>();//Managers needed by this system
	std::vector<component_type> operatesOn = std::vector<component_type>();			//Components that qualify for this system
	uint64_t requiredTags = 0;
	//A list of possible component combinations that meet the requirements to be used by the system
	std::vector<std::vector<component_type>> entityListRequiredComponents = std::vector<std::vector<component_type>>();
	EntitySystem() {};

	//Functions to be overridden by subclasses
	virtual void initialize() {};
	virtual void start() {};
	virtual void onUpdate() {};
	virtual void onConfigurationChange() {};
	virtual void addEntity(int entityID) { entities->push_back(entityID); };
	virtual void removeEntity(entity deadEntity) {};
	virtual void cleanup() {};

	void setScene(SceneNode* scene_in) { scene = scene_in; };
	void setConfig(configurationStructure &config_in) { config = &config_in; };	//Adds a reference to a configuration structure

	~EntitySystem() { delete entities; };
};

