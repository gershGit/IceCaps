#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

enum mType {SIMPLE, PBR, GLASS, UNLIT_TEX, PHONG, PHONG_SIMPLE, SSS};
enum tType {DIFFUSE, METAL_MASK, SPEC_MASK, NORMAL, DISPLACEMENT};

class GLMaterial {
public:
	mType type;
	ShaderProgram shader;

	std::vector<GLuint> textures;
	std::vector<tType> textureTypes;
	GLuint diffuse;
	int diffuseTexNumber;
	GLuint normal;
	GLuint mettalic;
	GLuint specular;
	GLuint ambient_occlusion;

	int addTexture(const char* file_name, tType texture_type, int texture_number);
	GLMaterial();
	~GLMaterial();
};