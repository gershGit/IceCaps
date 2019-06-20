/*
	Functions that translates strings to various types and vice versa
	Includes vulkan retrieval and check functions
*/

#pragma once
#include "EntityComponents.h"
#include "Vulkan/V_Components.h"

//Parses the key from a line
config_key getConfigKey(char * line, int buffer_size);

//Parses the value from a configuration line
std::string getConfigValue(char * line);

//Converts a string to an api identifier
API_used getAPI(char * value);

//Converts a string to a window mode
window_mode getWindowMode(char * value);

//Converts a string to a glfw monitor handle
GLFWmonitor* getMonitor(char * value);

//Translates a string to a vulkan format
VkFormat getFormat(char * value);

//Translates a string to a vulkan color space
VkColorSpaceKHR getColorSpace(char * value);

//Returns a vulkan present mode from a string value
VkPresentModeKHR getPresentMode(char * value);

//Returns a bool from a string
bool getBool(char * value);

//Converts a string to a material type
material_type stringToMaterialType(char * value);

//Returns only the value portion of a string from a KEY=VALUE line
int getValue(char * buffer, char * value, int bufferSize);

//Converts a line to a scene key
scene_key getSceneKey(char * buffer, int bufferSize);

//Gets a component type from a line value
component_type getComponentType(char * value);

//Gets a system type from a line value
system_type getSystemType(char * value);

//Strips leading tabs and spaces and returns a string
void getSubComponent(char * buffer, char * keyString, int buffer_size);

//Converts a string to a texture type
texture_type getTextureType(char * typeAsString);

//Converts a string to a collider type
collider_type getColliderType(char * typeAsString);

//Translates a string to a light type
light_type getLightTypeFromString(char * typeAsString);

//Parses an int from a word
int getChildCount(char * value);

//Generates a vector from a string of floats separated by commas
template<class Vector>
inline Vector getVectorFromString(char * value, int valueSize)
{
	char floatString[64];
	int fPos = 0;
	int vectorIndex = 0;
	Vector returnVector = Vector();
	for (int i = 0; i < valueSize; i++) {
		if (value[i] == ',') {
			floatString[fPos] = '\0';
			returnVector[vectorIndex] = strtof(floatString, nullptr);
			vectorIndex++;
			fPos = 0;
		}
		else {
			floatString[fPos] = value[i];
			fPos++;
		}
	}
	floatString[fPos] = '\0';
	returnVector[vectorIndex] = strtof(floatString, nullptr);
	return returnVector;
}

/*
	Configuration Structure Functions
*/

//Converts a string to a window mode
void getRefreshRate(char * value, configurationStructure &config);

//Converts a string to a window mode
void getResolution(char * value, configurationStructure &config, int valSize);

//Sets the version numbers from the string value
void getAppVersion(char * value, configurationStructure &config, int valSize);

//Adds a feature to the config
void addGpuFeature(char * value, configurationStructure &config);

//Sets the format in the configuration file
void setFormat(char * value, configurationStructure &config);

//Sets the color space in the configuration file from a string
void setColorSpace(char * value, configurationStructure &config);

//Sets the present mode of a configuration structure
void setPresentMode(char * value, configurationStructure &config);

//Sets the depth format of a configuration structure
void setDepthFormat(char * value, configurationStructure &config);

//Returns the sum of base^depth down from depth to 0
int powerFact(int base, int depth);

//Sets the count of scene_nodes making up an entire scene
void setNodeCount(configurationStructure &config, int base, int depth);

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

//Tag operations

template<typename TagType>
inline bool hasTag(TagType tags_active, TagType tags_wanted) {
	return tags_active & tags_wanted;
}

template<typename TagType>
inline TagType addTag(TagType current_tags, TagType new_tag)
{
	return current_tags | new_tag;
}

template<typename TagType>
inline TagType removeTag(TagType current_tags, TagType removed_tag) {
	return current_tags | (current_tags & (~removed_tag));
}
