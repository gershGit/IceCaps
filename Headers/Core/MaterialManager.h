/*
	Managers for materials based on the mapped component manager
*/

#pragma once
#include "Core/MappedComponentManager.h"
#include "Vulkan/Vulkan_Headers.h"
#include "Vulkan/V_Components.h"

class V_Device;

class V_MaterialManager : public MappedComponentManager
{
private:
	std::vector<v_material> *materials = new std::vector<v_material>();	//Vector of materials
public:
	V_MaterialManager() { type = V_MATERIAL; };	//Sets the type to vulkan material

	//Standard getters and setters for mapped component managers
	v_material* getEntity(int entityID) { return &materials->at(getEntityIndex(entityID)); };
	void addComponent(int index) { entities->push_back(index); materials->push_back(v_material()); };
	void setComponent(int entityID, v_material material) { materials->at(getEntityIndex(entityID)) = material; }
	std::vector<v_material> * getComponents() { return materials; };

	//Returns the number of total images in the manager, used for descriptor pool creation
	int getImageCount();
	
	void cleanup(V_Device* device_in);
	~V_MaterialManager();
};

class GL_MaterialManager : public MappedComponentManager
{
public:
	GL_MaterialManager() { type = GL_MATERIAL; };
	void addComponent(int index) {};
	~GL_MaterialManager();
};