/*
	Managers camera components using a map
*/

#pragma once
#include "MappedComponentManager.h"
#include "V_Components.h"

class CameraManager :
	public MappedComponentManager
{
public:
	//Vector filled with camera components
	std::vector<camera> *cameras = new std::vector<camera>();
	//Adds a camera component and an entity id to reference
	void addComponent(int id) {
		entities->push_back(id);
		cameras->push_back(camera());
	}
	//Sets a camera component based on an entity ID
	void setComponent(int id, camera camera) {
		cameras->at(getEntityIndex(id)) = camera;
	}
	//Returns the camera component associated with an entity ID
	std::vector<camera> * getComponents() { return cameras; };

	//Conctructure sets the type of manager to camera
	CameraManager() {
		type = CAMERA;
	};
	~CameraManager();
};

