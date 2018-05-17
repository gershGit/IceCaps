#pragma once

#include <iostream>
#include "Headers.h"
#include "OpenGlInstance.h"
#include "GameObject.h"
#include "Drawable.h"
#include "Material.h"
#include "Renderer.h"
#include "baseShapeCoords.h"
#include "InputControl.h"
#include "objLoader.h"

InputControl* input;
GLRenderer renderer = GLRenderer();
GameObject* mainCamera = new GameObject();
std::vector<GameObject*> objects;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	float xdif = xpos - mainCamera->camera->lastXpos;
	mainCamera->camera->lastXpos = xpos;
	mainCamera->rot.y -= (xdif / 800) * 2;

	float ydif = ypos - mainCamera->camera->lastYpos;
	mainCamera->camera->lastYpos = ypos;
	mainCamera->rot.x -= (ydif / 600) * 2;
	mainCamera->moved = true;
}

void runUpdates() {
	if (input->isDown(UP_ARROW_KEY)) {
		objects[0]->pos.y += 0.01f;
	}
	if (input->isDown(DOWN_ARROW_KEY)) {
		objects[0]->pos.y -= 0.01f;
	}
	if (input->isDown(LEFT_ARROW_KEY)) {
		objects[0]->pos.x -= 0.01f;
	}
	if (input->isDown(RIGHT_ARROW_KEY)) {
		objects[0]->pos.x += 0.01f;
	}
	if (input->isDown(W_KEY)) {
		//std::cout << "Rotation: " << mainCamera->rot.x << ", " << mainCamera->rot.y << ", " << mainCamera->rot.z << std::endl;
		//std::cout << "Forward: " << mainCamera->forward().x << ", " << mainCamera->forward().y << ", " << mainCamera->forward().z << std::endl;
		mainCamera->pos -= mainCamera->forward() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(S_KEY)) {
		//std::cout << "Rotation: " << mainCamera->rot.x << ", " << mainCamera->rot.y << ", " << mainCamera->rot.z << std::endl;
		//std::cout << "Forward: " << mainCamera->forward().x << ", " << mainCamera->forward().y << ", " << mainCamera->forward().z << std::endl;
		mainCamera->pos += mainCamera->forward() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(A_KEY)) {
		//std::cout << "Rotation: " << mainCamera->rot.x << ", " << mainCamera->rot.y << ", " << mainCamera->rot.z << std::endl;
		//std::cout << "Right: " << mainCamera->right().x << ", " << mainCamera->right().y << ", " << mainCamera->right().z << std::endl;
		mainCamera->pos += mainCamera->right() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(D_KEY)) {
		//std::cout << "Rotation: " << mainCamera->rot.x << ", " << mainCamera->rot.y << ", " << mainCamera->rot.z << std::endl;
		//std::cout << "Right: " << mainCamera->right().x << ", " << mainCamera->right().y << ", " << mainCamera->right().z << std::endl;
		mainCamera->pos -= mainCamera->right() * 0.01f;
		mainCamera->moved = true;
	}
};
void runSimulations() {};
void calculatePhysics() {};
void callIntersections() {};
void renderScene() {
	for (GameObject* object : objects) {
		if (object->drawFlag) {
			double nowTime = fmod(glfwGetTime(),12);
			
			//object->pos.z = 12 -nowTime;
			//object->scale.x = nowTime;
			//object->rot.z = nowTime / 6;
			if (strcmp(object->name, "Parent") == 0) {
				object->rot.y = nowTime;
				object->rot.z = nowTime / 6;
				object->rot.x = nowTime / 2;
				object->moved = true;
			}
			renderer.render(object, mainCamera);
		}
	}
};
void loadScene() {
	//TODO fix so that multiple objects have their own vbo and vao correctly
	GLCamera* mainCam = new GLCamera();
	mainCam->fov = 45;
	mainCam->cameraFlag = true;
	mainCam->ptype = CAMERA;
	mainCam->gameObject = mainCamera;
	mainCamera->camera = mainCam;
	

	//------------Materials--------------
	GLMaterial* tMaterial = new GLMaterial();
	tMaterial->type = PHONG;
	ShaderProgram tShader = ShaderProgram("base.vert", "base.frag");
	tMaterial->shader = tShader;


	//------------Drawables-------------------
	GLDrawable* triangleMesh = new GLDrawable();
	triangleMesh->ptype = DRAWABLE;
	triangleMesh->renderFlag = true;
	triangleMesh->dtype = MESH;
	triangleMesh->coords = {	0.0f, 1.0f, 10.0f,		1.0f, 0.0f, 0.0f,
								1.0f, 0.0f, 10.0f,		0.0f, 1.0f, 0.0f,
								-1.0f, 0.0f, 10.0f,		0.0f, 0.0f, 1.0f};
	triangleMesh->generateBuffers(3, 0, 0);
	triangleMesh->material = tMaterial;

	GLDrawable* planeMesh = new GLDrawable();
	planeMesh->ptype = DRAWABLE;
	planeMesh->renderFlag = true;
	planeMesh->dtype = MESH;
	planeMesh->coords = planeCoords;
	planeMesh->generateBuffers(3, 0, 0);
	planeMesh->material = tMaterial;

	GLDrawable* squareMesh = new GLDrawable();
	squareMesh->ptype = DRAWABLE;
	squareMesh->renderFlag = true;
	squareMesh->dtype = MESH;
	squareMesh->coords = squareCoords;
	squareMesh->generateBuffers(3, 0, 0);
	squareMesh->material = tMaterial;
	
	

	/*
	GLDrawable* suzzane_drawable = new GLDrawable();
	suzzane_drawable->ptype = DRAWABLE;
	suzzane_drawable->renderFlag = true;
	suzzane_drawable->dtype = MESH;
	suzzane_drawable->coords = createCoordVector("someObj.txt");
	suzzane_drawable->generateBuffers(3, 3, 0);
	suzzane_drawable->material = tMaterial;*/


	//-----------Objects------------
	GameObject* childCube = new GameObject();
	childCube->pos.x = 3.0f;
	childCube->scale = glm::vec3(0.5, 0.5, 0.5);
	childCube->name = "Child";
	childCube->properties.push_back(squareMesh);
	childCube->glDrawable = squareMesh;
	childCube->drawFlag = true;

	GameObject* triangle = new GameObject();
	triangle->name = "Tri";
	triangle->drawFlag = true;
	triangle->glDrawable = triangleMesh;
	triangle->properties.push_back(triangleMesh);
	triangle->pos.z = -6;

	GameObject* parentCube = new GameObject();
	parentCube->name = "Parent";
	parentCube->properties.push_back(squareMesh);
	parentCube->glDrawable = squareMesh;
	parentCube->drawFlag = true;
	parentCube->addChild(childCube);
	parentCube->pos.z = -12.0f;

	/*
	GameObject* suzaneHead = new GameObject();
	suzaneHead->name = "sh";
	suzaneHead->properties.push_back(suzzane_drawable);
	suzaneHead->glDrawable = suzzane_drawable;
	suzaneHead->drawFlag = true;
	suzaneHead->pos.x = -3;
	suzaneHead->pos.z = -20;*/
	
	GameObject* ground = new GameObject();
	ground->name = "Ground";
	ground->drawFlag = true;
	ground->glDrawable = planeMesh;
	ground->properties.push_back(planeMesh);
	ground->pos.z = -12.0f;
	ground->pos.y = -5;
	ground->scale = glm::vec3(100, 100, 100);
	

	//-------------Adding objects to list-----------------
	objects.push_back(parentCube);
	objects.push_back(ground);
	objects.push_back(triangle);
	//objects.push_back(suzaneHead);
};

int main()
{
	GLInstance instance;
	instance.initialize();
	instance.createWindow();
	instance.initGlad();
	input = new InputControl(instance.window);
	glfwSetKeyCallback(instance.window, key_callback);
	glfwSetCursorPosCallback(instance.window, cursor_position_callback);
	glfwSetInputMode(instance.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(instance.window, framebuffer_size_callback);

	loadScene();
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_GEQUAL);
	glDepthRange(0.0f, 1.0f);

	// render loop
	while (!glfwWindowShouldClose(instance.window))
	{
		// render
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClearDepth(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//input->setFlags();
		instance.processInput(instance.window);
		runSimulations();
		calculatePhysics();
		runUpdates();
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	input->setFlag(key, scancode, action, mods);
}