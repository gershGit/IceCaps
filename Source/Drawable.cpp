#include "Drawable.h"

GLDrawable::GLDrawable()
{
}

GLDrawable::~GLDrawable()
{
}

void GLDrawable::generateBuffers() {
	//VAO creation
	if (LODs <= 1) {
		vao_vector.push_back(GLuint());
		glGenVertexArrays(1, &vao_vector[0]);
		glBindVertexArray(vao_vector[0]);

		// VBO creation
		vbo_vector.push_back(GLuint());
		glGenBuffers(1, &vbo_vector[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);
		glBufferData(GL_ARRAY_BUFFER, coords[0].size() * sizeof(float), coords[0].data(), GL_STATIC_DRAW);
	}
	else {
		for (int i = 0; i < LODs; i++) {
			vao_vector.push_back(GLuint());
			glGenVertexArrays(1, &vao_vector[i]);
			glBindVertexArray(vao_vector[i]);

			// VBO creation
			vbo_vector.push_back(GLuint());
			glGenBuffers(1, &vbo_vector[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[i]);
			glBufferData(GL_ARRAY_BUFFER, coords[i].size() * sizeof(float), coords[i].data(), GL_STATIC_DRAW);
		}
	}

	
	//EBO creation
	if (usingEBO && LODs<=1) {
		ebo_vector.push_back(GLuint());
		glGenBuffers(1, &ebo_vector[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_vector[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[0].size() * sizeof(unsigned int), indices[0].data(), GL_STATIC_DRAW);
	}
	else if (usingEBO) {	
		for (int e = 0; e < LODs; e++) {
			glBindVertexArray(vao_vector[e]);
			ebo_vector.push_back(GLuint());
			glGenBuffers(1, &ebo_vector[e]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_vector[e]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[e].size() * sizeof(unsigned int), indices[e].data(), GL_STATIC_DRAW);
		}
	}

	// VAO implementation
	if (material->type == SIMPLE) {
		glEnableVertexAttribArray(0); //Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), 0);
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x > 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Color attribute
		glEnableVertexAttribArray(2); //Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

		//Color info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

		//Normal info stored in vao at position 2
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (bufferAttributes.x > 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Color attribute
		glEnableVertexAttribArray(2); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

		//Color info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//UV info stored in vao at position 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

		//UV info stored in vao at position 1
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y > 0 && bufferAttributes.z > 0 && bufferAttributes.w == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glEnableVertexAttribArray(2); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[0]);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//UV info stored in vao at position 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	else if (material->type == STANDARD || material->type == PBR_BASIC || material->type == PHONG_SIMPLE || material->type == SIMPLE_TEX || material->type == DIALECTRIC || material->type == METALLIC || material->type == PBR_SIMPLE || material->type == PBR_EMISSIVE) {
		for (int i = 0; i < LODs; i++) {
			glBindVertexArray(vao_vector[i]);
			glEnableVertexAttribArray(0); //Position attribute
			glEnableVertexAttribArray(1); //Normal attribute
			glEnableVertexAttribArray(2); //Tangent attribute
			glEnableVertexAttribArray(3); //BitTangent attribute
			glEnableVertexAttribArray(4); //UV attribute
			glBindBuffer(GL_ARRAY_BUFFER, vbo_vector[i]);

			//Position info stored in vao at position 0
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), 0);

			//Normal info stored in vao at position 1
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

			//Tangent info stored in vao at position 1
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

			//BitTangent info stored in vao at position 1
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(9 * sizeof(float)));

			//UV info stored in vao at position 2
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}