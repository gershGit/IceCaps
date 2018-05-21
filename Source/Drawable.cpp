#include "Drawable.h"

GLDrawable::GLDrawable()
{
	
}


GLDrawable::~GLDrawable()
{
}

void GLDrawable::generateBuffers() {
	// VBO creation
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

	//VAO creation
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO implementation
	if (bufferAttributes.x > 0 && bufferAttributes.y == 0 && bufferAttributes.z == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Color attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));

		//Color info stored in vao position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x == 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //Normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	else if (bufferAttributes.x > 0 && bufferAttributes.y > 0 && bufferAttributes.z == 0) {
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
	else if (bufferAttributes.x > 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0) {
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
	else if (bufferAttributes.x == 0 && bufferAttributes.y == 0 && bufferAttributes.z > 0) {
		glEnableVertexAttribArray(0); //Position attribute
		glEnableVertexAttribArray(1); //UV attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));

		//UV info stored in vao at position 1
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	}
	glBindVertexArray(0);
}