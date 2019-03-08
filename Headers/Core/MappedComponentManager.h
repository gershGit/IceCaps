/*
	Parent class for managers that use maps instead of arrays
	!Possibly unusable based on how maps work
*/

#pragma once
#include "Core/ComponentManager.h"
class MappedComponentManager :
	public ComponentManager
{
protected:
	std::vector<int> *entities = new std::vector<int>();	//Entity list
public:
	//Functions based solely on the entity list vector
	int getEntityIndex(int entityID) {
		for (int i = 0; i < entities->size(); i++) {
			if (entities->at(i) == entityID) {
				return i;
			}
		}
		return -1;
	};
	bool hasEntity(int entityID) {
		for (int i = 0; i < entities->size(); i++) {
			if (entities->at(i) == entityID) {
				return true;
			}
		}
		return false;
	};
	int getSize() { return (int)entities->size(); }
	std::vector<int> * getEntities() { return entities; };
	int getEntityAt(int index) { return entities->at(index); }

	MappedComponentManager();
	~MappedComponentManager();
};

