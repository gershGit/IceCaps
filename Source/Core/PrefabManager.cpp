#include "PrefabManager.h"


int PrefabManager::getEntityIndex(int entityID)
{
	for (int i = 0; i < entities->size(); i++) {
		if (entities->at(i) == entityID) {
			return i;
		}
	}
	return -1;
}


PrefabManager::~PrefabManager()
{
}
