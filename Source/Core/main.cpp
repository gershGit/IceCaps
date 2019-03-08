#include "Core/Lifecycle_Functions.h"
#include "Core/ManagersFactories.h"
#include "Core/SceneLoader.h"
#include "Core/Global_Callbacks.h"
#include <thread>

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

	//Loads a scene into managers and systems
	SceneLoader::loadScene(-1, config, managers, systems);

	//Run the game
	mainLoop(config, systems);

	//Stop all extra threads
	for (unsigned int i = 0; i < config.cpu_info->coreCount; i++) {
		if (config.cpu_info->threads[i].joinable()) {
			config.cpu_info->threads[i].join();
		}
	}

	//Unload the current scene
	SceneLoader::unloadScene(config, managers);

	//Clean up resources
	cleanup(config);

	//Exit gracefully
	return 0;
}