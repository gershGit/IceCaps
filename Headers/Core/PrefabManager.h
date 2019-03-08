/*
	Manager for prefab components
*/

#pragma once
#include "MappedComponentManager.h"
class PrefabManager :
	public MappedComponentManager
{
private:
	std::vector<prefab> * prefabs = new std::vector<prefab>(); //List of prefab components
public:
	PrefabManager() {type = PREFAB_COMPONENT;}; //Sets the type to prefab

	//Special getter function that finds the entity index based on prefab information
	int getEntityIndex(int entityID);

	//Standard mapped managers getters and setters
	void addComponent(int entityID) { entities->push_back(entityID); prefabs->push_back(prefab()); };
	void setComponent(int entityID, prefab prefab_in) { prefabs->at(getEntityIndex(entityID)) = prefab_in; }
	~PrefabManager();
};

