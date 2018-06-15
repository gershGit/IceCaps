#include "Headers.h"
#include "GameObject.h"
#include "Drawable.h"

class GLRenderer {

public:

	void renderObjects(GameObject* object, GameObject* camera, std::vector<GameObject*> lights) {
		//Debugging statement for which object is being rendered
		//std::cout << "Now rendering " << object->name << std::endl;
		//object->printPosition();

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
			GLuint color = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(color, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);
		}
		else if (drawableProp->material->type == PHONG_SIMPLE) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = camera->forward();
			//glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);
			glUniform3f(eye, eyeVec.x, eyeVec.y, eyeVec.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, -2, -6, -1);

			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), lights[0]->pos.x, lights[0]->pos.y, lights[0]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), lights[1]->pos.x, lights[1]->pos.y, lights[1]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), lights[2]->pos.x, lights[2]->pos.y, lights[2]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), lights[3]->pos.x, lights[3]->pos.y, lights[3]->pos.z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == DIALECTRIC || drawableProp->material->type == METALLIC) {
			GLuint eyeLoc = glGetUniformLocation(shader.id(), "eyePos");
			//glm::vec3 eyeVec = camera->forward();
			glUniform3f(eyeLoc, camera->pos.x, camera->pos.y, camera->pos.z);
			//glUniform3f(eye, eyeVec.x, eyeVec.y, eyeVec.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, -2, -6, -1);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 0.8, 0.6, 0.8, 5.0);

			GLuint roughness = glGetUniformLocation(shader.id(), "roughness");
			glUniform1f(roughness, 0.4);

			GLuint baseReflectance = glGetUniformLocation(shader.id(), "baseReflectance");
			glUniform1f(baseReflectance, 0.04);

			//TODO ensure this is right and being used
			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix3fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), lights[0]->pos.x, lights[0]->pos.y, lights[0]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), lights[1]->pos.x, lights[1]->pos.y, lights[1]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), lights[2]->pos.x, lights[2]->pos.y, lights[2]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), lights[3]->pos.x, lights[3]->pos.y, lights[3]->pos.z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == SIMPLE_TEX) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint sunPos = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunPos, -2, -9, -3);

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
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = camera->forward();
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint sunPos = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunPos, -2, -6, 1);

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
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = camera->forward();
			//glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);
			glUniform3f(eye, eyeVec.x, eyeVec.y, eyeVec.z);

			//Lights
			GLuint sunAngle = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunAngle, -2, -9, -3);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 1.0, 1.0, 1.0, 1.0);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), lights[0]->pos.x, lights[0]->pos.y, lights[0]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), lights[1]->pos.x, lights[1]->pos.y, lights[1]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), lights[2]->pos.x, lights[2]->pos.y, lights[2]->pos.z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), lights[3]->pos.x, lights[3]->pos.y, lights[3]->pos.z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);


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
			renderObjects(child, camera, lights);
		}
	}

	void renderLights(GameObject* light_object, GameObject* camera) {
		//Debugging statement for which object is being rendered
		//std::cout << "Now rendering " << object->name << std::endl;
		//object->printPosition();

		//Get a reference to the drawable being rendered
		GLDrawable* drawableProp = light_object->glDrawable;
		//Get the shaderprogram to be used
		ShaderProgram shader = drawableProp->material->shader;
		shader.use();

		//Store the matrices used in all shaders
		GLuint loc = glGetUniformLocation(shader.id(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &light_object->getTransform()[0][0]);
		GLuint viewLoc = glGetUniformLocation(shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->camera->getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(camera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);

		//Upload uniforms based on the type of material
		GLuint color = glGetUniformLocation(shader.id(), "aColor");
		glUniform3f(color, light_object->light->color.r, light_object->light->color.g, light_object->light->color.b);

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
	}
};

