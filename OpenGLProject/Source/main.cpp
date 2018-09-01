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
#include <mutex>
#include <functional>
#include "GameTimer.h"
#include "ParticleSystem.h"

InputControl* input;
GLRenderer renderer = GLRenderer();
GameObject* mainCamera = new GameObject();
std::vector<GameObject*> objects;
std::vector<GameObject*> online_playerObjects;
std::vector<ParticleSystem*> pSystems;
std::vector<GameObject*> lights;
double lastTime = 0.0f;
Imap *envMap;
Imap *irrMap;
std::vector<std::string> messageOutList = std::vector <std::string>();
std::vector<std::string> messageInList = std::vector < std::string>();
std::mutex messageIn_mutex;
std::mutex messageOut_mutex;
NetClient myClient = NetClient(&messageInList, &messageOutList, &messageIn_mutex, &messageOut_mutex);
int trackingInt = 0;
ObjectFactory* mObjectFactory;
GameTimer timer;

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
	return 0;
}

DWORD WINAPI createClientReceiver(void *data) {
	std::cout << "Attempting to create a client" << std::endl;
	NetClient myThreadedClient = NetClient(&messageInList, &messageOutList, &messageIn_mutex, &messageOut_mutex);
	int res = myThreadedClient.Initialize();
	std::cout << "Initialization success? --> " << res << std::endl;
	res = myThreadedClient.receiveLoop();
	return 0;
}
void createClientOnThread() {
	std::cout << "Attempting to create a client" << std::endl;
	myClient = NetClient(&messageInList, &messageOutList, &messageIn_mutex, &messageOut_mutex);
	int res = myClient.Initialize();
	std::cout << "Initialization success? --> " << res << std::endl;

	HANDLE thread = CreateThread(NULL, 0, createClientReceiver, NULL, 0, NULL);

	res = myClient.ConnectSocket();
	std::cout << "Connection success? --> " << res << std::endl;
	std::string myData = "IP: <this ip>";
	res = myClient.SendTest(myData.c_str());
	std::cout << "Send success? --> " << res << std::endl;
}

void handleMessage(std::string message) {
	if (message[0] == '+') {
		std::cout << "Creating object" << std::endl;
	}
}

void handleMessages(){
	std::lock_guard<std::mutex> lock(messageIn_mutex);
	for (std::string message : messageInList) {
		handleMessage(message);
		std::cout << "Handling: " << message << std::endl;
	}
	messageInList.clear();
}
void runUpdates() {
	if (input->isDown(UP_ARROW_KEY)) {
		objects[2]->pos.y += 6.0f* timer.GetDeltaTime();
		objects[2]->moved = true;
	}
	if (input->isDown(DOWN_ARROW_KEY)) {
		objects[2]->pos.y -= 6.0f* timer.GetDeltaTime();
		objects[2]->moved = true;
	}
	if (input->isDown(LEFT_ARROW_KEY)) {
		objects[2]->pos.x -= 6.0f * timer.GetDeltaTime();
		objects[2]->moved = true;
	}
	if (input->isDown(RIGHT_ARROW_KEY)) {
		objects[2]->pos.x += 6.0f* timer.GetDeltaTime();
		objects[2]->moved = true;
	}
	if (input->isDown(W_KEY)) {
		mainCamera->pos += mainCamera->forward() * 6.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(S_KEY)) {
		mainCamera->pos -= mainCamera->forward() * 6.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(A_KEY)) {
		mainCamera->pos += mainCamera->right() * 6.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(D_KEY)) {
		mainCamera->pos -= mainCamera->right() * 6.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(P_KEY)) {
		myClient.SendTest("New Test");
	}
	if (input->isDown(SPACE_KEY)) {
		if (trackingInt == 0) {
			GameObject* nBall = mObjectFactory->createObject(SPHERE_PRIMITVE, trackingInt++);
			nBall->pos = mainCamera->pos;
			nBall->moved = true;
			nBall->name = "nBALL";
			RigidBody *sphere3rigid = new RigidBody();
			sphere3rigid->mass = 1;
			sphere3rigid->is_active = true;
			sphere3rigid->setStart(nBall->pos, mainCamera->forward()*30.0f);
			nBall->usingRigid = true;
			nBall->rigidBody = sphere3rigid;
			SphereCollider *coll = new SphereCollider();
			coll->position = nBall->pos;
			coll->radius = 1;
			nBall->sCollider = coll;
			nBall->usingCollider = true;
			nBall->glDrawable->generateBuffers();
			nBall->onStart();
			objects.push_back(nBall);
			std::string netInfo = "+ " + std::to_string(nBall->globalId) + " @ " + std::to_string(nBall->pos.x) + " " + std::to_string(nBall->pos.y) + " " + std::to_string(nBall->pos.z) + "\n\t";
			netInfo += "R: " + std::to_string(mainCamera->forward().x*3) + " " + std::to_string(mainCamera->forward().y * 3) + " " + std::to_string(mainCamera->forward().z * 3);
			messageOutList.push_back(netInfo);
		}
	}
};
void runSimulations() {
	for (ParticleSystem* system : pSystems) {
		system->update();
	}
};
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
	std::lock_guard<std::mutex> lock(messageOut_mutex);
	for (GameObject* object : online_playerObjects) {
		if (object->moved) {
			std::string info = std::string(object->name);
			info += " " + std::to_string(object->pos.x) + " " + std::to_string(object->pos.y) + " " + std::to_string(object->pos.z) + "\n";
			messageOutList.push_back(info);
		}
	}
	myClient.SendData();
}
void renderScene() {
	glDepthFunc(GL_LEQUAL);
	renderer.viewMatrix = mainCamera->camera->getViewMatrix();
	for (GameObject* object : objects) {
		if (object->drawFlag) {
			renderer.renderObjects(object, mainCamera, lights, irrMap, envMap);
		}
	}
	for (GameObject* object : lights) {
		renderer.renderLights(object, mainCamera);
	}
	renderer.renderIBL(envMap, mainCamera);
	for (ParticleSystem* system : pSystems) {
		renderer.renderParticleSystem(system, mainCamera, lights);
	}
};
void loadScene() {
	timer = GameTimer();
	CoordsSpawner * mCoordSpawner = new CoordsSpawner();
	std::vector<GLDrawable*> toGenerate = std::vector<GLDrawable*>();
	int texture_number = 0;
	mObjectFactory = new ObjectFactory();

	GLCamera* mainCam = new GLCamera();
	mainCam->fov = 45;
	mainCam->cameraFlag = true;
	mainCam->ptype = CAMERA;
	mainCam->gameObject = mainCamera;
	mainCamera->camera = mainCam;
	mainCamera->pos.y = 2.8f;
	renderer.myCamera = mainCamera;

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

	GLMaterial * roboMaterial = new GLMaterial();
	roboMaterial->addTexture("Textures/robo_diffuse.png", DIFFUSE, texture_number++);
	roboMaterial->addTexture("Textures/robo_metallic.png", METALLIC_MASK, texture_number++);
	roboMaterial->addTexture("Textures/robo_roughness.png", ROUGHNESS_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_ao.png", AO_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_normal.png", NORMAL_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_emissive.png", EMISSION_MAP, texture_number++);
	roboMaterial->setMaterialType(PBR_EMISSIVE);

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
	planeMesh->indices.push_back(mCoordSpawner->planeIndeces);
	planeMesh->material = gfPBR;
	planeMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(planeMesh);

	GLDrawable* roboMesh = new GLDrawable();
	roboMesh->ptype = DRAWABLE;
	roboMesh->renderFlag = true;
	roboMesh->dtype = MESH;
	std::vector<float> coords_robo = std::vector<float>();
	std::vector<unsigned int> indices_robo = std::vector<unsigned int>();
	roboMesh->indices.push_back(std::vector<unsigned int>());
	loadICE("robo.ice", roboMesh->coords, roboMesh->indices[0]);
	roboMesh->LODs = 0;
	roboMesh->usingEBO = true;
	roboMesh->material = roboMaterial;
	roboMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(roboMesh);

	GLDrawable* squareMesh = new GLDrawable();
	squareMesh->ptype = DRAWABLE;
	squareMesh->renderFlag = true;
	squareMesh->dtype = MESH;
	squareMesh->usingEBO = true;
	squareMesh->indices.push_back(std::vector<unsigned int>());
	squareMesh->coords = mCoordSpawner->squareCoordsOnly;
	squareMesh->indices[0] = mCoordSpawner->squareIndices;
	squareMesh->LODs = 0;
	squareMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	squareMesh->material = gfPBR;
	toGenerate.push_back(squareMesh);

	std::cout << "Loading suzzane head" << std::endl;
	GLDrawable* suzzane_drawable = new GLDrawable();
	suzzane_drawable->ptype = DRAWABLE;
	suzzane_drawable->renderFlag = true;
	suzzane_drawable->dtype = MESH;
	std::vector<float> coords = std::vector<float>();
	suzzane_drawable->indices.push_back(std::vector<unsigned int>());
	loadICE("some.ice", suzzane_drawable->coords, suzzane_drawable->indices[0]);
	suzzane_drawable->LODs = 0;
	suzzane_drawable->usingEBO = true;
	suzzane_drawable->material = sphereMat;
	suzzane_drawable->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(suzzane_drawable);

	//-----------Objects------------
	GameObject * lodTest = loadICEasGameObject("lod_test.ice");
	lodTest->pos = glm::vec3(2, 4, 10);
	lodTest->glDrawable->LOD_distances.push_back(5.0f);
	lodTest->glDrawable->LOD_distances.push_back(12.0f);
	lodTest->glDrawable->LOD_distances.push_back(INFINITY);
	toGenerate.push_back(lodTest->glDrawable);

	ParticleSystem * mySystem = new ParticleSystem(&timer);
	mySystem->setAcceleration(glm::vec3(0, -0.125, 0));
	mySystem->setStartVelocity(glm::vec3(0, 0.5, 0));
	mySystem->setLifeTime(25.0f);
	mySystem->setParticleStartSize(6.0f);
	mySystem->setRandomness(2.0);
	mySystem->setSpawnRate(20);
	mySystem->setSpawnTime(2.0f);
	mySystem->moved = true;
	mySystem->setTexture("Textures/smoke.png", texture_number++);
	mySystem->shader = ShaderProgram("particleSystem.vert", "particleSystem.geom", "particleSystem.frag");

	ParticleSystem * fireSystem = new ParticleSystem(&timer);
	fireSystem->pos = glm::vec3(2, 0.1, -25);
	fireSystem->moved = true;
	fireSystem->setType(FIRE, texture_number++);

	HeightMap * myMap = new HeightMap;
	GameObject* myTerrain = mObjectFactory->createTerrainSaveMap(100, 40, 30, "Textures/heightmap_hq.png", myMap);
	myTerrain->glDrawable->material = sphereMat;
	myTerrain->moved = true;
	toGenerate.push_back(myTerrain->glDrawable);

	GameObject* childCube = new GameObject();
	childCube->pos.x = 3.0f;
	childCube->scale = glm::vec3(0.5, 0.5, 0.5);
	childCube->name = "Child";
	childCube->properties.push_back(squareMesh);
	childCube->glDrawable = squareMesh;
	childCube->drawFlag = true;


	GameObject* parentCube = new GameObject();
	parentCube->name = "Parent";
	parentCube->properties.push_back(squareMesh);
	parentCube->glDrawable = squareMesh;
	parentCube->drawFlag = true;
	parentCube->addChild(childCube);
	parentCube->pos.z = 12.0f;
	parentCube->pos.x = -8.0f;
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
	suzaneHead->name = "SUZANNE HEAD";
	suzaneHead->properties.push_back(suzzane_drawable);
	suzaneHead->glDrawable = suzzane_drawable;
	suzaneHead->drawFlag = true;
	suzaneHead->pos.x = -3;
	suzaneHead->pos.z = 20;

	GameObject* robo1 = new GameObject();
	robo1->name = "Player_Model";
	robo1->glDrawable = roboMesh;
	robo1->drawFlag = true;
	robo1->pos.x = 3;
	robo1->pos.z = 20;
	robo1->pos.y = 4;

	GameObject* ground = new GameObject();
	ground->name = "Ground";
	ground->drawFlag = true;
	ground->glDrawable = planeMesh;
	ground->properties.push_back(planeMesh);
	ground->pos.z = -12.0f;
	ground->pos.y = 0;
	ground->scale = glm::vec3(100, 100, 100);

	mObjectFactory = new ObjectFactory();

	GameObject* spawnedSphere = mObjectFactory->createObject(SPHERE_PRIMITVE);
	spawnedSphere->glDrawable->material = gfPBR;
	toGenerate.push_back(spawnedSphere->glDrawable);
	mObjectFactory->addFoliage(spawnedSphere, 0, 12, myMap);
	SphereCollider* lowSphere = new SphereCollider();
	lowSphere->position = spawnedSphere->pos;
	lowSphere->radius = 1;
	spawnedSphere->usingCollider = true;
	spawnedSphere->sCollider = lowSphere;
	spawnedSphere->online_playerOwned = true;

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

	GameObject* player1_bottom = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(0, -0.9, 0), glm::vec3(1.0f, 1.0f, 0.0f), 1.0, true);
	robo1->addChild(player1_bottom);
	toGenerate.push_back(player1_bottom->glDrawable);

	GameObject* player1_back = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(0, -0.25, -0.85), glm::vec3(1.0f, 0.0f, 0.0f), 1.0, true);
	robo1->addChild(player1_back);
	toGenerate.push_back(player1_back->glDrawable);

	qsort(&toGenerate[0], toGenerate.size(), sizeof(GLDrawable*), compareByCoordSize);
	for (GLDrawable* drawable : toGenerate) {
		drawable->generateBuffers();
	}

	//-------------Adding objects to list-----------------
	//objects.push_back(myTerrain);
	//objects.push_back(parentCube);
	objects.push_back(ground);
	//objects.push_back(suzaneHead);
	//objects.push_back(spawnedSphere);
	//objects.push_back(spawnedSphere2);
	objects.push_back(robo1);
	objects.push_back(lodTest);

	//pSystems.push_back(mySystem);
	pSystems.push_back(fireSystem);

	lights.push_back(spawnedLight_0);
	lights.push_back(spawnedLight_1);
	//lights.push_back(spawnedLight_2);
	//lights.push_back(spawnedLight_3);
	lights.push_back(player1_bottom);
	lights.push_back(player1_back);

	free(mCoordSpawner);
	for (GameObject* obj : objects) {
		if (obj->online_playerOwned) {
			online_playerObjects.push_back(obj);
		}
	}
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
	for (ParticleSystem* system : pSystems) {
		system->onStart();
	}
	timer.Start();
};

int main()
{
	createClientOnThread();

	std::cout << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS << std::endl;

	GLInstance instance;
	instance.initialize();
	instance.createWindow("IceCaps - Week 9", 720, 1280);
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
		handleMessages();
		instance.processInput(instance.window);
		timer.Update();
		runUpdates();
		runSimulations();
		calculatePhysics();
		callIntersections();
		sendMessages();
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