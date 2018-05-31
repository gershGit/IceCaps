#pragma once
#include "Headers.h"
#include "GameObject.h"

class ObjectFactory {
public:
	static GameObject* createObject();
	static GLDrawable* createGLDrawable();
	static GLMaterial* createGLMaterial();
	static ShaderProgram createShaderProgram();

	static GameObject* createObject(GLDrawable* drawable = nullptr, GLMaterial* material = nullptr);
	static GameObject* createObject(std::vector<float> coordinates);
	static GameObject* createObject(GLMaterial* material);
	static void setMaterial(GameObject* object, GLMaterial* material);
	static void setDrawable(GLMaterial* material, GLDrawable* drawable);
	
};