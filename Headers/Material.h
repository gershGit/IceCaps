#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

enum mType {PBR, GLASS, PHONG, SSS};
enum tType {DIFFUSE, METAL_MASK, SPEC_MASK, NORMAL, DISPLACEMENT};

class GLMaterial {
public:
	mType type;
	Shader shader;

	std::vector<GLuint> textures;
	std::vector<tType> textureTypes;

	GLMaterial();
	~GLMaterial();
};