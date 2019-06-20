/*
	System that recalculates bounds based on new positions
*/

#pragma once
#include "Core/Main_Headers.h"
#include "Core/EntitySystem.h"

class BoundsSystem : public EntitySystem {
public:
	//Managers that are used by the system
	ArrayManager<transform>* transformManager = nullptr;
	ArrayManager<uint64_t>* entityTags = nullptr;
	ArrayManager<AABB>* boundsManager = nullptr;

	BoundsSystem();
	~BoundsSystem() {};

	//Entity system functions
	void initialize();
	void start();
	void onUpdate();

	//Function used on the update of the system
	void calculateBoundExtremes(int entityID);
};