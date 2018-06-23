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
#include <string>
#include <cstring>

InputControl* input;
GLRenderer renderer = GLRenderer();
GameObject* mainCamera = new GameObject();
std::vector<GameObject*> objects;
std::vector<GameObject*> lights;
double lastTime = 0.0f;
Imap *envMap;
Imap *irrMap;
std::vector<const char *> messageOutList = std::vector < const char *>();
std::vector<const char *> messageInList = std::vector < const char *>();

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
int compareByCoordSize(const void * d1, const void * d2) {
	GLDrawable* draw1 = *(GLDrawable**)d1;
	GLDrawable* draw2 = *(GLDrawable**)d2;
	if (draw1->coords.size() > draw2->coords.size()) return -1;
	if (draw1->coords.size() == draw2->coords.size()) return 0;
	if (draw1->coords.size() < draw2->coords.size()) return 1;
}

DWORD WINAPI createClient(void *data) {
	std::cout << "Attempting to create a client" << std::endl;
	NetClient myClient = NetClient(&messageInList);
	int res = myClient.Initialize();
	std::cout << "Initialization success? --> " << res << std::endl;
	res = myClient.ConnectSocket();
	std::cout << "Connection success? --> " << res << std::endl;
	std::string myData = "Hello from client!!!";
	res = myClient.SendData(myData.c_str());
	std::cout << "Send success? --> " << res << std::endl;
	res = myClient.Kill();
	std::cout << "Kill success? --> " << res << std::endl;
	return 0;
}
void createClientOnThread() {
	HANDLE thread = CreateThread(NULL, 0, createClient, NULL, 0, NULL);
	if (!thread) {
		printf("Thread creation failed\n");
	}
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
		mainCamera->pos -= mainCamera->forward() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(S_KEY)) {
		mainCamera->pos += mainCamera->forward() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(A_KEY)) {
		mainCamera->pos += mainCamera->right() * 0.01f;
		mainCamera->moved = true;
	}
	if (input->isDown(D_KEY)) {
		mainCamera->pos -= mainCamera->right() * 0.01f;
		mainCamera->moved = true;
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
void sendMessages() {
	for (GameObject* object : objects) {
		if (object->moved) {
			std::string info = object->name;
			messageOutList.push_back(info.c_str());
		}
	}
}
void renderScene() {
	glDepthFunc(GL_LEQUAL);
	for (GameObject* object : objects) {
		if (object->drawFlag) {
			double nowTime = fmod(glfwGetTime(), 12);

			if (strcmp(object->name, "SPHERE") == 0) {
				//object->pos.y = sin(nowTime) + 3.0;
				object->rot.z = nowTime / 6;
				object->rot.x = nowTime / 2;
				object->moved = true;
			}
			renderer.renderObjects(object, mainCamera, lights, irrMap, envMap);
		}
	}
	for (GameObject* object : lights) {
		renderer.renderLights(object, mainCamera);
	}
	renderer.renderIBL(envMap, mainCamera);
};
void loadScene() {
	CoordsSpawner * mCoordSpawner = new CoordsSpawner();
	std::vector<GLDrawable*> toGenerate = std::vector<GLDrawable*>();
	int texture_number = 0;

	GLCamera* mainCam = new GLCamera();
	mainCam->fov = 45;
	mainCam->cameraFlag = true;
	mainCam->ptype = CAMERA;
	mainCam->gameObject = mainCamera;
	mainCamera->camera = mainCam;
	mainCamera->pos.y = 2.8f;

	envMap = new Imap("Textures/Arches_E_PineTree_8k.jpg", texture_number++);
	irrMap = new Imap("Textures/Arches_E_PineTree_Env.hdr", texture_number++);

	//------------Materials--------------
	GLMaterial* sphereMat = new GLMaterial();
	sphereMat->color = glm::vec3(1.00, 0.71, 0.29);
	sphereMat->setMaterialType(DIALECTRIC);

	GLMaterial* goldMat = new GLMaterial();
	goldMat->color = glm::vec3(1.00, 0.71, 0.29);
	goldMat->setMaterialType(METALLIC);

	GLMaterial* gfPBR = new GLMaterial();
	gfPBR->addTexture("Textures/diffuse.png", DIFFUSE, texture_number++);
	gfPBR->addTexture("Textures/metallic.png", METALLIC_MASK, texture_number++);
	gfPBR->addTexture("Textures/roughness.png", ROUGHNESS_MAP, texture_number++);
	gfPBR->addTexture("Textures/ao.png", AO_MAP, texture_number++);
	gfPBR->addTexture("Textures/normal.png", NORMAL_MAP, texture_number++);
	gfPBR->setMaterialType(PBR_BASIC);

	GLMaterial* pMaterial = new GLMaterial();
	pMaterial->type = SIMPLE;
	ShaderProgram pShader = ShaderProgram("base.vert", "base.frag");
	pMaterial->shader = pShader;

	GLMaterial* sMaterial = new GLMaterial();
	sMaterial->type = PHONG_SIMPLE;
	ShaderProgram sShader = ShaderProgram("phongSun.vert", "phongSun.frag");
	sMaterial->shader = sShader;

	GLMaterial* tMaterial = new GLMaterial();
	tMaterial->type = PBR_BASIC;
	int addResult = tMaterial->addTexture("Textures/diffuse.png", DIFFUSE, texture_number++);
	addResult = tMaterial->addTexture("Textures/metallic.png", SPECULAR_MASK, texture_number++);
	addResult = tMaterial->addTexture("Textures/normal.png", NORMAL_MAP, texture_number++);
	ShaderProgram tShader = ShaderProgram("PBR.vert", "PBR.frag");
	tMaterial->shader = tShader;

	GLMaterial* bottomMaterial = new GLMaterial();
	bottomMaterial->type = SIMPLE_DIFFUSE_SPECULAR;
	addResult = bottomMaterial->addTexture("specular.bmp", DIFFUSE, texture_number++);
	addResult = bottomMaterial->addTexture("specular.bmp", SPECULAR_MASK, texture_number++);
	ShaderProgram bShader = ShaderProgram("diffuseSpec.vert", "diffuseSpec.frag");
	bottomMaterial->shader = bShader;

	//------------Drawables-------------------
	GLDrawable* planeMesh = new GLDrawable();
	planeMesh->ptype = DRAWABLE;
	planeMesh->renderFlag = true;
	planeMesh->dtype = MESH;
	planeMesh->usingEBO = true;
	planeMesh->coords = mCoordSpawner->planeCoordsOnly;
	planeMesh->indices = mCoordSpawner->planeIndeces;
	planeMesh->material = gfPBR;
	planeMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(planeMesh);

	GLDrawable* squareMesh = new GLDrawable();
	squareMesh->ptype = DRAWABLE;
	squareMesh->renderFlag = true;
	squareMesh->dtype = MESH;
	squareMesh->usingEBO = true;
	squareMesh->coords = mCoordSpawner->squareCoordsOnly;
	squareMesh->indices = mCoordSpawner->squareIndices;
	squareMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	squareMesh->material = tMaterial;
	toGenerate.push_back(squareMesh);

	std::cout << "Loading suzzane head" << std::endl;
	GLDrawable* suzzane_drawable = new GLDrawable();
	suzzane_drawable->ptype = DRAWABLE;
	suzzane_drawable->renderFlag = true;
	suzzane_drawable->dtype = MESH;
	std::vector<float> coords = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	loadICE("some.ice", coords, indices);
	suzzane_drawable->usingEBO = true;
	suzzane_drawable->coords = coords;
	suzzane_drawable->indices = indices;
	suzzane_drawable->material = sphereMat;
	suzzane_drawable->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(suzzane_drawable);

	//-----------Objects------------
	GameObject* childCube = new GameObject();
	childCube->pos.x = 3.0f;
	childCube->scale = glm::vec3(0.5, 0.5, 0.5);
	childCube->name = "Child";
	childCube->properties.push_back(squareMesh);
	childCube->glDrawable = squareMesh;
	childCube->drawFlag = true;

	/*
	GameObject* triangle = new GameObject();
	triangle->name = "Tri";
	triangle->drawFlag = true;
	triangle->glDrawable = triangleMesh;
	triangle->properties.push_back(triangleMesh);
	triangle->pos.z = -6;*/

	GameObject* parentCube = new GameObject();
	parentCube->name = "Parent";
	parentCube->properties.push_back(squareMesh);
	parentCube->glDrawable = squareMesh;
	parentCube->drawFlag = true;
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
	suzaneHead->properties.push_back(suzzane_drawable);
	suzaneHead->glDrawable = suzzane_drawable;
	suzaneHead->drawFlag = true;
	suzaneHead->pos.x = -3;
	suzaneHead->pos.z = 20;

	GameObject* ground = new GameObject();
	ground->name = "Ground";
	ground->drawFlag = true;
	ground->glDrawable = planeMesh;
	ground->properties.push_back(planeMesh);
	ground->pos.z = -12.0f;
	ground->pos.y = 0;
	ground->scale = glm::vec3(100, 100, 100);

	ObjectFactory* mObjectFactory = new ObjectFactory();

	GameObject* spawnedSphere = mObjectFactory->createObject(SPHERE_PRIMITVE);
	spawnedSphere->glDrawable->material = gfPBR;
	toGenerate.push_back(spawnedSphere->glDrawable);
	spawnedSphere->pos.z = 12;
	spawnedSphere->pos.y = 3;
	SphereCollider* lowSphere = new SphereCollider();
	lowSphere->position = spawnedSphere->pos;
	lowSphere->radius = 1;
	spawnedSphere->usingCollider = true;
	spawnedSphere->sCollider = lowSphere;

	GameObject* spawnedSphere2 = mObjectFactory->createObject(SPHERE_PRIMITVE);
	spawnedSphere2->glDrawable->material = sphereMat;
	spawnedSphere2->name = "SPHERE2";
	toGenerate.push_back(spawnedSphere2->glDrawable);
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
	toGenerate.push_back(spawnedLight_0->glDrawable);

	GameObject* spawnedLight_1 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(-2, 4, 14), glm::vec3(0.0f, 1.0f, 0.0f), 1.0, true);
	toGenerate.push_back(spawnedLight_1->glDrawable);

	GameObject* spawnedLight_2 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(2, 2, 11), glm::vec3(0.0f, 0.0f, 1.0f), 1.0, true);
	toGenerate.push_back(spawnedLight_2->glDrawable);

	GameObject* spawnedLight_3 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(3, 3, 16), glm::vec3(1.0f, 0.0f, 1.0f), 1.0, true);
	toGenerate.push_back(spawnedLight_3->glDrawable);

	qsort(&toGenerate[0], toGenerate.size(), sizeof(GLDrawable*), compareByCoordSize);
	for (GLDrawable* drawable : toGenerate) {
		drawable->generateBuffers();
	}

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

	free(mCoordSpawner);
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
	createClientOnThread();

	std::cout << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS << std::endl;

	GLInstance instance;
	instance.initialize();
	instance.createWindow("IceCaps - Week 2", 720, 1280);
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
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

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