#pragma once

#include <iostream>
#include "IrradianceMap.h"
#include "Headers.h"
#include "OpenGlInstance.h"
#include "GameObject.h"
#include "Drawable.h"
#include "Material.h"
#include "Renderer.h"
#include "baseShapeCoords.h"
#include "InputControl.h"
#include "objLoader.h"
#include "iceLoader.h"
#include "RigidBody.h"
#include "ObjectFactory.h"
#include <cstdlib>
#include "NetServer.h"
#include "NetClient.h"

InputControl* input;
std::vector<GameObject*> objects;
std::vector<GameObject*> lights;
double lastTime = 0.0f;
std::vector<const char *> messages;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

DWORD WINAPI createServer(void *data) {
	messages = std::vector<const char *>();
	std::cout << "Attempting to establish a network" << std::endl;
	NetServer myNet = NetServer(&messages);
	int res = myNet.initialize();
	std::cout << "Initialization success? --> " << res << std::endl;
	res = myNet.createServer();
	std::cout << "Creation success? --> " << res << std::endl;
	res = myNet.startListen();
	std::cout << "Listen success? --> " << res << std::endl;
	res = myNet.sendRecieve();
	std::cout << "Send/Recieve success? --> " << res << std::endl;
	return 0;
}
void createServerOnThread() {
	HANDLE thread = CreateThread(NULL, 0, createServer, NULL, 0, NULL);
	if (!thread) {
		printf("Thread creation failed\n");
	}
}

void handleMessages() {
	for (const char * message : messages) {
		printf("Recieved: %s", message);
	}
	messages.clear();
}
void runUpdates() {
	if (input->isDown(UP_ARROW_KEY)) {
		objects[2]->pos.y += 0.01f;
		objects[2]->moved = true;
	}
	if (input->isDown(DOWN_ARROW_KEY)) {
		objects[2]->pos.y -= 0.01f;
		objects[2]->moved = true;
	}
	if (input->isDown(LEFT_ARROW_KEY)) {
		objects[2]->pos.x -= 0.01f;
		objects[2]->moved = true;
	}
	if (input->isDown(RIGHT_ARROW_KEY)) {
		objects[2]->pos.x += 0.01f;
		objects[2]->moved = true;
	}
	if (input->isDown(W_KEY)) {

	}
	if (input->isDown(S_KEY)) {

	}
	if (input->isDown(A_KEY)) {

	}
	if (input->isDown(D_KEY)) {

	}
};
void runSimulations() {};
void calculatePhysics() {
	double newTime = glfwGetTime();
	double timestep = newTime - lastTime;
	lastTime = newTime;
	for (GameObject* object : objects) {
		if (object->usingRigid) {
			object->updatePhysics(timestep);
			object->moved = true;
		}
	}
};
void callIntersections() {
	for (GameObject* object : objects) {
		if (object->usingCollider) {
			object->sCollider->position = object->pos;
			object->checkCollisions(objects);
		}
	}
};
void renderScene() {
	//Server does not render
};
void loadScene() {
	//-----------Objects------------
	GameObject* childCube = new GameObject();
	childCube->pos.x = 3.0f;
	childCube->scale = glm::vec3(0.5, 0.5, 0.5);
	childCube->name = "Child";

	GameObject* parentCube = new GameObject();
	parentCube->name = "Parent";
	parentCube->addChild(childCube);
	parentCube->pos.z = 12.0f;
	parentCube->pos.y = 20.0f;
	RigidBody* squareRigid = new RigidBody();
	squareRigid->setStart(parentCube->pos, glm::vec3(0), glm::vec3(0));
	parentCube->rigidBody = squareRigid;
	parentCube->usingRigid = true;
	SphereCollider* squareCollider = new SphereCollider();
	squareCollider->position = parentCube->pos;
	squareCollider->radius = 1;
	parentCube->usingCollider = true;
	parentCube->sCollider = squareCollider;

	GameObject* suzaneHead = new GameObject();
	suzaneHead->name = "sh";
	suzaneHead->pos.x = -3;
	suzaneHead->pos.z = 20;

	GameObject* ground = new GameObject();
	ground->name = "Ground";
	ground->pos.z = -12.0f;
	ground->pos.y = 0;
	ground->scale = glm::vec3(100, 100, 100);

	ObjectFactory* mObjectFactory = new ObjectFactory();

	GameObject* spawnedSphere = new GameObject();
	spawnedSphere->pos.z = 12;
	spawnedSphere->pos.y = 3;
	SphereCollider* lowSphere = new SphereCollider();
	lowSphere->position = spawnedSphere->pos;
	lowSphere->radius = 1;
	spawnedSphere->usingCollider = true;
	spawnedSphere->sCollider = lowSphere;

	GameObject* spawnedSphere2 = new GameObject();
	spawnedSphere2->name = "SPHERE2";
	spawnedSphere2->pos.z = 12;
	spawnedSphere2->pos.y = 20;
	SphereCollider *highSphere = new SphereCollider();
	highSphere->position = spawnedSphere->pos;
	highSphere->radius = 1;
	spawnedSphere2->usingCollider = true;
	spawnedSphere2->sCollider = highSphere;
	RigidBody *sphere2rigid = new RigidBody();
	sphere2rigid->mass = 1;
	sphere2rigid->is_active = true;
	sphere2rigid->setStart(spawnedSphere2->pos);
	spawnedSphere2->usingRigid = true;
	spawnedSphere2->rigidBody = sphere2rigid;

	GameObject* spawnedLight_0 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(-2, 4, 8), glm::vec3(1.0f, 0.0f, 0.0f), 1.0, true);

	GameObject* spawnedLight_1 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(-2, 4, 14), glm::vec3(0.0f, 1.0f, 0.0f), 1.0, true);

	GameObject* spawnedLight_2 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(2, 2, 11), glm::vec3(0.0f, 0.0f, 1.0f), 1.0, true);

	GameObject* spawnedLight_3 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(3, 3, 16), glm::vec3(1.0f, 0.0f, 1.0f), 1.0, true);

	//-------------Adding objects to list-----------------
	//objects.push_back(parentCube);
	objects.push_back(ground);
	objects.push_back(suzaneHead);
	objects.push_back(spawnedSphere);
	objects.push_back(spawnedSphere2);

	lights.push_back(spawnedLight_0);
	lights.push_back(spawnedLight_1);
	lights.push_back(spawnedLight_2);
	lights.push_back(spawnedLight_3);
};
void callStart(GameObject* parent) {
	parent->onStart();
	for (GameObject* child : parent->children) {
		callStart(child);
	}
}
void startScene() {
	for (GameObject* object : objects) {
		callStart(object);
	}
};

int main()
{
	messages = std::vector<const char *>();
	createServerOnThread();

	GLInstance instance;
	instance.initialize();
	instance.createWindow("IceCaps - Server Running", 720, 1280);
	instance.initGlad();
	input = new InputControl(instance.window);
	glfwSetKeyCallback(instance.window, key_callback);
	glfwSetFramebufferSizeCallback(instance.window, framebuffer_size_callback);

	loadScene();

	lastTime = glfwGetTime();
	startScene();
	// render loop
	while (!glfwWindowShouldClose(instance.window))
	{
		// render
		glClearColor(0.1f, 0.1f, 0.22f, 1.0f);
		//glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//input->setFlags();
		instance.processInput(instance.window);
		handleMessages();
		runSimulations();
		calculatePhysics();
		runUpdates();
		callIntersections();
		renderScene();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
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