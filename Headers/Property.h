#pragma once
#include "Headers.h"

class GameObject;

enum pType {PHYSICS_BODY, LIGHT, CAMERA, DRAWABLE};

class Property {
public:
	GameObject * gameObject;
	pType ptype;
	bool lightFlag, cameraFlag, renderFlag;
};