/*
	Headers needed throughout the application
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>

class GLInstance;
class V_Instance;

//Enumerations used throughout the program
enum API_used { openGL, Vulkan, DX_11, DX_12 };
enum window_mode { FULL, BORDERLESS, WINDOWED, NO_RESIZE };
enum config_key { NO_KEY_VALUE, PREFERRED_API, DEFAULT_API, WINDOW_MODE, MONITOR, REFRESH_RATE, RESOLUTION,
					APP_NAME, APP_VERSION,
					MULTI_GPU, GPU_SELECTED, COMPUTE_REQUIRED, GPU_FEATURE, ANTI_ALIASING, ANTI_ALIASING_RES, ANISOTROPY_RES,
					SURFACE_FORMAT, SURFACE_COLORSPACE, PRESENT_MODE, SWAPCHAIN_BUFFERING, DEPTH_FORMAT,
					GAME_PATH};
enum dimension { WIDTH, HEIGHT };
enum anti_aliasing_type {SUPER_SAMPLING, MULTI_SAMPLING, NO_ANTI_ALIASING};
enum scene_key { SCENE_NAME, MANAGERS, SYSTEMS, ADD_MANAGER, ADD_SYSTEM, ADD_PIPELINE, ENTITY_COUNT, ENTITY, ADD_COMPONENT, NO_SCENE_KEY, END_STATEMENT, END_ENTITIES };
enum materialSubComponents { MAT_TYPE, TEXTURE, END };
enum material_type { PBR };
enum texture_type { DIFFUSE, METALLIC_SPEC, NORMAL, ROUGHNESS, AO, NO_TEXTURE_TYPE = -1 };

//API information, specifically useful for instance retrieval
struct initAPIinfo {
	bool vulkan = false;
	bool openGL = false;
	bool dx_11 = false;
	bool dx_12 = false;
	GLInstance* gl_instance;
	V_Instance* v_Instance;
};

//Information on the cpu and threads to be used
struct cpuInfo {
	unsigned int coreCount;
	std::vector<std::thread> threads;
};

//Information on the gpu
struct gpuInfo {

};

//Structure to save current information
struct save_info {
	std::string levelName;
	int levelIndex;
};

//Structure to hold the configuration of the app, reloaded on each startup and saved on close
struct configurationStructure {
	bool changed = false;
	bool debugOn;
	API_used api;
	API_used backup_api;
	initAPIinfo apiInfo;
	window_mode window_mode_selected;
	GLFWmonitor* monitor;
	GLFWvidmode* videoSettings = new GLFWvidmode();
	GLFWwindow* window;
	cpuInfo* cpu_info;
	gpuInfo gpu_info;
	std::string appName;
	int appVersion[3];
	char* engineName;
	int engineVersion[3];
	bool multiGPU;
	int gpuSelected;
	bool computeRequired;
	VkPhysicalDeviceFeatures featuresRequired = VkPhysicalDeviceFeatures();
	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	int swapchainBuffering = 1;
	VkFormat colorFormat;
	VkFormat depthFormat;
	std::string gamePath;
	save_info saveInfo;
	anti_aliasing_type antiAliasing;
	int anti_aliasing_resolution;
	int anisotropy;
};

//SubComponents
struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 uv;
};

//UI structure with infromation on how to render
struct ui_rectangle {
	glm::vec2 topLeft;
	glm::vec2 bottomRight;
};

//Special vertex structure containing bone information
struct skinned_vertex {
	vertex mVertex;
	std::vector<unsigned int> bones;
	std::vector<float> weights;
};

struct ViewPersp {
	glm::mat4 view;
	glm::mat4 persp;
	//glm::mat4 normal;
};

struct LightObject {
	glm::vec4 position;
	glm::vec4 color;
};