#include "Headers.h"
#include "GameObject.h"
#include "Drawable.h"

class GLRenderer {

public:

	void render(GameObject* object, GameObject* camera) {
		//std::cout << "Now rendering " << object->name << std::endl;

		GLDrawable* drawableProp = object->glDrawable;
		ShaderProgram shader = drawableProp->material->shader;
		shader.use();

	
		GLuint loc = glGetUniformLocation(shader.id(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &object->getTransform()[0][0]);
		GLuint viewLoc = glGetUniformLocation(shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->camera->getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera->camera->getProjectionMatrix()[0][0]);
		if (drawableProp->material->type == SIMPLE) {

		}
		else if (drawableProp->material->type == PHONG_SIMPLE) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//TODO make an array of lights instead
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glm::mat3 itMatrix = object->getInverseTranspose();
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &itMatrix[0][0]);
		}
		else if (drawableProp->material->type == SIMPLE_TEX) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//TODO make an array of lights instead
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glm::mat3 itMatrix = object->getInverseTranspose();
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &itMatrix[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);
		}
		else if (drawableProp->material->type == SIMPLE_DIFFUSE_SPECULAR) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//TODO make an array of lights instead
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glm::mat3 itMatrix = object->getInverseTranspose();
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &itMatrix[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);
		}
		else if (drawableProp->material->type == SIMPLE_PHONG_TEXTURED) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//TODO make an array of lights instead
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glm::mat3 itMatrix = object->getInverseTranspose();
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &itMatrix[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);
		}
		else if (drawableProp->material->type == UNLIT_TEX) {
			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);
		}

		// render the object
		glBindVertexArray(drawableProp->vao);	
		if (drawableProp->usingEBO) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawableProp->ebo);
			glDrawElements(GL_TRIANGLES, drawableProp->indeces.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, drawableProp->coords.size());
			glBindVertexArray(0);
		}
		
		for (GameObject* child : object->children) {
			render(child, camera);
		}
	}
};

