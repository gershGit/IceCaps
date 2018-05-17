#include "Headers.h"
#include "GameObject.h"
#include "Drawable.h"

class GLRenderer {

public:

	void render(GameObject* object, GameObject* camera) {
		//std::cout << "Now rendering " << object->name << std::endl;

		/*
		std::cout << "Triangle Coords:" << std::endl;
		std::cout << object->glDrawable->coords[0] << ", " << object->glDrawable->coords[1] << ", " << object->glDrawable->coords[2] << std::endl;
		std::cout << object->glDrawable->coords[6] << ", " << object->glDrawable->coords[7] << ", " << object->glDrawable->coords[8] << std::endl;
		std::cout << object->glDrawable->coords[12] << ", " << object->glDrawable->coords[13] << ", " << object->glDrawable->coords[14] << std::endl;
		std::cout << std::endl;
		
		std::cout << "Transform:" << std::endl;
		std::cout << object->getTransform()[0][0] << ", " << object->getTransform()[1][0] << ", " << object->getTransform()[2][0] << ", " << object->getTransform()[3][0] << std::endl;
		std::cout << object->getTransform()[0][1] << ", " << object->getTransform()[1][1] << ", " << object->getTransform()[2][1] << ", " << object->getTransform()[3][1] << std::endl;
		std::cout << object->getTransform()[0][2] << ", " << object->getTransform()[1][2] << ", " << object->getTransform()[2][2] << ", " << object->getTransform()[3][2] << std::endl;
		std::cout << object->getTransform()[0][3] << ", " << object->getTransform()[1][3] << ", " << object->getTransform()[2][3] << ", " << object->getTransform()[3][3] << std::endl;
		std::cout << std::endl;

		
		std::cout << "Camera Position:" << std::endl;
		std::cout << camera->getTransform()[0][0] << ", " << camera->getTransform()[1][0] << ", " << camera->getTransform()[2][0] << ", " << camera->getTransform()[3][0] << std::endl;
		std::cout << camera->getTransform()[0][1] << ", " << camera->getTransform()[1][1] << ", " << camera->getTransform()[2][1] << ", " << camera->getTransform()[3][1] << std::endl;
		std::cout << camera->getTransform()[0][2] << ", " << camera->getTransform()[1][2] << ", " << camera->getTransform()[2][2] << ", " << camera->getTransform()[3][2] << std::endl;
		std::cout << camera->getTransform()[0][3] << ", " << camera->getTransform()[1][3] << ", " << camera->getTransform()[2][3] << ", " << camera->getTransform()[3][3] << std::endl;
		std::cout << std::endl;

		std::cout << "View Matrix:" << std::endl;
		std::cout << camera->camera->getViewMatrix()[0][0] << ", " << camera->camera->getViewMatrix()[1][0] << ", " << camera->camera->getViewMatrix()[2][0] << ", " << camera->camera->getViewMatrix()[3][0] << std::endl;
		std::cout << camera->camera->getViewMatrix()[0][1] << ", " << camera->camera->getViewMatrix()[1][1] << ", " << camera->camera->getViewMatrix()[2][1] << ", " << camera->camera->getViewMatrix()[3][1] << std::endl;
		std::cout << camera->camera->getViewMatrix()[0][2] << ", " << camera->camera->getViewMatrix()[1][2] << ", " << camera->camera->getViewMatrix()[2][2] << ", " << camera->camera->getViewMatrix()[3][2] << std::endl;
		std::cout << camera->camera->getViewMatrix()[0][3] << ", " << camera->camera->getViewMatrix()[1][3] << ", " << camera->camera->getViewMatrix()[2][3] << ", " << camera->camera->getViewMatrix()[3][3] << std::endl;
		std::cout << std::endl; 

		
		std::cout << "Projection:" << std::endl;
		std::cout << camera->camera->getProjectionMatrix()[0][0] << ", " << camera->camera->getProjectionMatrix()[1][0] << ", " << camera->camera->getProjectionMatrix()[2][0] << ", " << camera->camera->getProjectionMatrix()[3][0] << std::endl;
		std::cout << camera->camera->getProjectionMatrix()[0][1] << ", " << camera->camera->getProjectionMatrix()[1][1] << ", " << camera->camera->getProjectionMatrix()[2][1] << ", " << camera->camera->getProjectionMatrix()[3][1] << std::endl;
		std::cout << camera->camera->getProjectionMatrix()[0][2] << ", " << camera->camera->getProjectionMatrix()[1][2] << ", " << camera->camera->getProjectionMatrix()[2][2] << ", " << camera->camera->getProjectionMatrix()[3][2] << std::endl;
		std::cout << camera->camera->getProjectionMatrix()[0][3] << ", " << camera->camera->getProjectionMatrix()[1][3] << ", " << camera->camera->getProjectionMatrix()[2][3] << ", " << camera->camera->getProjectionMatrix()[3][3] << std::endl;
		std::cout << std::endl;
		

		glm::vec4 pointA(object->glDrawable->coords[0], object->glDrawable->coords[1], object->glDrawable->coords[2], object->glDrawable->coords[3]);
		std::cout << pointA.x << ", " << pointA.y << ", " << pointA.z << ", " << pointA.w << std::endl;

		pointA = glm::vec4(object->glDrawable->coords[0], object->glDrawable->coords[1], object->glDrawable->coords[2], object->glDrawable->coords[3]);
		pointA = pointA * object->getTransform();
		std::cout << pointA.x << ", " << pointA.y << ", " << pointA.z << ", " << pointA.w << std::endl;

		pointA = glm::vec4(object->glDrawable->coords[0], object->glDrawable->coords[1], object->glDrawable->coords[2], object->glDrawable->coords[3]);
		pointA = pointA * object->getTransform() * camera->camera->getViewMatrix();
		std::cout << pointA.x << ", " << pointA.y << ", " << pointA.z << ", " << pointA.w << std::endl;

		pointA = glm::vec4(object->glDrawable->coords[0], object->glDrawable->coords[1], object->glDrawable->coords[2], object->glDrawable->coords[3]);
		pointA = pointA * object->getTransform() * camera->camera->getViewMatrix() * camera->camera->getProjectionMatrix();
		std::cout << pointA.x << ", " << pointA.y << ", " << pointA.z << ", " << pointA.w << std::endl;
		std::cout << std::endl; 
		*/

		GLDrawable* drawableProp = object->glDrawable;
		ShaderProgram shader = drawableProp->material->shader;
		shader.use();

		GLuint loc = glGetUniformLocation(shader.id(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &object->getTransform()[0][0]);
		GLuint viewLoc = glGetUniformLocation(shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->camera->getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera->camera->getProjectionMatrix()[0][0]);

		// render the object
		glBindVertexArray(drawableProp->vao);	
		glDrawArrays(GL_TRIANGLES, 0, drawableProp->coords.size());
		
		for (GameObject* child : object->children) {
			render(child, camera);
		}
	}
};

