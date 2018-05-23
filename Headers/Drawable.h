#pragma once
#include "Headers.h"
#include "Property.h"
#include <vector>
#include "Material.h"

enum dType { MESH, SPHERE };

class GLDrawable: public Property
{
private:

public:
	dType dtype;

	GLuint vbo;
	GLuint vao;
	GLuint ebo;

	bool usingEBO = false;
	glm::vec3 bufferAttributes;
	std::vector<float> coords;
	std::vector<unsigned int> indeces;
	GLMaterial* material;

	GLDrawable();
	~GLDrawable();
	void generateBuffers();
};
