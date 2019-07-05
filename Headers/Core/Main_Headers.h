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

#include <mutex>

class GLInstance;
class V_Instance;

//Enumerations used throughout the program
enum platform_code {Windows, Linux, Mac};
enum API_used { openGL, Vulkan, DX_11, DX_12 };
enum window_mode { FULL, BORDERLESS, WINDOWED, NO_RESIZE };
enum config_key { NO_KEY_VALUE, PREFERRED_API, DEFAULT_API, WINDOW_MODE, MONITOR, REFRESH_RATE, RESOLUTION,
					APP_NAME, APP_VERSION,
					MULTI_GPU, GPU_SELECTED, COMPUTE_REQUIRED, GPU_FEATURE, ANTI_ALIASING, ANTI_ALIASING_RES, ANISOTROPY_RES,
					SURFACE_FORMAT, SURFACE_COLORSPACE, PRESENT_MODE, SWAPCHAIN_BUFFERING, DEPTH_FORMAT,
					GAME_PATH};
enum dimension { WIDTH, HEIGHT };
enum anti_aliasing_type {SUPER_SAMPLING, MULTI_SAMPLING, NO_ANTI_ALIASING};
enum scene_key { SCENE_NAME, MANAGERS, SYSTEMS, ADD_MANAGER, ADD_SYSTEM, ADD_PIPELINE, ENTITY_COUNT, ENTITY, ADD_COMPONENT, ADD_TAG, IS_DYNAMIC, NO_SCENE_KEY,
	SCENE_SIZE, TREE_DEPTH, SCENE_STRUCTURE, END_STATEMENT, END_ENTITIES, FILE_LOAD_KEY };
enum materialSubComponents { MAT_TYPE, TEXTURE, END };
enum material_type { PBR, PBR_CHAR };
enum texture_type { DIFFUSE, METALLIC_SPEC, NORMAL, ROUGHNESS, AO, NO_TEXTURE_TYPE = -1 };
enum collider_type { SPHERE_COLLIDER, AABB_COLLIDER, BOX_COLLIDER, CAPSULE_COLLIDER, MESH_COLLIDER, NO_COLLIDER_TYPE = -1 };
enum light_type { POINT_LIGHT, DIRECTION_LIGHT, SPOT_LIGHT };
enum key_code {
	NO_KEY, UNIQUE_KEY_1, UNIQUE_KEY_2, UNIQUE_KEY_3, UNIQUE_KEY_4, UNIQUE_KEY_5,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_PAD_1, KEY_PAD_2, KEY_PAD_3, KEY_PAD_4, KEY_PAD_5, KEY_PAD_6, KEY_PAD_7, KEY_PAD_8, KEY_PAD_9, KEY_PAD_0,
	KEY_PAD_SLASH, KEY_PAD_MUL, KEY_PAD_MINUS, KEY_PAD_PLUS,
	KEY_LEFT_SHIFT, KEY_LEFT_CTL, KEY_LEFT_ALT, KEY_TAB, KEY_ENTER, KEY_BACKSPACE, KEY_DELETE, KEY_RIGHT_SHIFT, KEY_RIGHT_CTL, KEY_RIGHT_ALT, KEY_CAPS, KEY_SPACE,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
	KEY_FORWARD_SLASH, KEY_BACK_SLASH, KEY_LESS, KEY_GREATER, KEY_MINUS, KEY_PLUS, KEY_ESC, KEY_TILDA, KEY_COLON, KEY_QUOTE, KEY_BRACKET_OPEN, KEY_BRACKET_CLOSE,
	KEY_MUTE, KEY_VOL_MINUS, KEY_VOL_PLUS, KEY_REFRESH, KEY_WINDOWS
};
enum mouse_code {
	MOUSE_LEFT, MOUSE_RIGHT, MOUSE_SCROLL, MOUSE_SIDE_1, MOUSE_SIDE_2, MOUSE_SIDE_3, MOUSE_SIDE_4
};
enum key_state { UP, DOWN, PRESS, RELEASE };

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
	platform_code platform;
	int sceneChildren = 4;
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
	int sceneNodesCount;
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
	int bone_id[4];
	float bone_weight[4];
};

struct ViewPersp {
	glm::mat4 view;
	glm::mat4 persp;
	//glm::mat4 normal;
};

struct LightObject {
	glm::vec4 position = glm::vec4(0);
	glm::vec4 color = glm::vec4(0);
};

struct AABB {
	glm::vec3 size;
	glm::vec3 pos;
	glm::vec3 points[8];
};

struct SceneNode {
	int id;
	AABB bounds;

	bool isLeaf;
	SceneNode* children;
	int staticEntityCount;
	int* staticEntities;
	std::vector<int> * dynamicEntities;

	std::vector<int>* lightIDs;
	int lightCount;
	LightObject* lights;
};

struct frustum {
	glm::vec3 points[8];
	glm::vec3 axis[5];
};