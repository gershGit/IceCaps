/*
	Functions that translates strings to various types and vice versa
	Includes vulkan retrieval and check functions
*/

#pragma once
#include "EntityComponents.h"
#include "V_Components.h"

//Parses the key from a line
config_key getConfigKey(std::string line);

//Parses the value from a configuration line
std::string getConfigValue(std::string line);

//Converts a string to an api identifier
API_used getAPI(std::string value);

//Converts a string to a window mode
window_mode getWindowMode(std::string value);

//Converts a string to a glfw monitor handle
GLFWmonitor* getMonitor(std::string value);

//Translates a string to a vulkan format
VkFormat getFormat(std::string value);

//Translates a string to a vulkan color space
VkColorSpaceKHR getColorSpace(std::string value);

//Returns a vulkan present mode from a string value
VkPresentModeKHR getPresentMode(std::string value);

//Returns a bool from a string
bool getBool(std::string value);

//Converts a string to a material type
material_type stringToMaterialType(std::string value);

//Returns only the value portion of a string from a KEY=VALUE line
std::string getValue(std::string line);

//Converts a line to a scene key
scene_key getSceneKey(std::string line);

//Gets a component type from a line value
component_type getComponentType(std::string value);

//Gets a system type from a line value
system_type getSystemType(std::string value);

//Strips leading tabs and spaces and returns a string
std::string getSubComponent(std::string line);

//Converts a string to a texture type
texture_type getTextureType(std::string typeAsString);

/*
	Configuration Structure Functions
*/

//Converts a string to a window mode
void getRefreshRate(std::string value, configurationStructure &config);

//Converts a string to a window mode
void getResolution(std::string value, configurationStructure &config);

//Sets the version numbers from the string value
void getAppVersion(std::string value, configurationStructure &config);

//Adds a feature to the config
void addGpuFeature(std::string value, configurationStructure &config);

//Sets the format in the configuration file
void setFormat(std::string value, configurationStructure &config);

//Sets the color space in the configuration file from a string
void setColorSpace(std::string value, configurationStructure &config);

//Sets the present mode of a configuration structure
void setPresentMode(std::string value, configurationStructure &config);

//Sets the depth format of a configuration structure
void setDepthFormat(std::string value, configurationStructure &config);


/*
	Vulkan to strings
*/

//Feature list to strings
void displayFeatures(VkPhysicalDeviceFeatures features);

//Vulkan Format to String
std::string formatToString(VkSurfaceFormatKHR format);

//Present Mode To String
std::string presentModeToString(VkPresentModeKHR mode);

//Returns whether or not the format conatins a stencil component
bool hasStencilComponent(VkFormat format);