#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

enum mType {SIMPLE, PBR, GLASS, UNLIT_TEX, SIMPLE_TEX, SIMPLE_DIFFUSE_SPECULAR, PHONG, PHONG_SIMPLE, SSS};
enum tType {DIFFUSE, METAL_MASK, SPECULAR_MASK, NORMAL, DISPLACEMENT};

class GLMaterial {
public:
	mType type;
	ShaderProgram shader;

	//Diffuse texture information
	GLuint diffuse;
	int diffuseTexNumber;

	//Specular texture information
	GLuint normal;
	int specularTexNumber;

	GLuint mettalic;
	GLuint specular;
	GLuint ambient_occlusion;

	int addTexture(const char* file_name, tType texture_type, int texture_number);
	GLMaterial();
	~GLMaterial();
};