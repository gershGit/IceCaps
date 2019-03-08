/*
	Class to generate objects quickly
*/

#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "baseShapeCoords.h"

//Enumerations for primitives and lights
enum primitive { CUBE_PRIMITIVE, LIGHT_CUBE, SPHERE_PRIMITVE, PLANE_PRIMITIVE };
enum lightType { POINT_LIGHT };

struct HeightMap {
	unsigned char * image;
	float waterlevel;
	int width;
	int height;
	float maxHeight;
};

class ObjectFactory {
private:
	//Simple materials and coordinate spawner for default information
	GLMaterial * light_material;
	GLMaterial * purple_material;
	GLMaterial * dialectric_standard_material;
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
	GameObject* createObject(primitive primitive_type, int id);
	GameObject* createObject(GLDrawable* drawable = nullptr, GLMaterial* material = nullptr);
	GameObject* createObject(std::vector<float> coordinates);
	GameObject* createObject(GLMaterial* material);

	GameObject* createTerrain(float length, float maxHeight, float waterlevel, const char * image_path);
	GameObject* createTerrainSaveMap(float length, float maxHeight, float waterlevel, const char * image_path, HeightMap * map);
	//TODO implement this
	GameObject* createTerrain(float length, float maxHeight, float waterlevel, const char * image_path, const char * normal_map_path, unsigned char *image);
	//TODO more terrain creators for when there are more maps
	void addFoliage(GameObject * foliage_ptr, float x_pos, float z_pos, HeightMap * heightmap);
	GameObject * createLight(lightType type_of_light, glm::vec3 position, glm::vec3 color, float strength, bool visible);

	//Sets properties onto objects and materials
	void setMaterial(GameObject* object, GLMaterial* material);
	void setDrawable(GLMaterial* material, GLDrawable* drawable);
};

void AddPoint(int length, float resolution, int xVal, int zVal, unsigned char * image, int width, float maxHeight, std::vector<float> &coordinates);
