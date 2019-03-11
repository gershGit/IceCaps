/*
	Mapped manager for rigid body components
*/

#include "Core/Main_Headers.h"
#include "Core/MappedComponentManager.h"

class RigidBodyManager : public MappedComponentManager {
private:
	std::vector<rigid_body>* componenets = new std::vector<rigid_body>();
public:
	RigidBodyManager();
	~RigidBodyManager();
	void addComponent(int entityID) { entities->push_back(entityID); componenets->push_back(rigid_body()); }
	void setComponent(int entityID, rigid_body component_in) { componenets->at(getEntityIndex(entityID)) = component_in; };
	rigid_body getComponent(int entityID) { return componenets->at(getEntityIndex(entityID)); };
	rigid_body* getComponentAddress(int entityID) { return &componenets->at(getEntityIndex(entityID)); };
	std::vector<rigid_body>* getComponents() { return componenets; };
};