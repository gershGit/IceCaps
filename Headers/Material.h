/*
	Class to store a material for an opengl drawable
*/

#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

//Different types of materials, used to determine a shader and check for necessary info
enum mType {SIMPLE, PBR, GLASS, UNLIT_TEX, SIMPLE_TEX, SIMPLE_DIFFUSE_SPECULAR, SIMPLE_PHONG_TEXTURED, PHONG, PHONG_SIMPLE, SSS, STANDARD};

//Types of textures
enum tType {DIFFUSE, METAL_MASK, SPECULAR_MASK, NORMAL_MAP, DISPLACEMENT_MAP};

class GLMaterial {
public:
	//Constructor and destructor
	GLMaterial();
	~GLMaterial();

	//Stores the type of material
	mType type;
	//Reference to the shader that holds the vertex and fragment (?geometry) shaders
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

	//PBR texture information
	//TODO create PBR material support
	GLuint mettalic;
	GLuint ambient_occlusion;
	GLuint roughness;

	//Function to properly add a texture to the material
	int addTexture(const char* file_name, tType texture_type, int texture_number);
};