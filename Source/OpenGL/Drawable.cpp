#include "Drawable.h"

GLDrawable::GLDrawable()
{
}

GLDrawable::~GLDrawable()
{
}

void GLDrawable::generateBuffers() {
	//VAO creation
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VBO creation
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

	//EBO creation
	if (usingEBO) {
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	// VAO implementation
	if (material->type == SIMPLE) {
		glEnableVertexAttribArray(0); //Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(0 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x > 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Color attribute
		glEnableVertexAttribArray(2); //Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));

		//Color info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

		//Normal info stored in vao at position 2
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (bufferAttributes.x > 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Color attribute
		glEnableVertexAttribArray(2); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));

		//Color info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//UV info stored in vao at position 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));

		//UV info stored in vao at position 1
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y > 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glEnableVertexAttribArray(2); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//UV info stored in vao at position 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (material->type == STANDARD || material->type == PBR_BASIC || material->type == PHONG || material->type == PHONG_SIMPLE || material->type == PHONG_TEXTURED ||
		material->type == SIMPLE_TEX || material->type == DIALECTRIC || material->type == METALLIC || material->type == PBR_SIMPLE || material->type==PBR_EMISSIVE ||
		material->type == SSS) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glEnableVertexAttribArray(2); //Tangent attribute
		glEnableVertexAttribArray(3); //BitTangent attribute
		glEnableVertexAttribArray(4); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(0 * sizeof(float)));

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

		//Tangent info stored in vao at position 1
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

		//BitTangent info stored in vao at position 1
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(9 * sizeof(float)));

		//UV info stored in vao at position 2
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}