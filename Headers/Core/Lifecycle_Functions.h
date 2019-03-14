/*
	Functions that run throughout the lifecycle of the application
*/

#pragma once

#include "Core/Main_Headers.h"
#include "OpenGL/OpenGLInstance.h"
#include "Vulkan/V_Instance.h"
#include "Vulkan/V_Device.h"
#include "Core/StringTranslation.h"
#include "Core/SystemsHeader.h"
#include "GameTimer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

//Called when glfw recognizes a frame buffer resize
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto instance = reinterpret_cast<V_Instance*>(glfwGetWindowUserPointer(window));
	instance->framebufferResized = true;
}

//Called when glw rexognizes a key press, or mouse event
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputSystem::setKey(key, scancode, action, mods);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	InputSystem::setCursor(xpos, ypos);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	InputSystem::setMouseButton(button, action, mods);
}
//-----------------------------------------------------------------------------------------

//Loads settings for the engine and game
void loadConfiguration(configurationStructure &config) {
#ifdef _WIN32
	config.platform = Windows;
#else
	config.platform = Linux;
#endif // WIN32


	config.cpu_info = new cpuInfo();
	config.cpu_info->coreCount = std::thread::hardware_concurrency();
	if (config.cpu_info->coreCount == 0) {
		config.cpu_info->coreCount = 1;
	}
	config.cpu_info->threads.resize(config.cpu_info->coreCount);

	std::vector<config_key> keys = std::vector<config_key>();
	std::vector<std::string> values = std::vector<std::string>();

	std::ifstream infile("../../Configuration/icecaps.config");
	std::string line, value;
	config_key key;
	while (std::getline(infile, line))
	{
		key = getConfigKey(line);
		value = getValue(line);
		if (key == PREFERRED_API) {
			config.api = getAPI(value);
		}
		else if (key == DEFAULT_API) {
			config.backup_api = getAPI(value);
		}
		else if (key == WINDOW_MODE) {
			config.window_mode_selected = getWindowMode(value);
		}
		else if (key == MONITOR) {
			config.monitor = getMonitor(value);
		}
		else if (key == REFRESH_RATE) {
			getRefreshRate(value, config);
		}
		else if (key == RESOLUTION) {
			getResolution(value, config);
		}
		else if (key == APP_NAME) {
			config.appName = value;
		}
		else if (key == APP_VERSION) {
			getAppVersion(value, config);
		}
		else if (key == MULTI_GPU) {
			config.multiGPU = getBool(value);
		}
		else if (key == GPU_SELECTED) {
			config.gpuSelected = atoi(value.c_str());
		}
		else if (key == COMPUTE_REQUIRED) {
			config.computeRequired = getBool(value);
		}
		else if (key == GPU_FEATURE) {
			addGpuFeature(value, config);
		}
		else if (key == SURFACE_FORMAT) {
			setFormat(value, config);
		}
		else if (key == SURFACE_COLORSPACE) {
			setColorSpace(value, config);
		}
		else if (key == PRESENT_MODE) {
			setPresentMode(value, config);
		}
		else if (key == SWAPCHAIN_BUFFERING) {
			config.swapchainBuffering = atoi(value.c_str());
			if (config.swapchainBuffering < 1) {
				config.swapchainBuffering = 1;
			}
		}
		else if (key == DEPTH_FORMAT) {
			setDepthFormat(value, config);
		}
		else if (key == GAME_PATH) {
			config.gamePath = value;
		}
		else if (key == ANTI_ALIASING) {
			if (strcmp(value.c_str(), "MULTI_SAMPLING")) {
				config.antiAliasing = MULTI_SAMPLING;
			}
			else if (strcmp(value.c_str(), "SUPER_SAMPLING")) {
				config.antiAliasing = SUPER_SAMPLING;
			}
			else {
				config.antiAliasing = NO_ANTI_ALIASING;
			}
		}
		else if (key == ANTI_ALIASING_RES) {
			config.anti_aliasing_resolution = atoi(value.c_str());
		} 
		else if (key == ANISOTROPY_RES) {
			config.anisotropy = atoi(value.c_str());
		}
	}
}
//-----------------------------------------------------------------------------------------

//Creates a window set up for vulkan
GLFWwindow* vulkan_initializeWindow(configurationStructure &config) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);		//Vulkan specific to not create opengl context
	GLFWwindow* window;

	//Set window mode from preferences
	if (config.window_mode_selected == FULL) {
		window = glfwCreateWindow(config.videoSettings->width, config.videoSettings->height, "Icecaps", config.monitor, NULL);
	} else if (config.window_mode_selected == BORDERLESS) {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(config.videoSettings->width, config.videoSettings->height, "Vulkan Window", nullptr, nullptr);
	}
	else if (config.window_mode_selected == WINDOWED) {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(config.videoSettings->width, config.videoSettings->height, "Vulkan Window", nullptr, nullptr);
	}
	else if (config.window_mode_selected == NO_RESIZE) {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(config.videoSettings->width, config.videoSettings->height, "Vulkan Window", nullptr, nullptr);
	}
	else {
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(config.videoSettings->width, config.videoSettings->height, "Vulkan Window", nullptr, nullptr);
	}

	//Create the glfw window and get handle
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window for vulkan" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	return window;
}

//Creates a window set up for OpenGL
GLFWwindow* opengl_initializeWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Opengl Window", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window for opengl" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	return window;
}

//The main engine loop for an application using vulkan
void vulkan_mainloop(configurationStructure &config, std::vector<EntitySystem*> &systems) {
	GameTimer::Start();
	for (EntitySystem * system : systems) {
		system->start();
	}
	while (!glfwWindowShouldClose(config.window)) {	
		glfwPollEvents();
		GameTimer::Update();
		if (config.changed) {
			for (EntitySystem * system : systems) {
				system->onConfigurationChange();
			}
			config.changed = false;
		}
		for (EntitySystem * system : systems) {
			system->onUpdate();
		}
	}

	vkDeviceWaitIdle(config.apiInfo.v_Instance->getPrimaryDevice()->getLogicalDevice());
}

//The main engine loop for an application using OpenGL
void opengl_mainloop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

//Initializes everything needed for an OpenGL application
GLInstance* opengl_initialize() {
	GLInstance* instance = new GLInstance();
	instance->initialize();
	instance->initGlad();
	return instance;
}

//Initializes everything needed for a vulkan application
V_Instance* vulkan_initialize(configurationStructure &config) {
	V_Instance* instance = new V_Instance(&config);
	instance->enumerateExtensions();
	instance->initialize(config);
	instance->setupDebugCallback();
	instance->createSurface();
	glfwSetWindowUserPointer(config.window, instance);
	glfwSetFramebufferSizeCallback(config.window, framebufferResizeCallback);
	glfwSetKeyCallback(config.window, key_callback);
	glfwSetCursorPosCallback(config.window, cursor_position_callback);
	glfwSetInputMode(config.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(config.window, mouse_button_callback);
	instance->attachDevices(config.cpu_info->coreCount);
	instance->createTransferCommandPools();
	instance->createSwapChain();
	instance->createSampler();
	instance->createCommandPools(config.cpu_info->coreCount);
	instance->createSyncObjects();
	return instance;
}

//Returns the information associated with the selected graphics API
initAPIinfo initializeAPI(configurationStructure &config) {
	//load file with selection
	if (config.api == Vulkan) {
		std::cout << "Running with Vulkan" << std::endl;
		V_Instance* instance = vulkan_initialize(config);

		initAPIinfo info;
		info.vulkan = true;
		info.v_Instance = instance;

		config.apiInfo = info;
		return info;
	}
	else if (config.api == openGL) {
		std::cout << "Running with OpenGL" << std::endl;
		GLInstance* instance = opengl_initialize();

		initAPIinfo info;
		info.openGL = true;
		info.gl_instance = instance;

		config.apiInfo = info;
		return info;
	}
	return initAPIinfo();
}

//Cleans up everything related to vulkan
void vulkan_cleanup(configurationStructure &config) {
	config.apiInfo.v_Instance->cleanup();
	
	glfwDestroyWindow(config.window);

	glfwTerminate();

	char c;
	scanf_s("%c", &c, 1);
}

//Cleans up everything related to OpenGL
void opengl_cleanup(GLFWwindow* window){
	glfwDestroyWindow(window);

	glfwTerminate();
}


//Determines which subfunction to call based on configurations
GLFWwindow* initializeWindow(configurationStructure &config) {
	if (config.api == Vulkan) {
		return vulkan_initializeWindow(config);
	}
	else if (config.api == openGL) {
		return opengl_initializeWindow();
	}
	return nullptr;
}
void mainLoop(configurationStructure &config, std::vector<EntitySystem*> &systems) {
	if (config.api == Vulkan) {
		vulkan_mainloop(config, systems);
	}
	else if (config.api == openGL) {
		opengl_mainloop(config.window);
	}
}
void cleanup(configurationStructure &config) {
	if (config.apiInfo.vulkan) {
		//All vulkan cleanup
		vulkan_cleanup(config);
	}
	else {
		//All OpenGL cleanup
		opengl_cleanup(config.window);
	}
}