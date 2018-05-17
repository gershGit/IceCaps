#include "Drawable.h"

GLDrawable::GLDrawable()
{
	
}


GLDrawable::~GLDrawable()
{
}

void GLDrawable::generateBuffers(int colors, int normals, int uvs) {
	// VBO creation
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

	//VAO creation
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VAO implementation
	if (colors > 0 && normals == 0 && uvs == 0) {
		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		//Color info stored in vao position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (colors == 0 && normals>0 && uvs ==0) {
		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		//Normal info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (colors > 0 && normals > 0 && uvs == 0) {
		//Position info stored in vao at position 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
			(void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		//Color info stored in vao at position 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Normal info stored in vao at position 2
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}