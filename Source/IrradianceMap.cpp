#pragma once
#include "IrradianceMap.h"
#include "stb_image.h"

Imap::Imap(const char * fileName, int texturenumber) {
	mapTexNumber = texturenumber;
	glActiveTexture(GL_TEXTURE0 + texturenumber);
	glGenTextures(1, &envMap);
	glBindTexture(GL_TEXTURE_2D, envMap);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(fileName, &width, &height, &nrComponents, 0);
	if (data) {
		glGenTextures(1, &envMap);
		glBindTexture(GL_TEXTURE_2D, envMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load HDR image." << std::endl;
	}
}