/*
	Class to store a material for an opengl drawable
*/

#pragma once
#include "Headers.h"
#include "Shader.h"
#include <vector>

//Different types of materials, used to determine a shader and check for necessary info
enum mType { UI, SIMPLE, PBR_BASIC, GLASS, UNLIT_TEX, SIMPLE_TEX, SIMPLE_DIFFUSE_SPECULAR, SIMPLE_PHONG_TEXTURED, PHONG, PHONG_SIMPLE, PHONG_TEXTURED, SSS, STANDARD, DIALECTRIC, METALLIC, PBR_SIMPLE, PBR_EMISSIVE };

//Types of textures
enum tType { DIFFUSE, METAL_MASK, SPECULAR_MASK, NORMAL_MAP, DISPLACEMENT_MAP, ROUGHNESS_MAP, AO_MAP, METALLIC_MASK, EMISSION_MAP };

class GLMaterial {
public:
	//Constructor and destructor
	GLMaterial();
	~GLMaterial();

	//Stores the type of material
	mType type;
	//Reference to the shader that holds the vertex and fragment (?geometry) shaders
	ShaderProgram shader;

	//Overall color. Used in combination of textures or for simple shaders
	glm::vec3 color;

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
	GLuint metallic;
	int metallicTexNumber;

	GLuint ao;
	int aoTexNumber;

	GLuint emissive;
	int emissiveTexNumber;

	//Roughness texture information
	GLuint roughness;
	int roughnessTexNumber;

	//Values for textureless PBR
	float f_roughness;
	float f_metallness;

	//Function to properly add a texture to the material
	int addTexture(const char* file_name, tType texture_type, int texture_number);
	//Function that auto builds shader based on textures added and material type
	int buildShader();
	//Function to set material type
	void setMaterialType(mType material_type);
};