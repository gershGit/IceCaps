/*
	Class to load in an entire scene
	To be used in place of the loadScene() function in main.cpp
*/

#pragma once
#include "Headers.h"
#include "GameObject.h"

class SceneLoader {
private:
	//Lists of references to reduce memory duplication
	std::vector<GLMaterial*> materialList;
	std::vector<GLDrawable*> glDrawableList;
	std::vector<GameObject*> gameObjectList;

public:
	//Load scene by number or by a .cap scene file
	static void loadScene(int sceneNumber, std::vector<GameObject*> &objects);
	static void loadScene(const char * scene_file, std::vector<GameObject*> &objects);
};