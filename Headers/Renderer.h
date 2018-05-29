#include "Headers.h"
#include "GameObject.h"
#include "Drawable.h"

class GLRenderer {

public:

	void render(GameObject* object, GameObject* camera) {
		//Debugging statement for which object is being rendered
		//std::cout << "Now rendering " << object->name << std::endl;

		//Get a reference to the drawable being rendered
		GLDrawable* drawableProp = object->glDrawable;
		//Get the shaderprogram to be used
		ShaderProgram shader = drawableProp->material->shader;
		shader.use();

		//Store the matrices used in all shaders
		GLuint loc = glGetUniformLocation(shader.id(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &object->getTransform()[0][0]);
		GLuint viewLoc = glGetUniformLocation(shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->camera->getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(camera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);

		//Upload uniforms based on the type of material
		if (drawableProp->material->type == SIMPLE) {

		}
		else if (drawableProp->material->type == PHONG_SIMPLE) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);
		}
		else if (drawableProp->material->type == SIMPLE_TEX) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);
		}
		else if (drawableProp->material->type == SIMPLE_DIFFUSE_SPECULAR) {
			//Lights
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunPos, 2, -6, -2);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 1.0, 0.2, 0.2, 0.8);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), -2, 2, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), -2, 2, -10);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), 2, 2, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), 2, 2, -10);

			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), 0.1, 0.1, 1.0, 0.1);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), 0.1, 0.1, 1.0, 0.05);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), 0.1, 0.1, 1.0, 0.5);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), 0.1, 0.1, 1.0, 0.3);

			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);
		}
		else if (drawableProp->material->type == SIMPLE_PHONG_TEXTURED) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint sunPos = glGetUniformLocation(shader.id(), "sunPos");
			glUniform3f(sunPos, -2, 3, -6);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);
		}
		else if (drawableProp->material->type == STANDARD) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//Lights
			GLuint sunPos = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunPos, 2, -6, -2);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 1.0, 0.2, 0.2, 0.8);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), -2, 2, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), -2, 2, -10);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), 2, 2, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), 2, 2, -10);

			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), 0.1, 0.1, 1.0, 0.1);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), 0.1, 0.1, 1.0, 0.05);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), 0.1, 0.1, 1.0, 0.5);
			glUniform4f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), 0.1, 0.1, 1.0, 0.3);


			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);
		}
		else if (drawableProp->material->type == PBR_BASIC) {
			GLuint eye = glGetUniformLocation(shader.id(), "eye");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			//Lights
			GLuint sunAngle = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunAngle, 0, -1, 0);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 0.1, 4.0, 0.1, 0.4);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), -2, 4, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), -2, 8, -10);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), 2, 2, -4);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), 2, 2, -10);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), 0, 1.0f, 0.0f);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), 1.0f, 1.0f, 0.0f);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), 0.0f, 0.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), 1.0f, 0.0f, 1.0f);


			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "albedoSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specularImageLoc = glGetUniformLocation(shader.id(), "metallicSampler");
			glUniform1i(specularImageLoc, drawableProp->material->specularTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);
		}
		else if (drawableProp->material->type == UNLIT_TEX) {
			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);
		}

		// Bind the VAO of the object ot access the correct info
		glBindVertexArray(drawableProp->vao);	

		//Render by indices or vertices based on if an ebo has been created
		if (drawableProp->usingEBO) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawableProp->ebo);
			glDrawElements(GL_TRIANGLES, drawableProp->indices.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, drawableProp->coords.size());
			glBindVertexArray(0);
		}
		
		//Render all the children of this object at the same time to ensure all objects, not only top level ones are rendered
		for (GameObject* child : object->children) {
			render(child, camera);
		}
	}
};

