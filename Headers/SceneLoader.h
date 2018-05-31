#pragma once
#include "Headers.h"
#include "GameObject.h"


class SceneLoader {
private:
	std::vector<GLMaterial*> materialList;
	std::vector<GLDrawable*> glDrawableList;
	std::vector<GameObject*> gameObjectList;

public:
	static void loadScene(int sceneNumber, std::vector<GameObject*> &objects);
	static void loadScene(const char * scene_file, std::vector<GameObject*> &objects);
};