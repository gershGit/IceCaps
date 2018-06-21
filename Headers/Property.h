/*
	Class for a property of a game object
*/

#pragma once
#include "Headers.h"

class GameObject;

//Type of property of the game object
enum pType { PHYSICS_BODY, LIGHT, CAMERA, DRAWABLE };

class Property {
public:
	//Store a reference to the game object
	GameObject * gameObject;

	//Stores the type of this property
	pType ptype;

	//Flags for what type of property it is
	bool lightFlag, cameraFlag, renderFlag;
};