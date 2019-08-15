#include "Core/Lifecycle_Functions.h"
#include "Core/ManagersFactories.h"
#include "Core/SceneLoader.h"
#include "Core/Global_Callbacks.h"
#include "Core/ThreadPool.h"

using namespace std;

int main() {
	//Initialize glfw and prepare the configuration structure
	glfwInit();
	configurationStructure config;

	//Load configuration information
	loadConfiguration(config);

	//Create a window
	config.window = initializeWindow(config);

	//Connect our API to our window
	initializeAPI(config);

	//List of all needed component managers
	std::vector<ComponentManager*> managers = std::vector<ComponentManager*>();
	std::vector<EntitySystem*> systems = std::vector<EntitySystem*>();
	SceneNode* scene = new SceneNode(); //Freed during unloading of scene

	//Loads a scene into managers and systems
	SceneLoader::loadScene(-1, config, managers, systems, scene);

	//Run the game
	mainLoop(config, systems, scene);

	//Stop all extra threads
	ThreadPool::cleanup();

	//Unload the current scene
	SceneLoader::unloadScene(config, managers, systems);

	//Clean up resources
	cleanup(config);

	//Free memory allocated for config
	delete config.cpu_info;

	//Exit gracefully
	return 0;
}