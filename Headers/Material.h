/*
	Class to store a material for an opengl drawable
*/

#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

//Different types of materials, used to determine a shader and check for necessary info
enum mType {SIMPLE, PBR_BASIC, GLASS, UNLIT_TEX, SIMPLE_TEX, SIMPLE_DIFFUSE_SPECULAR, SIMPLE_PHONG_TEXTURED, PHONG, PHONG_SIMPLE, SSS, STANDARD};

//Types of textures
enum tType {DIFFUSE, METAL_MASK, SPECULAR_MASK, NORMAL_MAP, DISPLACEMENT_MAP, ROUGHNESS_MAP, AO_MAP};

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
	GLuint mettalic;
	int mettlaicTexNumber;

	GLuint ambient_occlusion;
	int ambient_occlusionTexNumber;

	//Roughness texture information
	GLuint roughness;
	int roughnessTexNumber;

	//Function to properly add a texture to the material
	int addTexture(const char* file_name, tType texture_type, int texture_number);
};