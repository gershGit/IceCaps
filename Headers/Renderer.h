#include "Headers.h"
#include "GameObject.h"
#include "Drawable.h"

class GLRenderer {

public:

	void render(GameObject* object, GameObject& camera) {
		//std::cout << "Now rendering " << object.name << std::endl;

		GLDrawable drawableProp = (GLDrawable) object->glDrawable;
		Shader shader = drawableProp.material.shader;
		shader.use();
		GLuint loc = glGetUniformLocation(shader.id(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &object->getTransform()[0][0]);
		GLuint viewLoc = glGetUniformLocation(shader.id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera.camera.getViewMatrix()[0][0]);
		GLuint projLoc = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera.camera.getProjectionMatrix()[0][0]);

		/*
		std::cout << object.getTransform()[0][0] << ", " << object.getTransform()[1][0] << ", " << object.getTransform()[2][0] << ", " << object.getTransform()[3][0] << std::endl;
		std::cout << object.getTransform()[0][1] << ", " << object.getTransform()[1][1] << ", " << object.getTransform()[2][1] << ", " << object.getTransform()[3][1] << std::endl;
		std::cout << object.getTransform()[0][2] << ", " << object.getTransform()[1][2] << ", " << object.getTransform()[2][2] << ", " << object.getTransform()[3][2] << std::endl;
		std::cout << object.getTransform()[0][3] << ", " << object.getTransform()[1][3] << ", " << object.getTransform()[2][3] << ", " << object.getTransform()[3][3] << std::endl;
		std::cout << std::endl;*/

		/*
		std::cout << camera.camera.getViewMatrix()[0][0] << ", " << camera.camera.getViewMatrix()[1][0] << ", " << camera.camera.getViewMatrix()[2][0] << ", " << camera.camera.getViewMatrix()[3][0] << std::endl;
		std::cout << camera.camera.getViewMatrix()[0][1] << ", " << camera.camera.getViewMatrix()[1][1] << ", " << camera.camera.getViewMatrix()[2][1] << ", " << camera.camera.getViewMatrix()[3][1] << std::endl;
		std::cout << camera.camera.getViewMatrix()[0][2] << ", " << camera.camera.getViewMatrix()[1][2] << ", " << camera.camera.getViewMatrix()[2][2] << ", " << camera.camera.getViewMatrix()[3][2] << std::endl;
		std::cout << camera.camera.getViewMatrix()[0][3] << ", " << camera.camera.getViewMatrix()[1][3] << ", " << camera.camera.getViewMatrix()[2][3] << ", " << camera.camera.getViewMatrix()[3][3] << std::endl;
		std::cout << std::endl; */

		/*
		std::cout << camera.camera.getProjectionMatrix()[0][0] << ", " << camera.camera.getProjectionMatrix()[1][0] << ", " << camera.camera.getProjectionMatrix()[2][0] << ", " << camera.camera.getProjectionMatrix()[3][0] << std::endl;
		std::cout << camera.camera.getProjectionMatrix()[0][1] << ", " << camera.camera.getProjectionMatrix()[1][1] << ", " << camera.camera.getProjectionMatrix()[2][1] << ", " << camera.camera.getProjectionMatrix()[3][1] << std::endl;
		std::cout << camera.camera.getProjectionMatrix()[0][2] << ", " << camera.camera.getProjectionMatrix()[1][2] << ", " << camera.camera.getProjectionMatrix()[2][2] << ", " << camera.camera.getProjectionMatrix()[3][2] << std::endl;
		std::cout << camera.camera.getProjectionMatrix()[0][3] << ", " << camera.camera.getProjectionMatrix()[1][3] << ", " << camera.camera.getProjectionMatrix()[2][3] << ", " << camera.camera.getProjectionMatrix()[3][3] << std::endl;
		std::cout << std::endl;*/

		// render the object
		glBindVertexArray(drawableProp.vao);
		glDrawArrays(GL_TRIANGLES, 0, drawableProp.coords.size());
		
		//TODO fix children positioning and rendering
		for (GameObject* child : object->children) {
			render(child, camera);
		}
	}
};

