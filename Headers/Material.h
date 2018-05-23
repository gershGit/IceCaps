#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

enum mType {SIMPLE, PBR, GLASS, UNLIT_TEX, SIMPLE_TEX, SIMPLE_DIFFUSE_SPECULAR, SIMPLE_PHONG_TEXTURED, PHONG, PHONG_SIMPLE, SSS};
enum tType {DIFFUSE, METAL_MASK, SPECULAR_MASK, NORMAL_MAP, DISPLACEMENT_MAP};

class GLMaterial {
public:
	mType type;
	ShaderProgram shader;

	//Diffuse texture information
	GLuint diffuse;
	int diffuseTexNumber;

	//Specular texture information
	GLuint specular;
	int specularTexNumber;

	//Normal texture information
	GLuint normal;
	int normalTexNumber;

	GLuint mettalic;
	GLuint ambient_occlusion;
	GLuint roughness;

	int addTexture(const char* file_name, tType texture_type, int texture_number);
	GLMaterial();
	~GLMaterial();
};