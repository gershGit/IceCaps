#include "Core/BoundsSystem.h"
#include "Core/ManagersFactories.h"
#include "Core/StringTranslation.h"
#include "Core/Global_Callbacks.h"

//Sets the entity system data for the bunds system
BoundsSystem::BoundsSystem()
{
	operatesOn = { TRANSFORM, PREFAB_COMPONENT, AABB_COMPONENT, TAGS_COMPONENT };
	entityListRequiredComponents = { {TRANSFORM, AABB_COMPONENT} };
	systemType = BOUNDS_SYSTEM;
}

//Set the managers from the list
void BoundsSystem::initialize()
{
	transformManager = dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(*managers, TRANSFORM));
	entityTags = dynamic_cast<ArrayManager<uint64_t>*>(getCManager<uint64_t>(*managers, TAGS_COMPONENT));
	boundsManager = dynamic_cast<ArrayManager<AABB>*>(getCManager<AABB>(*managers, AABB_COMPONENT));
	delete managers;
}

//Do nothing on start
void BoundsSystem::start()
{
}

//On update this system calculates the bounds of every object
void BoundsSystem::onUpdate()
{
	for (int i = 0; i < entities->size(); i++) {
		calculateBoundExtremes(entities->at(i));
	}
}

//Calculates the absolute maximum bounds for the entities
void BoundsSystem::calculateBoundExtremes(int entityID)
{
	transform t = transformManager->getComponent(entityID);
	AABB* bounds = boundsManager->getComponentAddress(entityID);

	bounds->pos = t.pos;
	bounds->points[0] = glm::vec3(bounds->pos.x - bounds->size.x, bounds->pos.y - bounds->size.y, bounds->pos.z - bounds->size.z);
	bounds->points[7] = glm::vec3(bounds->pos.x + bounds->size.x, bounds->pos.y + bounds->size.y, bounds->pos.z + bounds->size.z);
}
