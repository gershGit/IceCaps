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
#include "NetServer.h"
#include "NetClient.h"
#include <string>
#include <cstring>
#include <mutex>
#include <functional>
#include "GameTimer.h"
#include "ParticleSystem.h"

bool tempSwitch = false;

InputControl* input;
GLRenderer renderer = GLRenderer();
GameObject* mainCamera = new GameObject();
std::vector<GameObject*> objects;
std::vector<GameObject*> online_playerObjects;
std::vector<ParticleSystem*> pSystems;
std::vector<GameObject*> lights;
std::vector<GLDrawable*> drawables;
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

float lastSpawnTime;

float rand_float_11() {
	return (((float)rand() / (RAND_MAX + 1)) - 0.5f) * 2;
}
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
	/*if (timer.GetTotalElapsed() - lastSpawnTime > 0.125f) {
		GameObject* nBall = new GameObject();
		nBall->glDrawable = drawables[rand() % 8];
		nBall->drawFlag = true;
		nBall->pos = glm::vec3(rand_float_11()*10.0f, 1.1f, rand_float_11()*10.0f);
		nBall->moved = true;
		nBall->name = "nBALL";
		nBall->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		RigidBody *sphere3rigid = new RigidBody();
		sphere3rigid->mass = 1;
		sphere3rigid->is_active = true;
		sphere3rigid->setStart(nBall->pos, glm::vec3(rand_float_11()*5.0f, (rand_float_11() + 1.0f) * 15, rand_float_11()*5.0f));
		nBall->usingRigid = true;
		nBall->rigidBody = sphere3rigid; 
		nBall->onStart();
		objects.push_back(nBall);
		lastSpawnTime = timer.GetTotalElapsed();
	}*/
	if (timer.GetTotalElapsed() > 2 && !tempSwitch) {
		tempSwitch = true;
		for (GameObject * object : objects) {
			object->drawFlag = true;
		}
	}
	if (input->isDown(UP_ARROW_KEY)) {
		objects[1]->pos.y += 2.0f* timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(DOWN_ARROW_KEY)) {
		objects[1]->pos.y -= 6.0f* timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(LEFT_ARROW_KEY)) {
		objects[1]->pos.x -= 6.0f * timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(RIGHT_ARROW_KEY)) {
		objects[1]->pos.x += 6.0f* timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(P_KEY)) {
		objects[1]->pos.z -= 6.0f * timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(O_KEY)) {
		objects[1]->pos.z += 6.0f* timer.GetDeltaTime();
		objects[1]->moved = true;
	}
	if (input->isDown(W_KEY)) {
		mainCamera->pos += mainCamera->forward() * 2.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(S_KEY)) {
		mainCamera->pos -= mainCamera->forward() * 2.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(A_KEY)) {
		mainCamera->pos += mainCamera->right() * 2.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	}
	if (input->isDown(D_KEY)) {
		mainCamera->pos -= mainCamera->right() * 2.0f * (float)timer.GetDeltaTime();
		mainCamera->moved = true;
	} 
	//if (input->isDown(P_KEY)) {
	//	myClient.SendTest("New Test");
	//}
	if (input->isDown(SPACE_KEY)) {
		
		//std::cout << "Generated object" << std::endl;
		/*if (trackingInt == 0) {
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
		}*/
	}
	
	//mainCamera->pos.x = sin(timer.GetTotalElapsed() * 0.5) * 50;
	//mainCamera->pos.z = cos(timer.GetTotalElapsed() * 0.5) * 50;
	//mainCamera->rot.y = timer.GetTotalElapsed() * 0.5f;
	//mainCamera->moved = true;

	//std::cout << "Camera Positions" << std::endl;
	//std::cout << "\t Pos X: " << mainCamera->pos.x << ", Y: " << mainCamera->pos.y << " , Z: " << mainCamera->pos.z << std::endl;
	//std::cout << "\t Rot X: " << mainCamera->rot.x << ", Y: " << mainCamera->rot.y << " , Z: " << mainCamera->rot.z << std::endl;
	
	//std::cout << "Object 1 Positions" << std::endl;
	//std::cout << "\t Pos X: " << objects[1]->pos.x << ", Y: " << objects[1]->pos.y << " , Z: " << objects[1]->pos.z << std::endl;
	//-6.7 2.5 -21.54
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
	renderer.fogAmount = 1.0f;

	std::vector<GameObject*> renderList;
	renderer.cullObjects(&objects, &renderList);

	for (GameObject* object : renderList) {
		if (object->drawFlag) {
			if (object->name != "Ground") {
				//object->rot.y += 1.0f * timer.GetDeltaTime();
				object->moved = true;
			}
			renderer.renderObjects(object, mainCamera, lights, irrMap, envMap);
		}
	}
	for (GameObject* object : lights) {
		object->pos.y = sin(timer.GetOurCurrentTime())*3+3;
		renderer.renderLights(object, mainCamera);
	}
	//renderer.renderIBL(envMap, mainCamera);
	for (ParticleSystem* system : pSystems) {
		renderer.renderParticleSystem(system, mainCamera, lights);
	}
};
void loadScene() {
	lastSpawnTime = 0.0f;
	srand(95);
	timer = GameTimer();
	CoordsSpawner * mCoordSpawner = new CoordsSpawner();
	std::vector<GLDrawable*> toGenerate = std::vector<GLDrawable*>();
	drawables = std::vector<GLDrawable*>();
	int texture_number = 0;
	mObjectFactory = new ObjectFactory();

	GLCamera* mainCam = new GLCamera();
	mainCam->fov = 45;
	mainCam->cameraFlag = true;
	mainCam->ptype = CAMERA;
	mainCam->gameObject = mainCamera;
	mainCamera->camera = mainCam;
	mainCamera->pos.x = 8.83145;
	mainCamera->pos.y = 2.4324;
	mainCamera->pos.z = -12.7533;

	mainCamera->rot.x = -6.17331;
	mainCamera->rot.y = -5.765;
	mainCamera->rot.z = 0;
	//mainCamera->rot.y = -3.14159f;
	renderer.myCamera = mainCamera;

	envMap = new Imap("../../../../Demo/Textures/Arches_E_PineTree_8k.jpg", texture_number++);
	irrMap = new Imap("../../../../Demo/Textures/Arches_E_PineTree_Env.hdr", texture_number++);

	//------------Materials--------------
	GLMaterial* ground_material = new GLMaterial();
	//ground_material->color = glm::vec3(0.43137254902, 0.7725490196, 0.91372549019);
	ground_material->color = glm::vec3(0.1f, 0.1f, 0.1f);
	ground_material->setMaterialType(SIMPLE);

	
	GLMaterial* mat0 = new GLMaterial();
	mat0->color = glm::vec3(0.9, 0.85, 0.6);
	mat0->setMaterialType(SIMPLE);

	GLMaterial* mat1 = new GLMaterial();
	mat1->color = glm::vec3(0.9, 0.85, 0.6);
	mat1->setMaterialType(PHONG_SIMPLE);

	GLMaterial* mat2 = new GLMaterial();
	mat2->color = glm::vec3(0.9, 0.85, 0.6);
	mat2->setMaterialType(PHONG);

	GLMaterial* mat4 = new GLMaterial();
	mat4->addTexture("../../../../Demo/Textures/DiffSpec/diffuse.png", DIFFUSE, texture_number++);
	mat4->addTexture("../../../../Demo/Textures/DiffSpec/specular.png", SPECULAR_MASK, texture_number++);
	mat4->setMaterialType(PHONG_TEXTURED);

	/*
	GLMaterial* dialectric_material = new GLMaterial();
	dialectric_material->color = glm::vec3(1.00, 0.71, 0.29);
	dialectric_material->setMaterialType(DIALECTRIC);

	GLMaterial* metallic_material = new GLMaterial();
	metallic_material->color = glm::vec3(1.00, 0.71, 0.29);
	metallic_material->setMaterialType(METALLIC);

	GLMaterial* gfPBR = new GLMaterial();
	gfPBR->addTexture("Textures/diffuse.png", DIFFUSE, texture_number++);
	gfPBR->addTexture("Textures/metallic.png", METALLIC_MASK, texture_number++);
	gfPBR->addTexture("Textures/roughness.png", ROUGHNESS_MAP, texture_number++);
	gfPBR->addTexture("Textures/ao.png", AO_MAP, texture_number++);
	gfPBR->addTexture("Textures/normal.png", NORMAL_MAP, texture_number++);
	gfPBR->setMaterialType(PBR_BASIC);

	GLMaterial* baseMaterial = new GLMaterial();
	baseMaterial->type = SIMPLE;
	ShaderProgram pShader = ShaderProgram("base.vert", "base.frag");
	baseMaterial->shader = pShader;

	GLMaterial* phongMaterial = new GLMaterial();
	phongMaterial->type = PHONG_SIMPLE;
	ShaderProgram sShader = ShaderProgram("phongSun.vert", "phongSun.frag");
	phongMaterial->shader = sShader;

	GLMaterial * roboMaterial = new GLMaterial();
	roboMaterial->addTexture("Textures/robo_diffuse.png", DIFFUSE, texture_number++);
	roboMaterial->addTexture("Textures/robo_metallic.png", METALLIC_MASK, texture_number++);
	roboMaterial->addTexture("Textures/robo_roughness.png", ROUGHNESS_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_ao.png", AO_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_normal.png", NORMAL_MAP, texture_number++);
	roboMaterial->addTexture("Textures/robo_emissive.png", EMISSION_MAP, texture_number++);
	roboMaterial->setMaterialType(PBR_BASIC);*/

	GLMaterial * iceMaterial = new GLMaterial();
	iceMaterial->addTexture("../../../../Demo/Textures/IceEarth/diffuse.png", DIFFUSE, texture_number++);
	iceMaterial->addTexture("../../../../Demo/Textures/IceEarth/metallic.png", METALLIC_MASK, texture_number++);
	iceMaterial->addTexture("../../../../Demo/Textures/IceEarth/roughness.png", ROUGHNESS_MAP, texture_number++);
	iceMaterial->addTexture("../../../../Demo/Textures/IceEarth/ao.png", AO_MAP, texture_number++);
	iceMaterial->addTexture("../../../../Demo/Textures/IceEarth/normal.png", NORMAL_MAP, texture_number++);
	iceMaterial->setMaterialType(PBR_SIMPLE);
	
	GLMaterial * pbrExample = new GLMaterial();
	pbrExample->addTexture("../../../../Demo/Textures/PBR/diffuse.png", DIFFUSE, texture_number++);
	pbrExample->addTexture("../../../../Demo/Textures/PBR/metallic.png", METALLIC_MASK, texture_number++);
	pbrExample->addTexture("../../../../Demo/Textures/PBR/roughness.png", ROUGHNESS_MAP, texture_number++);
	pbrExample->addTexture("../../../../Demo/Textures/PBR/ao.png", AO_MAP, texture_number++);
	pbrExample->addTexture("../../../../Demo/Textures/PBR/normal.png", NORMAL_MAP, texture_number++);
	pbrExample->setMaterialType(PBR_SIMPLE);
	
	GLMaterial * pbrExampleBasic = new GLMaterial();
	pbrExampleBasic->addTexture("../../../../Demo/Textures/PBR/diffuse.png", DIFFUSE, texture_number++);
	pbrExampleBasic->addTexture("../../../../Demo/Textures/PBR/metallic.png", METALLIC_MASK, texture_number++);
	pbrExampleBasic->addTexture("../../../../Demo/Textures/PBR/roughness.png", ROUGHNESS_MAP, texture_number++);
	pbrExampleBasic->addTexture("../../../../Demo/Textures/PBR/ao.png", AO_MAP, texture_number++);
	pbrExampleBasic->addTexture("../../../../Demo/Textures/PBR/normal.png", NORMAL_MAP, texture_number++);
	pbrExampleBasic->setMaterialType(PBR_BASIC);

	GLMaterial * pbrExampleDialectric = new GLMaterial();
	pbrExampleDialectric->addTexture("../../../../Demo/Textures/Dialectric/diffuse.png", DIFFUSE, texture_number++);
	pbrExampleDialectric->addTexture("../../../../Demo/Textures/Dialectric/metallic.png", METALLIC_MASK, texture_number++);
	pbrExampleDialectric->addTexture("../../../../Demo/Textures/Dialectric/roughness.png", ROUGHNESS_MAP, texture_number++);
	pbrExampleDialectric->addTexture("../../../../Demo/Textures/Dialectric/ao.png", AO_MAP, texture_number++);
	pbrExampleDialectric->addTexture("../../../../Demo/Textures/Dialectric/normal.png", NORMAL_MAP, texture_number++);
	pbrExampleDialectric->setMaterialType(PBR_BASIC);

	GLMaterial * pbrExampleMettallic = new GLMaterial();
	pbrExampleMettallic->addTexture("../../../../Demo/Textures/Metallic/diffuse.png", DIFFUSE, texture_number++);
	pbrExampleMettallic->addTexture("../../../../Demo/Textures/Metallic/metallic.png", METALLIC_MASK, texture_number++);
	pbrExampleMettallic->addTexture("../../../../Demo/Textures/Metallic/roughness.png", ROUGHNESS_MAP, texture_number++);
	pbrExampleMettallic->addTexture("../../../../Demo/Textures/Metallic/ao.png", AO_MAP, texture_number++);
	pbrExampleMettallic->addTexture("../../../../Demo/Textures/Metallic/normal.png", NORMAL_MAP, texture_number++);
	pbrExampleMettallic->setMaterialType(PBR_BASIC);

	GLMaterial * skin = new GLMaterial();
	skin->color = glm::vec3(1.00, 0.71, 0.29);
	skin->setMaterialType(SSS);

	GLMaterial * helmMaterial = new GLMaterial();
	helmMaterial->addTexture("../../../../Demo/Textures/Helm/diffuse.png", DIFFUSE, texture_number++);
	helmMaterial->addTexture("../../../../Demo/Textures/Helm/metallic.png", METALLIC_MASK, texture_number++);
	helmMaterial->addTexture("../../../../Demo/Textures/Helm/roughness.png", ROUGHNESS_MAP, texture_number++);
	helmMaterial->addTexture("../../../../Demo/Textures/Helm/ao.png", AO_MAP, texture_number++);
	helmMaterial->addTexture("../../../../Demo/Textures/Helm/normal.png", NORMAL_MAP, texture_number++);
	helmMaterial->setMaterialType(PBR_BASIC);

	GLMaterial * shieldMaterial = new GLMaterial();
	shieldMaterial->addTexture("../../../../Demo/Textures/Shield/diffuse.png", DIFFUSE, texture_number++);
	shieldMaterial->addTexture("../../../../Demo/Textures/Shield/metallic.png", METALLIC_MASK, texture_number++);
	shieldMaterial->addTexture("../../../../Demo/Textures/Shield/roughness.png", ROUGHNESS_MAP, texture_number++);
	shieldMaterial->addTexture("../../../../Demo/Textures/Shield/ao.png", AO_MAP, texture_number++);
	shieldMaterial->addTexture("../../../../Demo/Textures/Shield/normal.png", NORMAL_MAP, texture_number++);
	shieldMaterial->setMaterialType(PBR_BASIC);

	GLMaterial * axeMaterial = new GLMaterial();
	axeMaterial->addTexture("../../../../Demo/Textures/Axe/diffuse.png", DIFFUSE, texture_number++);
	axeMaterial->addTexture("../../../../Demo/Textures/Axe/metallic.png", METALLIC_MASK, texture_number++);
	axeMaterial->addTexture("../../../../Demo/Textures/Axe/roughness.png", ROUGHNESS_MAP, texture_number++);
	axeMaterial->addTexture("../../../../Demo/Textures/Axe/ao.png", AO_MAP, texture_number++);
	axeMaterial->addTexture("../../../../Demo/Textures/Axe/normal.png", NORMAL_MAP, texture_number++);
	axeMaterial->setMaterialType(PBR_BASIC);

	GLMaterial * bodyMaterial = new GLMaterial();
	bodyMaterial->addTexture("../../../../Demo/Textures/Body/diffuse.png", DIFFUSE, texture_number++);
	bodyMaterial->addTexture("../../../../Demo/Textures/Body/metallic.png", METALLIC_MASK, texture_number++);
	bodyMaterial->addTexture("../../../../Demo/Textures/Body/roughness.png", ROUGHNESS_MAP, texture_number++);
	bodyMaterial->addTexture("../../../../Demo/Textures/Body/ao.png", AO_MAP, texture_number++);
	bodyMaterial->addTexture("../../../../Demo/Textures/Body/normal.png", NORMAL_MAP, texture_number++);
	bodyMaterial->setMaterialType(PBR_BASIC);

	/*
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
	bottomMaterial->shader = bShader; */

	//TODO link material through game object NOT drawable
	//------------Drawables-------------------
	GLDrawable* planeMesh = new GLDrawable();
	planeMesh->ptype = DRAWABLE;
	planeMesh->renderFlag = true;
	planeMesh->dtype = MESH;
	planeMesh->usingEBO = true;
	planeMesh->coords = mCoordSpawner->planeCoordsOnly;
	planeMesh->indices = mCoordSpawner->planeIndeces;
	planeMesh->material = ground_material;
	planeMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	toGenerate.push_back(planeMesh);

	/*
	GLDrawable* roboMesh = new GLDrawable();
	roboMesh->ptype = DRAWABLE;
	roboMesh->renderFlag = true;
	roboMesh->dtype = MESH;
	std::vector<float> coords_robo = std::vector<float>();
	std::vector<unsigned int> indices_robo = std::vector<unsigned int>();
	loadICE("robo.ice", roboMesh->coords, roboMesh->indices);
	roboMesh->usingEBO = true;
	roboMesh->material = roboMaterial;
	roboMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	//toGenerate.push_back(roboMesh);*/

	GLDrawable* bodyMesh = new GLDrawable();
	bodyMesh->ptype = DRAWABLE;
	bodyMesh->renderFlag = true;
	bodyMesh->dtype = MESH;
	loadICE("../../../../Demo/Models/body.ice", bodyMesh->coords, bodyMesh->indices);
	bodyMesh->usingEBO = true;
	bodyMesh->material = bodyMaterial;
	toGenerate.push_back(bodyMesh);

	GLDrawable* axeMesh = new GLDrawable();
	axeMesh->ptype = DRAWABLE;
	axeMesh->renderFlag = true;
	axeMesh->dtype = MESH;
	loadICE("../../../../Demo/Models/axe.ice", axeMesh->coords, axeMesh->indices);
	axeMesh->usingEBO = true;
	axeMesh->material = axeMaterial;
	toGenerate.push_back(axeMesh);

	GLDrawable* helmMesh = new GLDrawable();
	helmMesh->ptype = DRAWABLE;
	helmMesh->renderFlag = true;
	helmMesh->dtype = MESH;
	loadICE("../../../../Demo/Models/helm.ice", helmMesh->coords, helmMesh->indices);
	helmMesh->usingEBO = true;
	helmMesh->material = helmMaterial;
	toGenerate.push_back(helmMesh);

	GLDrawable* shieldMesh = new GLDrawable();
	shieldMesh->ptype = DRAWABLE;
	shieldMesh->renderFlag = true;
	shieldMesh->dtype = MESH;
	loadICE("../../../../Demo/Models/shield.ice", shieldMesh->coords, shieldMesh->indices);
	shieldMesh->usingEBO = true;
	shieldMesh->material = shieldMaterial;
	toGenerate.push_back(shieldMesh);

	GLDrawable* dis0 = new GLDrawable();
	dis0->ptype = DRAWABLE;
	dis0->renderFlag = true;
	dis0->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis0->coords, dis0->indices);
	dis0->usingEBO = true;
	dis0->material = mat0;
	toGenerate.push_back(dis0);

	GLDrawable* dis1 = new GLDrawable();
	dis1->ptype = DRAWABLE;
	dis1->renderFlag = true;
	dis1->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis1->coords, dis1->indices);
	dis1->usingEBO = true;
	dis1->material = mat1;
	toGenerate.push_back(dis1);

	GLDrawable* dis2 = new GLDrawable();
	dis2->ptype = DRAWABLE;
	dis2->renderFlag = true;
	dis2->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis2->coords, dis2->indices);
	dis2->usingEBO = true;
	dis2->material = mat2;
	toGenerate.push_back(dis2);

	GLDrawable* dis4 = new GLDrawable();
	dis4->ptype = DRAWABLE;
	dis4->renderFlag = true;
	dis4->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis4->coords, dis4->indices);
	dis4->usingEBO = true;
	dis4->material = pbrExampleMettallic;
	toGenerate.push_back(dis4);

	GLDrawable* dis5 = new GLDrawable();
	dis5->ptype = DRAWABLE;
	dis5->renderFlag = true;
	dis5->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis5->coords, dis5->indices);
	dis5->usingEBO = true;
	dis5->material = pbrExampleDialectric;
	toGenerate.push_back(dis5);

	GLDrawable* dis6 = new GLDrawable();
	dis6->ptype = DRAWABLE;
	dis6->renderFlag = true;
	dis6->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis6->coords, dis6->indices);
	dis6->usingEBO = true;
	dis6->material = mat4;
	toGenerate.push_back(dis6);

	GLDrawable* dis7 = new GLDrawable();
	dis7->ptype = DRAWABLE;
	dis7->renderFlag = true;
	dis7->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis7->coords, dis7->indices);
	dis7->usingEBO = true;
	dis7->material = skin;
	toGenerate.push_back(dis7);

	GLDrawable* dis8 = new GLDrawable();
	dis8->ptype = DRAWABLE;
	dis8->renderFlag = true;
	dis8->dtype = MESH;
	loadICE("../../../../Demo/Models/Icec.ice", dis8->coords, dis8->indices);
	dis8->usingEBO = true;
	dis8->material = iceMaterial;
	toGenerate.push_back(dis8);

	drawables.push_back(dis0);
	drawables.push_back(dis1);
	drawables.push_back(dis2);
	drawables.push_back(dis4);
	drawables.push_back(dis5);
	drawables.push_back(dis6);
	drawables.push_back(dis7);
	drawables.push_back(dis8);
	/*
	GLDrawable* squareMesh = new GLDrawable();
	squareMesh->ptype = DRAWABLE;
	squareMesh->renderFlag = true;
	squareMesh->dtype = MESH;
	squareMesh->usingEBO = true;
	squareMesh->coords = mCoordSpawner->squareCoordsOnly;
	squareMesh->indices = mCoordSpawner->squareIndices;
	squareMesh->bufferAttributes = glm::vec4(0, 3, 2, 2);
	squareMesh->material = gfPBR;
	//toGenerate.push_back(squareMesh);

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
	//toGenerate.push_back(suzzane_drawable);*/

	//-----------Objects------------
	/*
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
	*/
	/*
	ParticleSystem * fireSystem = new ParticleSystem(&timer);
	fireSystem->pos = glm::vec3(2, 0.1, -25);
	fireSystem->moved = true;
	fireSystem->setType(FIRE, texture_number++); */

	/*
	HeightMap * myMap = new HeightMap;
	GameObject* myTerrain = mObjectFactory->createTerrainSaveMap(100, 40, 30, "Textures/heightmap_hq.png", myMap);
	myTerrain->glDrawable->material = mat2;
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
	*/

	GameObject* body = new GameObject();
	body->name = "Body";
	body->glDrawable = bodyMesh;
	body->drawFlag = true;
	objects.push_back(body);

	GameObject* axe = new GameObject();
	axe->name = "Axe";
	axe->glDrawable = axeMesh;
	axe->drawFlag = true;
	objects.push_back(axe);

	GameObject* helm = new GameObject();
	helm->name = "Helm";
	helm->glDrawable = helmMesh;
	helm->drawFlag = true;
	objects.push_back(helm);

	GameObject* shield = new GameObject();
	shield->name = "shield";
	shield->glDrawable = shieldMesh;
	shield->drawFlag = true;
	objects.push_back(shield);

	GameObject* obj0 = new GameObject();
	obj0->name = "Material_Display_0";
	obj0->glDrawable = dis0;
	obj0->drawFlag = true;
	obj0->pos.x = rand_float_11() * 4.0f;
	obj0->pos.z = rand_float_11() * 4.0f;
	obj0->pos.y = 0.4;
	obj0->scale = glm::vec3(0.4, 0.4, 0.4);

	GameObject* obj1 = new GameObject();
	obj1->name = "Material_Display_1";
	obj1->glDrawable = dis1;
	obj1->drawFlag = true;
	obj1->pos.x = rand_float_11() * 4.0f;
	obj1->pos.z = rand_float_11() * 4.0f;
	obj1->pos.y = 0.4;
	obj1->scale = glm::vec3(0.4, 0.4, 0.4);

	GameObject* obj2 = new GameObject();
	obj2->name = "Material_Display_2";
	obj2->glDrawable = dis2;
	obj2->drawFlag = true;
	obj2->pos.x = rand_float_11() * 4.0f;
	obj2->pos.z = rand_float_11() * 4.0f;
	obj2->pos.y = 0.4;
	obj2->scale = glm::vec3(0.4, 0.4, 0.4);

	GameObject* obj4 = new GameObject();
	obj4->name = "Material_Display_4";
	obj4->glDrawable = dis4;
	obj4->drawFlag = true;
	obj4->pos.x = rand_float_11() * 4.0f;
	obj4->pos.z = rand_float_11() * 4.0f;
	obj4->pos.y = 0.4;
	obj4->scale = glm::vec3(0.4, 0.4, 0.4);

	GameObject* obj5 = new GameObject();
	obj5->name = "Material_Display_4";
	obj5->glDrawable = dis5;
	obj5->drawFlag = true;
	obj5->pos.x = -11.0f;
	obj5->pos.z = -21.0f;
	obj5->pos.y = 0.4;
	obj5->scale = glm::vec3(1, 1, 1);
	/*
	RigidBody *obj5Rigid = new RigidBody();
	obj5Rigid->mass = 1;
	obj5Rigid->is_active = true;
	obj5Rigid->setStart(obj5->pos, glm::vec3(5.0f, 7.5f, 0.0f));
	obj5->usingRigid = true;
	obj5->rigidBody = obj5Rigid;
	SphereCollider *obj5Coll = new SphereCollider();
	obj5Coll->position = obj5->pos;
	obj5Coll->radius = 0.45;
	obj5->sCollider = obj5Coll;
	obj5->usingCollider = true;*/

	GameObject* obj6 = new GameObject();
	obj6->name = "Material_Display_6";
	obj6->glDrawable = dis6;
	obj6->drawFlag = true;
	obj6->pos.x = rand_float_11() * 4.0f;
	obj6->pos.z = rand_float_11() * 4.0f;
	obj6->pos.y = 0.4;
	obj6->scale = glm::vec3(0.4, 0.4, 0.4);

	GameObject* obj7 = new GameObject();
	obj7->name = "Material_Display_7";
	obj7->glDrawable = dis7;
	obj7->drawFlag = true;
	obj7->pos.x = 0;
	obj7->pos.z = 0;
	obj7->pos.y = 1.0;
	obj7->scale = glm::vec3(1.0, 1.0, 1.0);

	GameObject* obj8 = new GameObject();
	obj8->name = "Material_Display_8";
	obj8->glDrawable = dis8;
	obj8->drawFlag = true;
	obj8->pos.x = -1.0f;
	obj8->pos.z = -21.0f;
	obj8->pos.y = 0.4;
	obj8->scale = glm::vec3(1, 1, 1);
	/*
	RigidBody *obj8Rigid = new RigidBody();
	obj8Rigid->mass = 1;
	obj8Rigid->is_active = true;
	obj8Rigid->setStart(obj8->pos, glm::vec3(-5.0f, 7.5f, 0.0f));
	obj8->usingRigid = true;
	obj8->rigidBody = obj8Rigid;
	SphereCollider *obj8Coll = new SphereCollider();
	obj8Coll->position = obj8->pos;
	obj8Coll->radius = 0.45;
	obj8->sCollider = obj8Coll;
	obj8->usingCollider = true;*/

	GameObject* ground = new GameObject();
	ground->name = "Ground";
	ground->drawFlag = true;
	ground->glDrawable = planeMesh;
	ground->properties.push_back(planeMesh);
	ground->pos.z = -12.0f;
	ground->pos.y = 0;
	ground->scale = glm::vec3(200, 200, 200);

	/*
	GameObject* spawnedSphere = mObjectFactory->createObject(SPHERE_PRIMITVE);
	spawnedSphere->glDrawable->material = gfPBR;
	//toGenerate.push_back(spawnedSphere->glDrawable);
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
	//toGenerate.push_back(spawnedSphere2->glDrawable);
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
	*/

	/*
	for (int i = 0; i < 100; i++) {
		GameObject* nBall = new GameObject();
		//nBall->glDrawable = drawables[rand() % 7 + 1];
		nBall->glDrawable = drawables[rand()%8];
		nBall->drawFlag = true;
		nBall->pos = glm::vec3(rand_float_11()*80.0f, 2.0f, rand_float_11()*80.0f);
		nBall->rot.y = rand_float_11() * 5;
		nBall->moved = true;
		nBall->name = "nBALL";
		nBall->scale = glm::vec3(2.0f, 2.0f, 2.0f);
		objects.push_back(nBall);
	}

	
	for (int i = 0; i < 18; i++) {
	ParticleSystem * fireSystem = new ParticleSystem(&timer);
	fireSystem->pos = glm::vec3(rand_float_11()*50, 0.1, rand_float_11()*50);
	fireSystem->moved = true;
	fireSystem->setType(RANDOM, texture_number++);
	pSystems.push_back(fireSystem);
	} */

	GameObject* spawnedLight_0 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(-2, 4, -2), glm::vec3(1.0f, 0.0f, 0.0f), 1.0, true);
	//toGenerate.push_back(spawnedLight_0->glDrawable);

	GameObject* spawnedLight_1 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(-2, 4, 2), glm::vec3(0.0f, 1.0f, 0.0f), 1.0, true);
	//toGenerate.push_back(spawnedLight_1->glDrawable);

	GameObject* spawnedLight_2 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(2, 2, 2), glm::vec3(0.0f, 0.0f, 1.0f), 1.0, true);
	//toGenerate.push_back(spawnedLight_2->glDrawable);

	GameObject* spawnedLight_3 = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(3, 3, -3), glm::vec3(1.0f, 0.0f, 1.0f), 1.0, true);
	//toGenerate.push_back(spawnedLight_3->glDrawable);

	//GameObject* player1_bottom = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(0, -0.9, 0), glm::vec3(1.0f, 1.0f, 0.0f), 1.0, true);
	//robo1->addChild(player1_bottom);
	//toGenerate.push_back(player1_bottom->glDrawable);

	//GameObject* player1_back = mObjectFactory->createLight(POINT_LIGHT, glm::vec3(0, -0.25, -0.85), glm::vec3(1.0f, 0.0f, 0.0f), 1.0, true);
	//robo1->addChild(player1_back);
	//toGenerate.push_back(player1_back->glDrawable);

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
	//objects.push_back(robo1);
	//objects.push_back(obj0);
	//objects.push_back(obj1);
	//objects.push_back(obj2);
	//objects.push_back(obj4);
	//objects.push_back(obj5);
	//objects.push_back(obj6);
	//objects.push_back(obj7);
	//objects.push_back(obj8);

	//pSystems.push_back(mySystem);
	//pSystems.push_back(fireSystem);

	lights.push_back(spawnedLight_0);
	lights.push_back(spawnedLight_1);
	lights.push_back(spawnedLight_2);
	lights.push_back(spawnedLight_3);
	//lights.push_back(player1_bottom);
	//lights.push_back(player1_back);

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
	std::cout << "Engine Starting" << std::endl;
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
		glClearColor(0.1, 0.1, 0.1, 1.0f);
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