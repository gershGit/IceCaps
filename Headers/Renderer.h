/*
	Class to render objects one by one
*/

#include "Headers.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Drawable.h"
#include "IrradianceMap.h"
#include <map>

class GLRenderer {
public:
	//Environment information
	GLuint cubeVBO;
	GLuint cubeVAO;
	GLuint testVAO;
	GLuint testVBO;
	float fogAmount;
	ShaderProgram cubeShader;
	ShaderProgram testShader;
	glm::mat4 viewMatrix;
	GameObject* myCamera;

	void renderObjects(GameObject* object, GameObject* camera, std::vector<GameObject*> lights, Imap* irradianceMap, Imap* environmentMap) {
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
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(camera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);
		GLuint fogLoc = glGetUniformLocation(shader.id(), "fogAmount");
		glUniform1f(fogLoc, fogAmount);

		//Upload uniforms based on the type of material
		if (drawableProp->material->type == SIMPLE) {
			GLuint color = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(color, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);
		}
		else if (drawableProp->material->type == PHONG_SIMPLE) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = -camera->forward();
			//glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, 2, -3, 1);
		}
		else if (drawableProp->material->type == PHONG) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, 2, -3, 1);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == SSS) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyePos");
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, 2, -3, 1);

			GLuint sunColLoc = glGetUniformLocation(shader.id(), "sunColor");
			glUniform3f(sunColLoc, 0.4, 0.4, 0.4);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == PHONG_TEXTURED) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glUniform3f(eye, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint col = glGetUniformLocation(shader.id(), "aColor");
			glUniform3f(col, drawableProp->material->color.x, drawableProp->material->color.y, drawableProp->material->color.z);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, 2, -3, 1);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint specImageLoc = glGetUniformLocation(shader.id(), "specularSampler");
			glUniform1i(specImageLoc, drawableProp->material->specularTexNumber);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

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
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == PBR_SIMPLE) {
			GLuint eyeLoc = glGetUniformLocation(shader.id(), "eyePos");
			glUniform3f(eyeLoc, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint metalImageLoc = glGetUniformLocation(shader.id(), "metallicSampler");
			glUniform1i(metalImageLoc, drawableProp->material->metallicTexNumber);

			GLuint roughnessImageLoc = glGetUniformLocation(shader.id(), "roughnessSampler");
			glUniform1i(roughnessImageLoc, drawableProp->material->roughnessTexNumber);

			GLuint aoImageLoc = glGetUniformLocation(shader.id(), "aoSampler");
			glUniform1i(aoImageLoc, drawableProp->material->aoTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, -2, -6, -1);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 0.8, 0.6, 0.8, 5.0);

			GLuint baseReflectance = glGetUniformLocation(shader.id(), "baseReflectance");
			glUniform1f(baseReflectance, 0.04);

			//TODO ensure this is right and being used
			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix4fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}

		else if (drawableProp->material->type == SIMPLE_TEX) {
			GLuint eye = glGetUniformLocation(shader.id(), "eyeDir");
			glm::vec3 eyeVec = -camera->forward();
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
			glm::vec3 eyeVec = -camera->forward();
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
			glm::vec3 eyeVec = -camera->forward();
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
			if (irradianceMap != nullptr) {
				GLuint irrMapLoc = glGetUniformLocation(shader.id(), "irradianceMap");
				glUniform1i(irrMapLoc, irradianceMap->mapTexNumber);
			}

			if (environmentMap != nullptr) {
				GLuint eMapLoc = glGetUniformLocation(shader.id(), "environmentMap");
				glUniform1i(eMapLoc, environmentMap->mapTexNumber);
			}

			GLuint eyeLoc = glGetUniformLocation(shader.id(), "eyePos");
			glUniform3f(eyeLoc, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);


			GLuint metalImageLoc = glGetUniformLocation(shader.id(), "metallicSampler");
			glUniform1i(metalImageLoc, drawableProp->material->metallicTexNumber);

			GLuint roughnessImageLoc = glGetUniformLocation(shader.id(), "roughnessSampler");
			glUniform1i(roughnessImageLoc, drawableProp->material->roughnessTexNumber);

			GLuint aoImageLoc = glGetUniformLocation(shader.id(), "aoSampler");
			glUniform1i(aoImageLoc, drawableProp->material->aoTexNumber);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, -2, -6, -1);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 0.8, 0.6, 0.8, 1.5);

			GLuint baseReflectance = glGetUniformLocation(shader.id(), "baseReflectance");
			glUniform1f(baseReflectance, 0.04);

			//TODO ensure this is right and being used
			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix4fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
		}
		else if (drawableProp->material->type == PBR_EMISSIVE) {
			if (irradianceMap != nullptr) {
				GLuint irrMapLoc = glGetUniformLocation(shader.id(), "irradianceMap");
				glUniform1i(irrMapLoc, irradianceMap->mapTexNumber);
			}

			if (environmentMap != nullptr) {
				GLuint eMapLoc = glGetUniformLocation(shader.id(), "environmentMap");
				glUniform1i(eMapLoc, environmentMap->mapTexNumber);
			}

			GLuint eyeLoc = glGetUniformLocation(shader.id(), "eyePos");
			glUniform3f(eyeLoc, camera->pos.x, camera->pos.y, camera->pos.z);

			GLuint diffuseImageLoc = glGetUniformLocation(shader.id(), "diffuseSampler");
			glUniform1i(diffuseImageLoc, drawableProp->material->diffuseTexNumber);

			GLuint emissiveImageLoc = glGetUniformLocation(shader.id(), "emissiveSampler");
			glUniform1i(emissiveImageLoc, drawableProp->material->emissiveTexNumber);

			GLuint metalImageLoc = glGetUniformLocation(shader.id(), "metallicSampler");
			glUniform1i(metalImageLoc, drawableProp->material->metallicTexNumber);

			GLuint roughnessImageLoc = glGetUniformLocation(shader.id(), "roughnessSampler");
			glUniform1i(roughnessImageLoc, drawableProp->material->roughnessTexNumber);

			GLuint aoImageLoc = glGetUniformLocation(shader.id(), "aoSampler");
			glUniform1i(aoImageLoc, drawableProp->material->ao);

			GLuint normalImageLoc = glGetUniformLocation(shader.id(), "normalSampler");
			glUniform1i(normalImageLoc, drawableProp->material->normalTexNumber);

			GLuint sunLoc = glGetUniformLocation(shader.id(), "sunAngle");
			glUniform3f(sunLoc, -2, -6, -1);

			GLuint sunColor = glGetUniformLocation(shader.id(), "sunColor");
			glUniform4f(sunColor, 0.8, 0.6, 0.8, 1.5);

			GLuint baseReflectance = glGetUniformLocation(shader.id(), "baseReflectance");
			glUniform1f(baseReflectance, 0.04);

			//TODO ensure this is right and being used
			GLuint itModel = glGetUniformLocation(shader.id(), "itModel");
			glUniformMatrix4fv(itModel, 1, GL_FALSE, &glm::inverse(glm::transpose(object->getTransform()))[0][0]);

			//Point lights
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[0]"), (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).x, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).y, (glm::vec4(lights[0]->pos, 1) * glm::transpose(lights[0]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[1]"), (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).x, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).y, (glm::vec4(lights[1]->pos, 1) * glm::transpose(lights[1]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[2]"), (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).x, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).y, (glm::vec4(lights[2]->pos, 1) * glm::transpose(lights[2]->getTransform())).z);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightPos[3]"), (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).x, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).y, (glm::vec4(lights[3]->pos, 1) * glm::transpose(lights[3]->getTransform())).z);

			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[0]"), lights[0]->light->color.r, lights[0]->light->color.g, lights[0]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[1]"), lights[1]->light->color.r, lights[1]->light->color.g, lights[1]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[2]"), lights[2]->light->color.r, lights[2]->light->color.g, lights[2]->light->color.b);
			glUniform3f(glGetUniformLocation(shader.id(), "pointLightColors[3]"), lights[3]->light->color.r, lights[3]->light->color.g, lights[3]->light->color.b);
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
			glDrawArrays(GL_TRIANGLES, 0, drawableProp->coords.size()/14);
			glBindVertexArray(0);
		}

		//Render all the children of this object at the same time to ensure all objects, not only top level ones are rendered
		for (GameObject* child : object->children) {
			renderObjects(child, camera, lights, irradianceMap, environmentMap);
		}
	}

	void renderParticleSystem(ParticleSystem * pSystem, GameObject* camera, std::vector<GameObject*> lights) {
		pSystem->shader.use();
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); old version
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); //New version
		glDepthMask(false);
		glDepthFunc(GL_LEQUAL);
		//Store the matrices used in all shaders
		GLuint sizeLoc = glGetUniformLocation(pSystem->shader.id(), "radius");
		GLuint quadLoc = glGetUniformLocation(pSystem->shader.id(), "quad");
		GLuint colorLoc = glGetUniformLocation(pSystem->shader.id(), "color");

		GLuint camUpLoc = glGetUniformLocation(pSystem->shader.id(), "worldspace_up");
		glUniform3f(camUpLoc, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
		GLuint camRightLoc = glGetUniformLocation(pSystem->shader.id(), "worldspace_right");
		glUniform3f(camRightLoc, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

		GLuint texLoc = glGetUniformLocation(pSystem->shader.id(), "particle_texture"); 

		GLuint viewLoc = glGetUniformLocation(pSystem->shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
		GLuint projLoc = glGetUniformLocation(pSystem->shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(myCamera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);

		GLuint mixLoc = glGetUniformLocation(pSystem->shader.id(), "mixRate");
		glUniform1f(mixLoc, pSystem->mixRate);

		std::map<float, Particle*> sorted;
		for (Particle * particle : pSystem->particles) {
			if (particle->alive) {
				float distance = glm::length(camera->pos - particle->position);
				sorted[distance] = particle;
			}
		}
		for (std::map<float, Particle*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
			Particle* particle = it->second;
			glUniform2f(quadLoc, particle->quadInfo.x, particle->quadInfo.y);
			glUniform4f(colorLoc, particle->color.x, particle->color.y, particle->color.z, particle->opacity);
			glUniform1f(sizeLoc, particle->size);
			glUniform1i(texLoc, particle->particle_texture_num);
			glBindVertexArray(particle->vao);
			glDrawArrays(GL_POINTS, 0, 1);
		}

		glDisable(GL_BLEND);
		glBindVertexArray(0);
		glDepthMask(true);
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

	void renderIBL(Imap* map, GameObject* camera) {
		// initialize (if necessary)
		if (cubeVAO == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
																	  // front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
			};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			cubeShader = ShaderProgram("cubeMap.vert", "cubeMap.frag");
		}
		// render Cube
		cubeShader.use();
		glDepthFunc(GL_LEQUAL);
		GLuint viewLoc = glGetUniformLocation(cubeShader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->camera->getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(cubeShader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(camera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);
		GLuint equirectangularMap = glGetUniformLocation(cubeShader.id(), "equirectangularMap");
		glUniform1i(equirectangularMap, map->mapTexNumber);
		GLuint fogLoc = glGetUniformLocation(cubeShader.id(), "fogAmount");
		glUniform1f(fogLoc, fogAmount);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void renderTest() {
		if (testVBO == 0) {
			float points[] = {
				0.0f,  2.0f, 0.0f,
			};
			glGenVertexArrays(1, &testVAO);
			glGenBuffers(1, &testVBO);
			glBindVertexArray(testVAO);
			glBindBuffer(GL_ARRAY_BUFFER, testVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			testShader = ShaderProgram("particleSystem.vert", "particleSystem.geom", "particleSystem.frag");
		}
		testShader.use();
		glDepthFunc(GL_LEQUAL);

		GLuint sizeLoc = glGetUniformLocation(testShader.id(), "radius");
		glUniform1f(sizeLoc, 0.125);
		GLuint camUpLoc = glGetUniformLocation(testShader.id(), "worldspace_up");
		glUniform3f(camUpLoc, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
		GLuint camRightLoc = glGetUniformLocation(testShader.id(), "worldspace_right");
		glUniform3f(camRightLoc, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

		GLuint viewLoc = glGetUniformLocation(testShader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
		GLuint projLoc = glGetUniformLocation(testShader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &glm::perspective(glm::radians(myCamera->camera->fov), (float)1280 / (float)720, 0.1f, 100.0f)[0][0]);

		glBindVertexArray(testVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
};
