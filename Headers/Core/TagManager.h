/*
	Manager that stores all tags using an array
*/

#pragma once
#include "ComponentManager.h"
#include "EntityComponents.h"

class TagManager :
	public ComponentManager
{
public:
	//Array and its size
	int size;
	entity_tags *tagArray;

	//Functions ---------------------------------	
	TagManager() { type = TAGS_COMPONENT; }; //Sets the type to tags components

	void setSize(int size_in) { 
		tagArray = (entity_tags*)malloc(sizeof(entity_tags)*size_in); 
		size = size_in; 
		for (int i = 0; i < size_in; i++) {
			tagArray[i].tags = 0;
		}
	}; //Allocates the array
	void addComponent(int id) {};	//Reallocate the array to a larger size
	bool hasEntity(int id) { return id < size; };	//Special has entity function using the array size
	void addTag(int id, uint64_t newTag) { tagArray[id].tags = tagArray[id].tags | newTag; };
	void setTags(int id, entity_tags tags_in) { tagArray[id] = tags_in; };	//Sets the tags using the entity id as index
	bool hasTag(int entityID, uint64_t tag) { return tag & tagArray[entityID].tags; }	//Perfomrs bitwise and on the tags
	void cleanup() { free(tagArray); };	//Frees the allocated memory
	~TagManager() {};
};