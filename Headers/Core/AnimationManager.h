/*
	Mapped component manger for animation components
*/

#pragma once

#include "Core/Main_Headers.h"
#include "Core/MappedComponentManager.h"

class AnimationManager : public MappedComponentManager {
private:
	std::vector<animation>* animations = new std::vector<animation>();
public:
	AnimationManager() { type = ANIMATION_COMPONENT; };
	~AnimationManager() {};
	void addComponent(int entityID) { entities->push_back(entityID); animations->push_back(animation()); };
	void setComponent(int entityID, animation anim_in) { animations->at(getEntityIndex(entityID)) = anim_in; };
	animation getComponent(int entityID) { return animations->at(getEntityIndex(entityID)); };
	animation * getComponentAddress(int entityID) { return &animations->at(getEntityIndex(entityID)); };
	std::vector<animation>* getComponents() { return animations; };
};