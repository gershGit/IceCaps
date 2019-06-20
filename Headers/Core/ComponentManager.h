/*
* Parent class for all component managers
*/
#include "Core/EntityComponents.h"
#include <map>

#pragma once
class ComponentManager
{
public:
	component_type componentType = NO_TYPE;
	ComponentManager() {};
	~ComponentManager() {};
	virtual void setType(component_type type_in) { componentType = type_in; };
};

template<typename CType>
class CManager : public ComponentManager {
public:
	CManager() { this->componentType = NO_TYPE; };
	~CManager() {};
	virtual int getSize() { return 0; };
	virtual bool hasEntity(int entityID) {return false;};
	virtual CType getComponent(int entityID) {return CType();};
	virtual CType* getComponentAddress(int entityID) { return new CType(); };
	virtual void addComponent(int entityID, CType component) {};
	virtual void setComponent(int entityID, CType component) {};
	virtual std::vector<int> getEntities() { return std::vector<int>(); };
};

template <typename CType>
class MappedManager : public CManager<CType> {
public:
	std::map<int, CType> componentMap;
public:
	MappedManager(component_type type_in) { this->componentType = type_in; };
	~MappedManager() {};
	int getSize() { return (int) componentMap.size(); };
	bool hasEntity(int entityID) { return (componentMap.count(entityID) > 0); };
	CType getComponent(int entityID) { return componentMap.at(entityID); };
	CType* getComponentAddress(int entityID) { return &componentMap.at(entityID); };
	void addComponent(int entityID, CType component) { componentMap.insert(std::pair<int, CType>(entityID, component)); };
	void setComponent(int entityID, CType component) { componentMap.at(entityID) = component; };
	std::vector<int> getEntities(){ 
		std::vector<int> vec = std::vector<int>();
		for (std::pair<int, CType> pair : componentMap) {
			vec.push_back(pair.first);
		}
		return vec;
	}
	std::pair<int, CType*> getFirst() {return std::pair<int,CType*>(componentMap.begin()->first, &componentMap.begin()->second);}
};

template<typename CType>
class NodeManager : public CManager<CType> {
public:
	std::map<int, CType> componentMap;
	material_type pipelineType;
public:
	NodeManager(component_type type_in, material_type mType) { this->componentType = type_in; this->pipelineType = mType; };
	~NodeManager() {};
	int getSize() { return (int)componentMap.size(); };
	bool hasEntity(int entityID) { return (componentMap.count(entityID) > 0); };
	CType getComponent(int entityID) { return componentMap.at(entityID); };
	CType* getComponentAddress(int entityID) { return &componentMap.at(entityID); };
	void addComponent(int entityID, CType component) { componentMap.insert(std::pair<int, CType>(entityID, component)); };
	void setComponent(int entityID, CType component) { componentMap.at(entityID) = component; };
	std::vector<int> getEntities() {
		std::vector<int> vec = std::vector<int>();
		for (std::pair<int, CType> pair : componentMap) {
			vec.push_back(pair.first);
		}
		return vec;
	}
	std::pair<int, CType*> getFirst() { return std::pair<int, CType*>(componentMap.begin()->first, &componentMap.begin()->second); }
};

template <typename CType>
class ArrayManager : public CManager<CType> {
public:
	int mySize;
	CType * componentArray;
public:
	ArrayManager(component_type type_in) { this->componentType = type_in; };
	~ArrayManager() {};
	int getSize() { return mySize; };
	void setSize(int size_in) { mySize = size_in; componentArray = (CType*)malloc(sizeof(CType)*mySize); }
	bool hasEntity(int entityID) { return mySize > entityID; };
	CType getComponent(int entityID) { return componentArray[entityID]; };
	CType* getComponentAddress(int entityID) { return &componentArray[entityID]; };
	void addComponent(int entityID, CType component) { componentArray[entityID] = component; };
	void setComponent(int entityID, CType component) { componentArray[entityID] = component; };
};

template <typename CType>
class LODMappedManager : public CManager<CType> {
private:
	std::multimap<int, CType> lodMap;
public:
	LODMappedManager(component_type type_in) { this->componentType = type_in; };
	~LODMappedManager() {};
	int getSize() { return lodMap.size(); }
	bool hasEntity(int entityID) { return lodMap.count(entityID) > 0; }
	CType getComponent(int entityID) { return lodMap.find(entityID); }
	CType getComponentAddress(int entityID) { return lodMap.find(entityID); }
	void addComponent(int entityID, CType component) { lodMap.insert(std::pair<int, CType>(entityID, component)); };
	//void setComponent(int entityID, CType component) { lodMap.find(entityID) = std::pair<int, CType>(entityID, component)); };
};

template <typename CType>
class VectorManager : public CManager<CType> {
private:
	std::vector<CType> components = std::vector<CType>();
public:
	VectorManager(component_type type_in) { this->componentType = type_in; };
	~VectorManager() {};
	void addComponent(int ignore, CType component) {components.push_back(component);};
	int getSize() { return (int) components.size(); };
	CType getComponent(int index) { return components.at(index); };
	std::vector<CType>* getComponents() { return &components; };
};