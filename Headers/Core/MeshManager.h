/*
	Managers for mesh components
*/

#pragma once
#include "MappedComponentManager.h"
#include "V_Components.h"
#include "V_Device.h"

class V_MeshManager :
	public MappedComponentManager
{
private:
	V_Device* device; //Vulkan device reference needed for cleanup
	std::vector<v_mesh> *meshes = new std::vector<v_mesh>();	//List of vulkan meshes
public:
	V_MeshManager() { type = V_MESH; };	//Sets the type to vulkan mesh

	//Standard mapped manager getters and setters
	void addComponent(int index) { entities->push_back(index); meshes->push_back(v_mesh()); };
	void setComponent(int index, v_mesh mesh);
	v_mesh getComponent(int entityID) { return meshes->at(getEntityIndex(entityID)); };

	void cleanup(V_Device* device_in);
	~V_MeshManager();
};

class GL_MeshManager : public MappedComponentManager {
public:
	GL_MeshManager() { type = GL_MESH; };
	void addComponent(int index) {};
	~GL_MeshManager();
};
