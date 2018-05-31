#pragma once
#include "Headers.h"
#include "GameObject.h"


class SceneLoader {
private:
	std::vector<GLMaterial*> materialList;
	std::vector<GLDrawable*> glDrawableList;
	std::vector<GameObject*> gameObjectList;
};