#include <iostream>

#include "Headers.h"
#include "OpenGlInstance.h"
#include "GameObject.h"
#include "Drawable.h"
#include "Material.h"
#include "Renderer.h"
#include "baseShapeCoords.h"

GLRenderer renderer = GLRenderer();
GameObject* mainCamera = new GameObject();
std::vector<GameObject*> objects;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void runSimulations() {};
void calculatePhysics() {};
void callIntersections() {};
void renderScene() {
	for (GameObject* object : objects) {
		if (object->drawFlag) {
			double nowTime = fmod(glfwGetTime(),6);
			
			//object->pos.y += -nowTime;
			//object->pos.x = -nowTime * 6;
			//object->scale.x = nowTime;
			//object->rot.z = nowTime * 6;
			//object->rot.y = nowTime;
			object->moved = true;
			renderer.render(object, mainCamera);
		}
	}
};
void loadScene() {
	GLCamera* mainCam = new GLCamera();
	mainCam->fov = 45;
	mainCam->cameraFlag = true;
	mainCam->ptype = CAMERA;
	mainCam->gameObject = mainCamera;
	mainCamera->camera = mainCam;


	GameObject* singleTriangle = new GameObject();
	singleTriangle->name = "Parent";
	GLMaterial* tMaterial = new GLMaterial();
	tMaterial->type = PHONG;
	Shader tShader = Shader("base.vert", "base.frag");
	tMaterial->shader = tShader;

	GLDrawable* triangleMesh = new GLDrawable();
	triangleMesh->ptype = DRAWABLE;
	triangleMesh->renderFlag = true;
	triangleMesh->dtype = MESH;
	triangleMesh->coords = {	0.0f, 1.0f, 10.0f,		1.0f, 0.0f, 0.0f,
								1.0f, 0.0f, 10.0f,		0.0f, 1.0f, 0.0f,
								-1.0f, 0.0f, 10.0f,		0.0f, 0.0f, 1.0f};
	triangleMesh->generateBuffers();
	triangleMesh->material = tMaterial;

	GLDrawable* squareMesh = new GLDrawable();
	squareMesh->ptype = DRAWABLE;
	squareMesh->renderFlag = true;
	squareMesh->dtype = MESH;
	squareMesh->coords = squareCoords;
	squareMesh->generateBuffers();
	squareMesh->material = tMaterial;
	
	singleTriangle->properties.push_back(squareMesh);
	singleTriangle->glDrawable = squareMesh;
	singleTriangle->drawFlag = true;

	GameObject* childTriangle = new GameObject();
	childTriangle->pos.x = 3.0f;
	childTriangle->scale.x = 0.5;
	childTriangle->name = "Child";
	childTriangle->properties.push_back(triangleMesh);
	childTriangle->glDrawable = triangleMesh;
	childTriangle->drawFlag = true;

	singleTriangle->addChild(childTriangle);
	singleTriangle->pos.z = 6.0f;

	objects.push_back(singleTriangle);
};

int main()
{
	GLInstance instance;
	instance.initialize();
	instance.createWindow();
	instance.initGlad();
	glfwSetFramebufferSizeCallback(instance.window, framebuffer_size_callback);

	loadScene();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// render loop
	while (!glfwWindowShouldClose(instance.window))
	{
		instance.processInput(instance.window);

		// render
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		runSimulations();
		calculatePhysics();
		callIntersections();
		renderScene();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(instance.window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}