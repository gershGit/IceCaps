/*
	A class for any object to be drawn using OpenGL
*/

#pragma once
#include "Headers.h"
#include "Property.h"
#include <vector>
#include "Material.h"

//Types of objects. This is important for raytracing in particular
enum dType { MESH, SPHERE, UI_DRAWABLE };

class GLDrawable : public Property
{
private:

public:
	//Constructor and destructor
	GLDrawable();
	~GLDrawable();

	//Stores the type of object
	dType dtype;

	//References for an objects vertex buffer object, index/element buffer object, and the vertex attribute object to use them
	GLuint vbo;
	GLuint vao;
	GLuint ebo;

	//Bool used to determine whether or not OpenGL should draw by vertices or indices
	bool usingEBO = false;

	//Vector that tells what attributes a vertex buffer object needs
	//TODO deprecate this in favor of the type of material being used
	glm::vec4 bufferAttributes;

	int LODs = 0;
	std::vector<float> LOD_distances;
	//The coordinates to be used for the vertex buffer object
	//TODO ensure this is freed once the vbo is initialized
	std::vector<std::vector<float>> coords;
	//Sizes are kept for rendering information
	std::vector<int> coords_sizes;

	//The coordinates to be used for the vertex buffer object
	//TODO ensure this is freed once the vbo is initialized
	std::vector<std::vector<unsigned int>> indices;
	//Sizes are kept for rendering information
	std::vector<int> indices_sizes;

	//The material this drawable will use to render. Includes the shader
	GLMaterial* material;

	//Function to initialize the vbo, ebo, and vao. Called once the coordinates and indices have been set
	//TODO fix this to use the material instead of the bufferAttricutes vector
	void generateBuffers();
};
