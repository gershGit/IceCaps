/*
	Class to generate objects quickly
*/

#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "baseShapeCoords.h"

//Enumerations for primitives and lights
enum primitive {CUBE_PRIMITIVE, LIGHT_CUBE, SPHERE_PRIMITVE, PLANE_PRIMITIVE};
enum lightType {POINT_LIGHT};

class ObjectFactory {
private:
	//Simple materials and coordinate spawner for default information
	GLMaterial * light_material;
	GLMaterial * purple_material;
	CoordsSpawner * mCoordsSpawner;
public:
	ObjectFactory();
	~ObjectFactory();

	//Creation of blank objects
	GameObject* createObject();
	GLDrawable* createGLDrawable();
	GLMaterial* createGLMaterial();
	ShaderProgram createShaderProgram();

	//Creation of objects with varying parameters
	GameObject* createObject(primitive primitive_type);
	GameObject * createLight(lightType type_of_light, glm::vec3 position, glm::vec3 color, float strength, bool visible);
	GameObject* createObject(GLDrawable* drawable = nullptr, GLMaterial* material = nullptr);
	GameObject* createObject(std::vector<float> coordinates);
	GameObject* createObject(GLMaterial* material);

	//Sets properties onto objects and materials
	void setMaterial(GameObject* object, GLMaterial* material);
	void setDrawable(GLMaterial* material, GLDrawable* drawable);
	
};