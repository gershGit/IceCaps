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
	std::vector<float> coords;
	GLMaterial material;

	GLDrawable();
	~GLDrawable();
	void generateBuffers();
};
