#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int GLMaterial::addTexture(const char * file_name, tType texture_type, int texture_number)
{
	std::cout << "Loading texture at " << file_name << std::endl;
	if (texture_type == DIFFUSE) {
		diffuseTexNumber = texture_number;
		glActiveTexture(GL_TEXTURE0 + texture_number);
		glGenTextures(1, &diffuse);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}
	else if (texture_type == SPECULAR_MASK) {
		specularTexNumber = texture_number;
		glActiveTexture(GL_TEXTURE0 + texture_number);
		glGenTextures(1, &specular);
		glBindTexture(GL_TEXTURE_2D, specular);
	}
	else if (texture_type == NORMAL_MAP) {
		normalTexNumber = texture_number;
		glActiveTexture(GL_TEXTURE0 + texture_number);
		glGenTextures(1, &normal);
		glBindTexture(GL_TEXTURE_2D, normal);
	}
		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(file_name, &width, &height, &nrComponents, 0);
		if (data) {
			GLenum format = 0;
			switch (nrComponents) {
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << file_name << std::endl;
			stbi_image_free(data);
			return 1;
		}
}

GLMaterial::GLMaterial() {};
GLMaterial::~GLMaterial() {};