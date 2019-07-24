#pragma once
#include "Core/StringTranslation.h"
#include "Core/EntityComponents.h"
#include "Vulkan/V_Components.h"

//Parses the key from a line
config_key getConfigKey(char * line, int buffer_size) {
	char ck[64];
	int pos = 0;
	for (int i = 0; i < buffer_size; i++) {
		if (line[i] == '=') {
			ck[pos] = '\0';
			break;
		}
		else {
			ck[pos] = line[i];
			pos++;
		}
	}

	if (strcmp(ck, "PREFERRED_API") == 0) {
		return PREFERRED_API;
	}
	else  if (strcmp(ck, "DEFAULT_API") == 0) {
		return DEFAULT_API;
	}
	else if (strcmp(ck, "WINDOW_MODE") == 0) {
		return WINDOW_MODE;
	}
	else if (strcmp(ck, "MONITOR") == 0) {
		return MONITOR;
	}
	else if (strcmp(ck, "REFRESH_RATE") == 0) {
		return REFRESH_RATE;
	}
	else if (strcmp(ck, "RESOLUTION") == 0) {
		return RESOLUTION;
	}
	else if (strcmp(ck, "APP_NAME") == 0) {
		return APP_NAME;
	}
	else if (strcmp(ck, "APP_VERSION") == 0) {
		return APP_VERSION;
	}
	else if (strcmp(ck, "MULTI_GPU") == 0) {
		return MULTI_GPU;
	}
	else if (strcmp(ck, "GPU") == 0) {
		return GPU_SELECTED;
	}
	else if (strcmp(ck, "COMPUTE_REQUIRED") == 0) {
		return COMPUTE_REQUIRED;
	}
	else if (strcmp(ck, "GPU_FEATURE") == 0) {
		return GPU_FEATURE;
	}
	else if (strcmp(ck, "SURFACE_FORMAT") == 0) {
		return SURFACE_FORMAT;
	}
	else if (strcmp(ck, "SURFACE_COLOR_SPACE") == 0) {
		return SURFACE_COLORSPACE;
	}
	else if (strcmp(ck, "PRESENT_MODE") == 0) {
		return PRESENT_MODE;
	}
	else if (strcmp(ck, "SWAPCHAIN_BUFFERING") == 0) {
		return SWAPCHAIN_BUFFERING;
	}
	else if (strcmp(ck, "DEPTH_FORMAT") == 0) {
		return DEPTH_FORMAT;
	}
	else if (strcmp(ck, "GAME_PATH") == 0) {
		return GAME_PATH;
	}
	else if (strcmp(ck, "ANTI_ALIASING") == 0) {
		return ANTI_ALIASING;
	}
	else if (strcmp(ck, "ANTI_ALIASING_RES") == 0) {
		return ANTI_ALIASING_RES;
	}
	else if (strcmp(ck, "ANISOTROPY_RES") == 0) {
		return ANISOTROPY_RES;
	}
	return NO_KEY_VALUE;
}

//Converts a string to an api identifier
API_used getAPI(char * value) {
	if (strcmp(value, "VULKAN") == 0) {
		return Vulkan;
	}
	else if (strcmp(value, "GL") == 0) {
		return openGL;
	}
	else if (strcmp(value, "DX11") == 0) {
		return DX_11;
	}
	else if (strcmp(value, "DX12") == 0) {
		return DX_12;
	}
	return openGL;
}

//Converts a string to a window mode
window_mode getWindowMode(char * value) {
	if (strcmp(value, "FULL") == 0) {
		return FULL;
	}
	else if (strcmp(value, "BORDERLESS") == 0) {
		return BORDERLESS;
	}
	else if (strcmp(value, "WINDOWED") == 0) {
		return WINDOWED;
	}
	else if (strcmp(value, "NO_RESIZE") == 0) {
		return NO_RESIZE;
	}
	return WINDOWED;
}

//Converts a string to a glfw monitor handle
GLFWmonitor* getMonitor(char * value) {
	if (strcmp(value, "-1") == 0) {
		return glfwGetPrimaryMonitor();
	}
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	int index = atoi(value);
	if (index < count) {
		return monitors[index];
	}
	return glfwGetPrimaryMonitor();
}

//Translates a string to a vulkan format
VkFormat getFormat(char * value) {
	if (strcmp(value, "UNDEFINED") == 0) {
		return VK_FORMAT_UNDEFINED;
	}
	else if (strcmp(value, "R4G4_UNORM_PACK8") == 0) {
		return VK_FORMAT_R4G4_UNORM_PACK8;
	}
	else if (strcmp(value, "R4G4B4A4_UNORM_PACK16") == 0) {
		return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
	}
	else if (strcmp(value, "B4G4R4A4_UNORM_PACK16") == 0) {
		return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
	}
	else if (strcmp(value, "R5G6B5_UNORM_PACK16") == 0) {
		return VK_FORMAT_R5G6B5_UNORM_PACK16;
	}
	else if (strcmp(value, "B5G6R5_UNORM_PACK16") == 0) {
		return VK_FORMAT_B5G6R5_UNORM_PACK16;
	}
	else if (strcmp(value, "R5G5B5A1_UNORM_PACK16") == 0) {
		return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
	}
	else if (strcmp(value, "B5G5R5A1_UNORM_PACK16") == 0) {
		return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
	}
	else if (strcmp(value, "A1R5G5B5_UNORM_PACK16") == 0) {
		return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
	}
	else if (strcmp(value, "R8_UNORM") == 0) {
		return VK_FORMAT_R8_UNORM;
	}
	else if (strcmp(value, "R8_SNORM") == 0) {
		return VK_FORMAT_R8_SNORM;
	}
	else if (strcmp(value, "R8_USCALED") == 0) {
		return VK_FORMAT_R8_USCALED;
	}
	else if (strcmp(value, "R8_SSCALED") == 0) {
		return VK_FORMAT_R8_SSCALED;
	}
	else if (strcmp(value, "R8_UINT") == 0) {
		return VK_FORMAT_R8_UINT;
	}
	else if (strcmp(value, "R8_SINT") == 0) {
		return VK_FORMAT_R8_SINT;
	}
	else if (strcmp(value, "R8_SRGB") == 0) {
		return VK_FORMAT_R8_SRGB;
	}
	else if (strcmp(value, "R8G8_UNORM") == 0) {
		return VK_FORMAT_R8G8_UNORM;
	}
	else if (strcmp(value, "R8G8_SNORM") == 0) {
		return VK_FORMAT_R8G8_SNORM;
	}
	else if (strcmp(value, "R8G8_USCALED") == 0) {
		return VK_FORMAT_R8G8_USCALED;
	}
	else if (strcmp(value, "R8G8_SSCALED") == 0) {
		return VK_FORMAT_R8G8_SSCALED;
	}
	else if (strcmp(value, "R8G8_UINT") == 0) {
		return VK_FORMAT_R8G8_UINT;
	}
	else if (strcmp(value, "R8G8_SINT") == 0) {
		return VK_FORMAT_R8G8_SINT;
	}
	else if (strcmp(value, "R8G8_SRGB") == 0) {
		return VK_FORMAT_R8G8_SRGB;
	}
	else if (strcmp(value, "R8G8B8_UNORM") == 0) {
		return VK_FORMAT_R8G8B8_UNORM;
	}
	else if (strcmp(value, "R8G8B8_SNORM") == 0) {
		return VK_FORMAT_R8G8B8_SNORM;
	}
	else if (strcmp(value, "R8G8B8_USCALED") == 0) {
		return VK_FORMAT_R8G8B8_USCALED;
	}
	else if (strcmp(value, "R8G8B8_SSCALED") == 0) {
		return VK_FORMAT_R8G8B8_SSCALED;
	}
	else if (strcmp(value, "R8G8B8_UINT") == 0) {
		return VK_FORMAT_R8G8B8_UINT;
	}
	else if (strcmp(value, "R8G8B8_SINT") == 0) {
		return VK_FORMAT_R8G8B8_SINT;
	}
	else if (strcmp(value, "R8G8B8_SRGB") == 0) {
		return VK_FORMAT_R8G8B8_SRGB;
	}
	else if (strcmp(value, "B8G8R8_UNORM") == 0) {
		return VK_FORMAT_B8G8R8_UNORM;
	}
	else if (strcmp(value, "B8G8R8_SNORM") == 0) {
		return VK_FORMAT_B8G8R8_SNORM;
	}
	else if (strcmp(value, "B8G8R8_USCALED") == 0) {
		return VK_FORMAT_B8G8R8_USCALED;
	}
	else if (strcmp(value, "B8G8R8_SSCALED") == 0) {
		return VK_FORMAT_B8G8R8_SSCALED;
	}
	else if (strcmp(value, "B8G8R8_UINT") == 0) {
		return VK_FORMAT_B8G8R8_UINT;
	}
	else if (strcmp(value, "B8G8R8_SINT") == 0) {
		return VK_FORMAT_B8G8R8_SINT;
	}
	else if (strcmp(value, "B8G8R8_SRGB") == 0) {
		return VK_FORMAT_B8G8R8_SRGB;
	}
	else if (strcmp(value, "R8G8B8A8_UNORM") == 0) {
		return VK_FORMAT_R8G8B8A8_UNORM;
	}
	else if (strcmp(value, "R8G8B8A8_SNORM") == 0) {
		return VK_FORMAT_R8G8B8A8_SNORM;
	}
	else if (strcmp(value, "R8G8B8A8_USCALED") == 0) {
		return VK_FORMAT_R8G8B8A8_USCALED;
	}
	else if (strcmp(value, "R8G8B8A8_SSCALED") == 0) {
		return VK_FORMAT_R8G8B8A8_SSCALED;
	}
	else if (strcmp(value, "R8G8B8A8_UINT") == 0) {
		return VK_FORMAT_R8G8B8A8_UINT;
	}
	else if (strcmp(value, "R8G8B8A8_SINT") == 0) {
		return VK_FORMAT_R8G8B8A8_SINT;
	}
	else if (strcmp(value, "R8G8B8A8_SRGB") == 0) {
		return VK_FORMAT_R8G8B8A8_SRGB;
	}
	else if (strcmp(value, "B8G8R8A8_UNORM") == 0) {
		return VK_FORMAT_B8G8R8A8_UNORM;
	}
	else if (strcmp(value, "B8G8R8A8_SNORM") == 0) {
		return VK_FORMAT_B8G8R8A8_SNORM;
	}
	else if (strcmp(value, "B8G8R8A8_USCALED") == 0) {
		return VK_FORMAT_B8G8R8A8_USCALED;
	}
	else if (strcmp(value, "B8G8R8A8_SSCALED") == 0) {
		return VK_FORMAT_B8G8R8A8_SSCALED;
	}
	else if (strcmp(value, "B8G8R8A8_UINT") == 0) {
		return VK_FORMAT_B8G8R8A8_UINT;
	}
	else if (strcmp(value, "B8G8R8A8_SINT") == 0) {
		return VK_FORMAT_B8G8R8A8_SINT;
	}
	else if (strcmp(value, "B8G8R8A8_SRGB") == 0) {
		return VK_FORMAT_B8G8R8A8_SRGB;
	}
	else if (strcmp(value, "A8B8G8R8_UNORM_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_SNORM_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_USCALED_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_USCALED_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_SSCALED_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_UINT_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_UINT_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_SINT_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_SINT_PACK32;
	}
	else if (strcmp(value, "A8B8G8R8_SRGB_PACK32") == 0) {
		return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_UNORM_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_SNORM_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_USCALED_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_USCALED_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_SSCALED_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_UINT_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_UINT_PACK32;
	}
	else if (strcmp(value, "A2R10G10B10_SINT_PACK32") == 0) {
		return VK_FORMAT_A2R10G10B10_SINT_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_UNORM_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_SNORM_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_USCALED_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_USCALED_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_SSCALED_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_UINT_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_UINT_PACK32;
	}
	else if (strcmp(value, "A2B10G10R10_SINT_PACK32") == 0) {
		return VK_FORMAT_A2B10G10R10_SINT_PACK32;
	}
	else if (strcmp(value, "R16_UNORM") == 0) {
		return VK_FORMAT_R16_UNORM;
	}
	else if (strcmp(value, "R16_SNORM") == 0) {
		return VK_FORMAT_R16_SNORM;
	}
	else if (strcmp(value, "R16_USCALED") == 0) {
		return VK_FORMAT_R16_USCALED;
	}
	else if (strcmp(value, "R16_SSCALED") == 0) {
		return VK_FORMAT_R16_SSCALED;
	}
	else if (strcmp(value, "R16_UINT") == 0) {
		return VK_FORMAT_R16_UINT;
	}
	else if (strcmp(value, "R16_SINT") == 0) {
		return VK_FORMAT_R16_SINT;
	}
	else if (strcmp(value, "R16_SFLOAT") == 0) {
		return VK_FORMAT_R16_SFLOAT;
	}
	else if (strcmp(value, "R16G16_UNORM") == 0) {
		return VK_FORMAT_R16G16_UNORM;
	}
	else if (strcmp(value, "R16G16_SNORM") == 0) {
		return VK_FORMAT_R16G16_SNORM;
	}
	else if (strcmp(value, "R16G16_USCALED") == 0) {
		return VK_FORMAT_R16G16_USCALED;
	}
	else if (strcmp(value, "R16G16_SSCALED") == 0) {
		return VK_FORMAT_R16G16_SSCALED;
	}
	else if (strcmp(value, "R16G16_UINT") == 0) {
		return VK_FORMAT_R16G16_UINT;
	}
	else if (strcmp(value, "R16G16_SINT") == 0) {
		return VK_FORMAT_R16G16_SINT;
	}
	else if (strcmp(value, "R16G16_SFLOAT") == 0) {
		return VK_FORMAT_R16G16_SFLOAT;
	}
	else if (strcmp(value, "R16G16B16_UNORM") == 0) {
		return VK_FORMAT_R16G16B16_UNORM;
	}
	else if (strcmp(value, "R16G16B16_SNORM") == 0) {
		return VK_FORMAT_R16G16B16_SNORM;
	}
	else if (strcmp(value, "R16G16B16_USCALED") == 0) {
		return VK_FORMAT_R16G16B16_USCALED;
	}
	else if (strcmp(value, "R16G16B16_SSCALED") == 0) {
		return VK_FORMAT_R16G16B16_SSCALED;
	}
	else if (strcmp(value, "R16G16B16_UINT") == 0) {
		return VK_FORMAT_R16G16B16_UINT;
	}
	else if (strcmp(value, "R16G16B16_SINT") == 0) {
		return VK_FORMAT_R16G16B16_SINT;
	}
	else if (strcmp(value, "R16G16B16_SFLOAT") == 0) {
		return VK_FORMAT_R16G16B16_SFLOAT;
	}
	else if (strcmp(value, "R16G16B16A16_UNORM") == 0) {
		return VK_FORMAT_R16G16B16A16_UNORM;
	}
	else if (strcmp(value, "R16G16B16A16_SNORM") == 0) {
		return VK_FORMAT_R16G16B16A16_SNORM;
	}
	else if (strcmp(value, "R16G16B16A16_USCALED") == 0) {
		return VK_FORMAT_R16G16B16A16_USCALED;
	}
	else if (strcmp(value, "R16G16B16A16_SSCALED") == 0) {
		return VK_FORMAT_R16G16B16A16_SSCALED;
	}
	else if (strcmp(value, "R16G16B16A16_UINT") == 0) {
		return VK_FORMAT_R16G16B16A16_UINT;
	}
	else if (strcmp(value, "R16G16B16A16_SINT") == 0) {
		return VK_FORMAT_R16G16B16A16_SINT;
	}
	else if (strcmp(value, "R16G16B16A16_SFLOAT") == 0) {
		return VK_FORMAT_R16G16B16A16_SFLOAT;
	}
	else if (strcmp(value, "R32_UINT") == 0) {
		return VK_FORMAT_R32_UINT;
	}
	else if (strcmp(value, "R32_SINT") == 0) {
		return VK_FORMAT_R32_SINT;
	}
	else if (strcmp(value, "R32_SFLOAT") == 0) {
		return VK_FORMAT_R32_SFLOAT;
	}
	else if (strcmp(value, "R32G32_UINT") == 0) {
		return VK_FORMAT_R32G32_UINT;
	}
	else if (strcmp(value, "R32G32_SINT") == 0) {
		return VK_FORMAT_R32G32_SINT;
	}
	else if (strcmp(value, "R32G32_SFLOAT") == 0) {
		return VK_FORMAT_R32G32_SFLOAT;
	}
	else if (strcmp(value, "R32G32B32_UINT") == 0) {
		return VK_FORMAT_R32G32B32_UINT;
	}
	else if (strcmp(value, "R32G32B32_SINT") == 0) {
		return VK_FORMAT_R32G32B32_SINT;
	}
	else if (strcmp(value, "R32G32B32_SFLOAT") == 0) {
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	else if (strcmp(value, "R32G32B32A32_UINT") == 0) {
		return VK_FORMAT_R32G32B32A32_UINT;
	}
	else if (strcmp(value, "R32G32B32A32_SINT") == 0) {
		return VK_FORMAT_R32G32B32A32_SINT;
	}
	else if (strcmp(value, "R32G32B32A32_SFLOAT") == 0) {
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	}
	else if (strcmp(value, "R64_UINT") == 0) {
		return VK_FORMAT_R64_UINT;
	}
	else if (strcmp(value, "R64_SINT") == 0) {
		return VK_FORMAT_R64_SINT;
	}
	else if (strcmp(value, "R64_SFLOAT") == 0) {
		return VK_FORMAT_R64_SFLOAT;
	}
	else if (strcmp(value, "R64G64_UINT") == 0) {
		return VK_FORMAT_R64G64_UINT;
	}
	else if (strcmp(value, "R64G64_SINT") == 0) {
		return VK_FORMAT_R64G64_SINT;
	}
	else if (strcmp(value, "R64G64_SFLOAT") == 0) {
		return VK_FORMAT_R64G64_SFLOAT;
	}
	else if (strcmp(value, "R64G64B64_UINT") == 0) {
		return VK_FORMAT_R64G64B64_UINT;
	}
	else if (strcmp(value, "R64G64B64_SINT") == 0) {
		return VK_FORMAT_R64G64B64_SINT;
	}
	else if (strcmp(value, "R64G64B64_SFLOAT") == 0) {
		return VK_FORMAT_R64G64B64_SFLOAT;
	}
	else if (strcmp(value, "R64G64B64A64_UINT") == 0) {
		return VK_FORMAT_R64G64B64A64_UINT;
	}
	else if (strcmp(value, "R64G64B64A64_SINT") == 0) {
		return VK_FORMAT_R64G64B64A64_SINT;
	}
	else if (strcmp(value, "R64G64B64A64_SFLOAT") == 0) {
		return VK_FORMAT_R64G64B64A64_SFLOAT;
	}

	if (strcmp(value, "B10G11R11_UFLOAT_PACK32") == 0) {
		return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	}
	else if (strcmp(value, "E5B9G9R9_UFLOAT_PACK32") == 0) {
		return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
	}
	else if (strcmp(value, "D16_UNORM") == 0) {
		return VK_FORMAT_D16_UNORM;
	}
	else if (strcmp(value, "X8_D24_UNORM_PACK32") == 0) {
		return VK_FORMAT_X8_D24_UNORM_PACK32;
	}
	else if (strcmp(value, "D32_SFLOAT") == 0) {
		return VK_FORMAT_D32_SFLOAT;
	}
	else if (strcmp(value, "S8_UINT") == 0) {
		return VK_FORMAT_S8_UINT;
	}
	else if (strcmp(value, "D16_UNORM_S8_UINT") == 0) {
		return VK_FORMAT_D16_UNORM_S8_UINT;
	}
	else if (strcmp(value, "D24_UNORM_S8_UINT") == 0) {
		return VK_FORMAT_D24_UNORM_S8_UINT;
	}
	else if (strcmp(value, "D32_SFLOAT_S8_UINT") == 0) {
		return VK_FORMAT_D32_SFLOAT_S8_UINT;
	}
	else if (strcmp(value, "BC1_RGB_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC1_RGB_SRGB_BLOCK") == 0) {
		return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
	}
	else if (strcmp(value, "BC1_RGBA_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC1_RGBA_SRGB_BLOCK") == 0) {
		return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
	}
	else if (strcmp(value, "BC2_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC2_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC2_SRGB_BLOCK") == 0) {
		return VK_FORMAT_BC2_SRGB_BLOCK;
	}
	else if (strcmp(value, "BC3_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC3_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC3_SRGB_BLOCK") == 0) {
		return VK_FORMAT_BC3_SRGB_BLOCK;
	}
	else if (strcmp(value, "BC4_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC4_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC4_SNORM_BLOCK") == 0) {
		return VK_FORMAT_BC4_SNORM_BLOCK;
	}
	else if (strcmp(value, "BC5_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC5_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC5_SNORM_BLOCK") == 0) {
		return VK_FORMAT_BC5_SNORM_BLOCK;
	}
	else if (strcmp(value, "BC6H_UFLOAT_BLOCK") == 0) {
		return VK_FORMAT_BC6H_UFLOAT_BLOCK;
	}
	else if (strcmp(value, "BC6H_SFLOAT_BLOCK") == 0) {
		return VK_FORMAT_BC6H_SFLOAT_BLOCK;
	}
	else if (strcmp(value, "BC7_UNORM_BLOCK") == 0) {
		return VK_FORMAT_BC7_UNORM_BLOCK;
	}
	else if (strcmp(value, "BC7_SRGB_BLOCK") == 0) {
		return VK_FORMAT_BC7_SRGB_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8A1_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8A1_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8A8_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
	}
	else if (strcmp(value, "ETC2_R8G8B8A8_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
	}
	else if (strcmp(value, "EAC_R11_UNORM_BLOCK") == 0) {
		return VK_FORMAT_EAC_R11_UNORM_BLOCK;
	}
	else if (strcmp(value, "EAC_R11_SNORM_BLOCK") == 0) {
		return VK_FORMAT_EAC_R11_SNORM_BLOCK;
	}
	else if (strcmp(value, "EAC_R11G11_UNORM_BLOCK") == 0) {
		return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
	}
	else if (strcmp(value, "EAC_R11G11_SNORM_BLOCK") == 0) {
		return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_4x4_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_4x4_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_5x4_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_5x4_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_5x5_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_5x5_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_6x5_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_6x5_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_6x6_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_6x6_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x5_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x5_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x6_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x6_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x8_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_8x8_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x5_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x5_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x6_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x6_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x8_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x8_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x10_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_10x10_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_12x10_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_12x10_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
	}
	else if (strcmp(value, "ASTC_12x12_UNORM_BLOCK") == 0) {
		return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
	}
	else if (strcmp(value, "ASTC_12x12_SRGB_BLOCK") == 0) {
		return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
	}

	return VK_FORMAT_UNDEFINED;
};

//Translates a string to a vulkan color space
VkColorSpaceKHR getColorSpace(char * value) {
	if (strcmp(value, "SRGB_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	}
	else if (strcmp(value, "DISPLAY_P3_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
	}
	else if (strcmp(value, "EXTENDED_SRGB_LINEAR") == 0) {
		return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
	}
	else if (strcmp(value, "DCI_P3_LINEAR") == 0) {
		return VK_COLOR_SPACE_DCI_P3_LINEAR_EXT;
	}
	else if (strcmp(value, "DCI_P3_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
	}
	else if (strcmp(value, "BT709_LINEAR") == 0) {
		return VK_COLOR_SPACE_BT709_LINEAR_EXT;
	}
	else if (strcmp(value, "BT709_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_BT709_NONLINEAR_EXT;
	}
	else if (strcmp(value, "BT2020_LINEAR") == 0) {
		return VK_COLOR_SPACE_BT2020_LINEAR_EXT;
	}
	else if (strcmp(value, "HDR10_ST2084") == 0) {
		return VK_COLOR_SPACE_HDR10_ST2084_EXT;
	}
	else if (strcmp(value, "DOLBYVISION") == 0) {
		return VK_COLOR_SPACE_DOLBYVISION_EXT;
	}
	else if (strcmp(value, "HDR10_HLG") == 0) {
		return VK_COLOR_SPACE_HDR10_HLG_EXT;
	}
	else if (strcmp(value, "ADOBERGB_LINEAR") == 0) {
		return VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT;
	}
	else if (strcmp(value, "ADOBERGB_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
	}
	else if (strcmp(value, "PASS_THROUGH") == 0) {
		return VK_COLOR_SPACE_PASS_THROUGH_EXT;
	}
	else if (strcmp(value, "EXTENDED_SRGB_NONLINEAR") == 0) {
		return VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;
	}

	return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}

//Returns a vulkan present mode from a string value
VkPresentModeKHR getPresentMode(char * value) {
	if (strcmp(value, "MAILBOX") == 0) {
		return VK_PRESENT_MODE_MAILBOX_KHR;
	}
	else if (strcmp(value, "FIFO") == 0) {
		return VK_PRESENT_MODE_FIFO_KHR;
	}
	else if (strcmp(value, "FIFO_RELAXED") == 0) {
		return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
	}
	else if (strcmp(value, "IMMEDIATE") == 0) {
		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	}
	else if (strcmp(value, "DEMAND") == 0) {
		return VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
	}
	else if (strcmp(value, "CONTINUOUS") == 0) {
		return VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

//Returns a bool from a string
bool getBool(char * value)
{
	return (strcmp(value, "TRUE") == 0);
}

//Converts a string to a material type
material_type stringToMaterialType(char * value) {
	if (strcmp(value, "PBR") == 0) {
		return PBR;
	}
	return material_type();
}

//Returns only the value portion of a string from a KEY=VALUE line
std::string getValue(std::string &line) {
	bool started = false;
	int length = 0, start = 0;
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '=') {
			started = true;
			start = i + 1;
		}
		else if (line[i] == ';') {
			break;
		}
		else if (started) {
			length++;
		}
	}
	return line.substr(start, length);
}

int getValue(char * buffer, char * value, int bufferSize)
{
	bool started = false;
	int pos = 0;
	for (int i = 0; i < bufferSize; i++) {
		if (buffer[i] == '=') {
			started = true;
		}
		else if (buffer[i] == ';') {
			value[pos] = '\0';
			return pos+1;
		}
		else if (started && buffer[i]!= ' ' && buffer[i]!='\t') {
			value[pos] = buffer[i];
			pos++;
		}
	}
	return -1;
}

//Converts a line to a scene key
scene_key getSceneKey(char * buffer, int bufferSize)
{
	char key[64];
	int keyPos = 0;
	for (int i = 0; i < bufferSize; i++) {
		if (buffer[i] == '=' || buffer[i] == ';') {
			key[keyPos] = '\0';
			break;
		}
		else if (buffer[i] != ' ' && buffer[i] != '\t') {
			key[keyPos] = buffer[i];
			keyPos++;
		}
	}

	if (strcmp(key, "NAME") == 0) {
		return SCENE_NAME;
	}
	else  if (strcmp(key, "MANAGERS") == 0) {
		return MANAGERS;
	}
	else  if (strcmp(key, "SYSTEMS") == 0) {
		return SYSTEMS;
	}
	else  if (strcmp(key, "ADD_MANAGER") == 0) {
		return ADD_MANAGER;
	}
	else  if (strcmp(key, "ADD_SYSTEM") == 0) {
		return ADD_SYSTEM;
	}
	else  if (strcmp(key, "ENTITY_COUNT") == 0) {
		return ENTITY_COUNT;
	}
	else if (strcmp(key, "ADD_PIPELINE") == 0) {
		return ADD_PIPELINE;
	}
	else if (strcmp(key, "ENTITY") == 0) {
		return ENTITY;
	}
	else if (strcmp(key, "ADD_COMPONENT") == 0) {
		return ADD_COMPONENT;
	}
	else if (strcmp(key, "END") == 0) {
		return END_STATEMENT;
	}
	else if (strcmp(key, "END_ENTITIES") == 0) {
		return END_ENTITIES;
	}
	else if (strcmp(key, "ADD_TAG") == 0) {
		return ADD_TAG;
	}
	else if (strcmp(key, "SIZE") == 0) {
		return SCENE_SIZE;
	}
	else if (strcmp(key, "DEPTH") == 0) {
		return TREE_DEPTH;
	}
	else if (strcmp(key, "SCENE_STRUCTURE") == 0) {
		return SCENE_STRUCTURE;
	}
	else if (strcmp(key, "FILE_LOAD") == 0) {
		return FILE_LOAD_KEY;
	}
	else if (strcmp(key, "DYNAMIC") == 0) {
		return IS_DYNAMIC;
	}
	else {
		return NO_SCENE_KEY;
	}
}

//Gets a component type from a line value
component_type getComponentType(char * value)
{
	if (strcmp(value, "TRANSFORM") == 0) {
		return TRANSFORM;
	}
	else if (strcmp(value, "MESH") == 0) {
		return MESH_COMPONENT;
	}
	else if (strcmp(value, "MATERIAL") == 0) {
		return MATERIAL_COMPONENT;
	}
	else if (strcmp(value, "CAMERA") == 0) {
		return CAMERA;
	}
	else if (strcmp(value, "LIGHT") == 0) {
		return LIGHT_COMPONENT;
	}
	else if (strcmp(value, "PREFAB") == 0) {
		return PREFAB_COMPONENT;
	}
	else if (strcmp(value, "RIGID_BODY") == 0) {
		return RIGID_BODY;
	}
	else if (strcmp(value, "COLLIDER") == 0) {
		return COLLIDER;
	}
	else if (strcmp(value, "ANIMATION") == 0) {
		return ANIMATION_COMPONENT;
	}
	else if (strcmp(value, "ARMATURE") == 0) {
		return ARMATURE_COMPONENT;
	}
	else if (strcmp(value, "ARMATURE_ANIMATION") == 0) {
		return ARMATURE_ANIMATION;
	}
	else if (strcmp(value, "SKINNED_MESH") == 0) {
		return SKINNED_MESH;
	}
	else if (strcmp(value, "TAGS") == 0) {
		return TAGS_COMPONENT;
	}
	else if (strcmp(value, "BOUNDS") == 0) {
		return AABB_COMPONENT;
	}
	return NO_TYPE;
}

//Gets a system type from a line value
system_type getSystemType(char * value)
{
	if (strcmp(value, "RENDER") == 0) {
		return RENDER_SYSTEM;
	}
	else if (strcmp(value, "RIGID_BODY") == 0) {
		return RIGID_BODY_SYSTEM;
	}
	else if (strcmp(value, "COLLISION") == 0) {
		return COLLISION_SYSTEM;
	}
	else if (strcmp(value, "ANIMATION") == 0) {
		return ANIMATION_SYSTEM;
	}
	else if (strcmp(value, "ARMATURE") == 0) {
		return ARMATURE_SYSTEM;
	}
	else if (strcmp(value, "CLIENT") == 0) {
		return CLIENT_NET_SYSTEM;
	}
	else if (strcmp(value, "SERVER") == 0) {
		return SERVER_NET_SYSTEM;
	}
	else if (strcmp(value, "PEER") == 0) {
		return PEER_NET_SYSTEM;
	}
	else if (strcmp(value, "INPUT") == 0) {
		return INPUT_SYSTEM;
	}
	else if (strcmp(value, "SCENE_TREE") == 0) {
		return SCENE_TREE_SYSTEM;
	}
	else if (strcmp(value, "BOUNDS") == 0) {
		return BOUNDS_SYSTEM;
	}
	return NO_SYSTEM_TYPE;
}

//Strips leading tabs and spaces and returns a string
void getSubComponent(char * buffer, char * keyString, int buffer_size)
{
	int pos = 0;
	bool started = false;
	for (int i = 0; i < buffer_size; i++) {
		if (buffer[i] == '=' || buffer[i] == ';') {
			keyString[pos] = '\0';
			break;
		}
		else if (!started && buffer[i] != '\t' && buffer[i] != ' ') {
			started = true;
			keyString[pos] = buffer[i];
			pos++;
		}
		else if (started) {
			keyString[pos] = buffer[i];
			pos++;
		}
	}
}

//Converts a string to a texture type
texture_type getTextureType(char * typeAsString) {
	if (strcmp(typeAsString, "DIFFUSE") == 0) {
		return DIFFUSE;
	}
	else if (strcmp(typeAsString, "METALLIC_SPEC") == 0) {
		return METALLIC_SPEC;
	}
	else if (strcmp(typeAsString, "NORMAL") == 0) {
		return NORMAL;
	}
	else if (strcmp(typeAsString, "AO") == 0) {
		return AO;
	}
	else if (strcmp(typeAsString, "ROUGHNESS") == 0) {
		return ROUGHNESS;
	}
	return NO_TEXTURE_TYPE;
}

//Converts a string to a collider type
collider_type getColliderType(char * typeAsString)
{
	if (strcmp(typeAsString, "SPHERE") == 0) {
		return SPHERE_COLLIDER;
	}
	else if (strcmp(typeAsString, "AABB") == 0) {
		return AABB_COLLIDER;
	}
	else if (strcmp(typeAsString, "BOX") == 0) {
		return BOX_COLLIDER;
	}
	else if (strcmp(typeAsString, "MESH") == 0) {
		return MESH_COLLIDER;
	}
	else if (strcmp(typeAsString, "CAPSULE") == 0) {
		return CAPSULE_COLLIDER;
	}
	return NO_COLLIDER_TYPE;
}

//Translates a string to a light type
light_type getLightTypeFromString(char * typeAsString) {
	if (strcmp(typeAsString, "POINT") == 0) {
		return POINT_LIGHT;
	}
	else if (strcmp(typeAsString, "DIRECTION") == 0) {
		return DIRECTION_LIGHT;
	}
	else if (strcmp(typeAsString, "SPOT") == 0) {
		return SPOT_LIGHT;
	}
	return POINT_LIGHT;
}

//Parses an int from a scene structure word
int getChildCount(char * value) {
	if (strcmp(value, "QUAD") == 0) {
		return 4;
	}
	return 0;
}

/*
	Configuration Structure Functions
*/

//Converts a string to a window mode
void getRefreshRate(char * value, configurationStructure &config) {
	int rate = atoi(value);
	if (rate == -1) {
		int count;
		const GLFWvidmode* modes = glfwGetVideoModes(config.monitor, &count);
		for (int i = 0; i < count; i++) {
			if (modes[i].refreshRate > rate) {
				rate = modes[i].refreshRate;
			}
			std::cout << modes[i].refreshRate << std::endl;
		}
	}
	config.videoSettings->refreshRate = rate;
}

//Converts a string to a window mode
void getResolution(char * value, configurationStructure &config, int valSize) {
	char res[64];
	int pos = 0;
	bool started = false;
	for (int i = 0; i < valSize; i++) {
		if (value[i] == ',') {
			res[pos] = '\0';
			config.videoSettings->width = atoi(res);
			pos = 0;
		}
		else {
			res[pos] = value[i];
			pos++;
		}
	}
	res[pos] = '\0';
	config.videoSettings->height = atoi(res);
}

//Sets the version numbers from the string value
void getAppVersion(char * value, configurationStructure &config, int length) {
	int index = 0, pos = 0;
	char ver[64];
	bool started_0 = false, started_1 = false;
	for (int i = 0; i < length; i++) {
		if (value[i] == ',') {
			ver[pos] = '\0';
			config.appVersion[index] = atoi(ver);
			pos = 0;
			index++;
		}
		else if (value[i] == ';') {
			ver[pos] = '\0';
			config.appVersion[index] = atoi(ver);
			break;
		}
		else {
			ver[pos] = value[i];
			pos++;
		}
	}
}

//Sets the format in the configuration file
void setFormat(char * value, configurationStructure &config) {
	config.surfaceFormat.format = getFormat(value);
}

//Sets the color space in the configuration file from a string
void setColorSpace(char * value, configurationStructure &config) {
	config.surfaceFormat.colorSpace = getColorSpace(value);
}

//Sets the present mode of a configuration structure
void setPresentMode(char * value, configurationStructure &config) {
	config.presentMode = getPresentMode(value);
}

//Sets the depth format of a configuration structure
void setDepthFormat(char * value, configurationStructure &config) {
	config.depthFormat = getFormat(value);
}

int powerFact(int base, int depth)
{
	if (depth == 0) {
		return 1;
	}
	else {
		return (int) pow(base, depth) + powerFact(base, (depth-1));
	}
}

void setNodeCount(configurationStructure & config, int base, int depth)
{
	config.sceneNodesCount = powerFact(base, depth-1);
}

//Adds a feature to the config
void addGpuFeature(char * value, configurationStructure &config) {
	if (strcmp(value, "SAMPLER_ANISOTROPY") == 0) {
		config.featuresRequired.samplerAnisotropy = true;
	}
}


/*
	Vulkan to strings
*/

//Feature list to strings
void displayFeatures(VkPhysicalDeviceFeatures features) {
	std::cout << "Device Features found: " << std::endl;
	if (features.alphaToOne == VK_TRUE) {
		std::cout << "\tALPHA_TO_ONE" << std::endl;
	}
	if (features.depthBiasClamp == VK_TRUE) {
		std::cout << "\tDEPTH_BIAS_CLAMP" << std::endl;
	}
	if (features.depthBounds == VK_TRUE) {
		std::cout << "\tDEPTH_BOUNDS" << std::endl;
	}
	if (features.depthClamp == VK_TRUE) {
		std::cout << "\tDEPTH_CLAMP" << std::endl;
	}
	if (features.drawIndirectFirstInstance == VK_TRUE) {
		std::cout << "\tDRAW_INDIRECT_FIRST_INSTANCE" << std::endl;
	}
	if (features.dualSrcBlend == VK_TRUE) {
		std::cout << "\tDUAL_SOURCE_BLEND" << std::endl;
	}
	if (features.fillModeNonSolid == VK_TRUE) {
		std::cout << "\tFILL_MODE_NON_SOLID" << std::endl;
	}
	if (features.fragmentStoresAndAtomics == VK_TRUE) {
		std::cout << "\tFRAGMENT_STORES_AND_ATOMICS" << std::endl;
	}
	if (features.fullDrawIndexUint32 == VK_TRUE) {
		std::cout << "\tFULL_DRAW_INDEX_UINT_32" << std::endl;
	}
	if (features.geometryShader == VK_TRUE) {
		std::cout << "\tGEOMETRY_SHADER" << std::endl;
	}
	if (features.imageCubeArray == VK_TRUE) {
		std::cout << "\tIMAGE_CUBE_ARRAY" << std::endl;
	}
	if (features.independentBlend == VK_TRUE) {
		std::cout << "\tINDEPENDENT_BLEND" << std::endl;
	}
	if (features.inheritedQueries == VK_TRUE) {
		std::cout << "\tINHERITED_QUERIES" << std::endl;
	}
	if (features.largePoints == VK_TRUE) {
		std::cout << "\tLARGE_POINTS" << std::endl;
	}
	if (features.logicOp == VK_TRUE) {
		std::cout << "\tLOGIC_OP" << std::endl;
	}
	if (features.multiDrawIndirect == VK_TRUE) {
		std::cout << "\tMULTI_DRAW_INDIRECT" << std::endl;
	}
	if (features.multiViewport == VK_TRUE) {
		std::cout << "\tMULTI_VIEWPORT" << std::endl;
	}
	if (features.occlusionQueryPrecise == VK_TRUE) {
		std::cout << "\tOCCLUSION_QUERY_PRECISE" << std::endl;
	}
	if (features.pipelineStatisticsQuery == VK_TRUE) {
		std::cout << "\tPIPELINE_STATISTICS_QUERY" << std::endl;
	}
	if (features.robustBufferAccess == VK_TRUE) {
		std::cout << "\tROBUST_BUFFER_ACCESS" << std::endl;
	}
	if (features.samplerAnisotropy == VK_TRUE) {
		std::cout << "\tSAMPLER_ANISOTROPY" << std::endl;
	}
	if (features.sampleRateShading == VK_TRUE) {
		std::cout << "\tSAMPLE_RATE_SHADING" << std::endl;
	}
	if (features.shaderClipDistance == VK_TRUE) {
		std::cout << "\tSHADER_CLIP_DISTANCE" << std::endl;
	}
	if (features.shaderCullDistance == VK_TRUE) {
		std::cout << "\tSHADER_CULL_DISTANCE" << std::endl;
	}
	if (features.shaderFloat64 == VK_TRUE) {
		std::cout << "\tSHADER_FLOAT_64" << std::endl;
	}
	if (features.shaderImageGatherExtended == VK_TRUE) {
		std::cout << "\tSHADER_IMAGE_GATHER_EXTENDED" << std::endl;
	}
	if (features.shaderInt16 == VK_TRUE) {
		std::cout << "\tSHADER_INT_16" << std::endl;
	}
	if (features.shaderInt64 == VK_TRUE) {
		std::cout << "\tSHADER_INT_64" << std::endl;
	}
	if (features.shaderResourceMinLod == VK_TRUE) {
		std::cout << "\tSHADER_RESOURCE_MIN_LOAD" << std::endl;
	}
	if (features.shaderResourceResidency == VK_TRUE) {
		std::cout << "\tSHADER_RESOURCE_RESIDENCY" << std::endl;
	}
	if (features.shaderSampledImageArrayDynamicIndexing == VK_TRUE) {
		std::cout << "\tSHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING" << std::endl;
	}
	if (features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_BUFFER_ARRAY_DYNAMIC_INDEXING" << std::endl;
	}
	if (features.shaderStorageImageArrayDynamicIndexing == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING" << std::endl;
	}
	if (features.shaderStorageImageExtendedFormats == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_IMAGE_EXTENDED_FORMATS" << std::endl;
	}
	if (features.shaderStorageImageMultisample == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_IMAGE_MULTISAMPLE" << std::endl;
	}
	if (features.shaderStorageImageReadWithoutFormat == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_IMAGE_READ_WITHOUT_FORMAT" << std::endl;
	}
	if (features.shaderStorageImageWriteWithoutFormat == VK_TRUE) {
		std::cout << "\tSHADER_STORAGE_IMAGE_WRITE_WTHOUT_FORMAT" << std::endl;
	}
	if (features.shaderTessellationAndGeometryPointSize == VK_TRUE) {
		std::cout << "\tSHADER_TESSELLATION_AND_GEOMETRY_POINT_SIZE" << std::endl;
	}
	if (features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE) {
		std::cout << "\tSHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING" << std::endl;
	}
	if (features.sparseBinding == VK_TRUE) {
		std::cout << "\tSPARSE_BINDING" << std::endl;
	}
	if (features.sparseResidency16Samples == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_16_SAMPLES" << std::endl;
	}
	if (features.sparseResidency2Samples == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_2_SAMPLES" << std::endl;
	}
	if (features.sparseResidency4Samples == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_4_SAMPLES" << std::endl;
	}
	if (features.sparseResidency8Samples == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_8_SAMPLES" << std::endl;
	}
	if (features.sparseResidencyAliased == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCT_ALIASED" << std::endl;
	}
	if (features.sparseResidencyBuffer == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_BUFFER" << std::endl;
	}
	if (features.sparseResidencyImage2D == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_IMAGE_2D" << std::endl;
	}
	if (features.sparseResidencyImage3D == VK_TRUE) {
		std::cout << "\tSPARSE_RESIDENCY_IMAGE_3D" << std::endl;
	}
	if (features.tessellationShader == VK_TRUE) {
		std::cout << "\tTESSELLATION_SHADER" << std::endl;
	}
	if (features.textureCompressionASTC_LDR == VK_TRUE) {
		std::cout << "\tTEXTURE_COMPRESSION_ASTC_LDR" << std::endl;
	}
	if (features.textureCompressionBC == VK_TRUE) {
		std::cout << "\tTEXTURE_COMPRESSION_BC" << std::endl;
	}
	if (features.textureCompressionETC2 == VK_TRUE) {
		std::cout << "\tTEXTURE_COMPRESSION_ETC_2" << std::endl;
	}
	if (features.variableMultisampleRate == VK_TRUE) {
		std::cout << "\tVARIABLE_MULTISAMPLE_RATE" << std::endl;
	}
	if (features.vertexPipelineStoresAndAtomics == VK_TRUE) {
		std::cout << "\tVERTEX_PIPELINE_STORES_AND_ATOMICS" << std::endl;
	}
	if (features.wideLines == VK_TRUE) {
		std::cout << "\tWIDE_LINES" << std::endl;
	}
};

//Vulkan Format to String
std::string formatToString(VkSurfaceFormatKHR format) {
	std::string retString = std::string();
	if (format.format == VK_FORMAT_UNDEFINED) {
		retString = "UNDEFINED";
	}
	else if (format.format == VK_FORMAT_R4G4_UNORM_PACK8) {
		retString = "R4G4_UNORM_PACK8";
	}
	else if (format.format == VK_FORMAT_R4G4B4A4_UNORM_PACK16) {
		retString = "R4G4B4A4_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_B4G4R4A4_UNORM_PACK16) {
		retString = "B4G4R4A4_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_R5G6B5_UNORM_PACK16) {
		retString = "R5G6B5_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_B5G6R5_UNORM_PACK16) {
		retString = "B5G6R5_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_R5G5B5A1_UNORM_PACK16) {
		retString = "R5G5B5A1_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_B5G5R5A1_UNORM_PACK16) {
		retString = "B5G5R5A1_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_A1R5G5B5_UNORM_PACK16) {
		retString = "A1R5G5B5_UNORM_PACK16 ";
	}
	else if (format.format == VK_FORMAT_R8_UNORM) {
		retString = "R8_UNORM ";
	}
	else if (format.format == VK_FORMAT_R8_SNORM) {
		retString = "R8_SNORM ";
	}
	else if (format.format == VK_FORMAT_R8_USCALED) {
		retString = "R8_USCALED";
	}
	else if (format.format == VK_FORMAT_R8_SSCALED) {
		retString = "R8_SSCALED";
	}
	else if (format.format == VK_FORMAT_R8_UINT) {
		retString = "R8_UINT";
	}
	else if (format.format == VK_FORMAT_R8_SINT) {
		retString = "R8_SINT";
	}
	else if (format.format == VK_FORMAT_R8_SRGB) {
		retString = "R8_SRGB";
	}
	else if (format.format == VK_FORMAT_R8G8_UNORM) {
		retString = "R8G8_UNORM";
	}
	else if (format.format == VK_FORMAT_R8G8_SNORM) {
		retString = "R8G8_SNORM";
	}
	else if (format.format == VK_FORMAT_R8G8_USCALED) {
		retString = "R8G8_USCALED";
	}
	else if (format.format == VK_FORMAT_R8G8_SSCALED) {
		retString = "R8G8_SSCALED";
	}
	else if (format.format == VK_FORMAT_R8G8_UINT) {
		retString = "R8G8_UINT";
	}
	else if (format.format == VK_FORMAT_R8G8_SINT) {
		retString = "R8G8_SINT";
	}
	else if (format.format == VK_FORMAT_R8G8_SRGB) {
		retString = "R8G8_SRGB";
	}
	else if (format.format == VK_FORMAT_R8G8B8_UNORM) {
		retString = "R8G8B8_UNORM";
	}
	else if (format.format == VK_FORMAT_R8G8B8_SNORM) {
		retString = "R8G8B8_SNORM";
	}
	else if (format.format == VK_FORMAT_R8G8B8_USCALED) {
		retString = "R8G8B8_USCALED";
	}
	else if (format.format == VK_FORMAT_R8G8B8_SSCALED) {
		retString = "R8G8B8_SSCALED";
	}
	else if (format.format == VK_FORMAT_R8G8B8_UINT) {
		retString = "R8G8B8_UINT";
	}
	else if (format.format == VK_FORMAT_R8G8B8_SINT) {
		retString = "R8G8B8_SINT";
	}
	else if (format.format == VK_FORMAT_R8G8B8_SRGB) {
		retString = "R8G8B8_SRGB";
	}
	else if (format.format == VK_FORMAT_B8G8R8_UNORM) {
		retString = "B8G8R8_UNORM";
	}
	else if (format.format == VK_FORMAT_B8G8R8_SNORM) {
		retString = "B8G8R8_SNORM";
	}
	else if (format.format == VK_FORMAT_B8G8R8_USCALED) {
		retString = "B8G8R8_USCALED";
	}
	else if (format.format == VK_FORMAT_B8G8R8_SSCALED) {
		retString = "B8G8R8_SSCALED";
	}
	else if (format.format == VK_FORMAT_B8G8R8_UINT) {
		retString = "B8G8R8_UINT";
	}
	else if (format.format == VK_FORMAT_A1R5G5B5_UNORM_PACK16) {
		retString = "B8G8R8_SINT";
	}
	else if (format.format == VK_FORMAT_A1R5G5B5_UNORM_PACK16) {
		retString = "B8G8R8_SRGB";
	}
	else if (format.format == VK_FORMAT_A1R5G5B5_UNORM_PACK16) {
		retString = "R8G8B8A8_UNORM";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_SNORM) {
		retString = "R8G8B8A8_SNORM";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_SNORM) {
		retString = "R8G8B8A8_SNORM";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_USCALED) {
		retString = "R8G8B8A8_USCALED";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_SSCALED) {
		retString = "R8G8B8A8_SSCALED";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_UINT) {
		retString = "R8G8B8A8_UINT";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_SINT) {
		retString = "R8G8B8A8_SINT";
	}
	else if (format.format == VK_FORMAT_R8G8B8A8_SRGB) {
		retString = "R8G8B8A8_SRGB";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_UNORM) {
		retString = "B8G8R8A8_UNORM";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_SNORM) {
		retString = "B8G8R8A8_SNORM";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_SSCALED) {
		retString = "B8G8R8A8_SSCALED";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_USCALED) {
		retString = "B8G8R8A8_USCALED";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_UINT) {
		retString = "B8G8R8A8_UINT";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_SINT) {
		retString = "B8G8R8A8_SINT";
	}
	else if (format.format == VK_FORMAT_B8G8R8A8_SRGB) {
		retString = "B8G8R8A8_SRGB";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_UNORM_PACK32) {
		retString = "A8B8G8R8_UNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_SNORM_PACK32) {
		retString = "A8B8G8R8_SNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_USCALED_PACK32) {
		retString = "A8B8G8R8_USCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_SSCALED_PACK32) {
		retString = "A8B8G8R8_SSCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_UINT_PACK32) {
		retString = "A8B8G8R8_UINT_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_SINT_PACK32) {
		retString = "A8B8G8R8_SINT_PACK32";
	}
	else if (format.format == VK_FORMAT_A8B8G8R8_SRGB_PACK32) {
		retString = "A8B8G8R8_SRGB_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_UNORM_PACK32) {
		retString = "A2R10G10B10_UNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_SNORM_PACK32) {
		retString = "A2R10G10B10_SNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_USCALED_PACK32) {
		retString = "A2R10G10B10_USCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_SSCALED_PACK32) {
		retString = "A2R10G10B10_SSCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_UINT_PACK32) {
		retString = "A2R10G10B10_UINT_PACK32";
	}
	else if (format.format == VK_FORMAT_A2R10G10B10_SINT_PACK32) {
		retString = "A2R10G10B10_SINT_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32) {
		retString = "A2B10G10R10_UNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_SNORM_PACK32) {
		retString = "A2B10G10R10_SNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_USCALED_PACK32) {
		retString = "A2B10G10R10_USCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_SSCALED_PACK32) {
		retString = "A2B10G10R10_SSCALED_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_UINT_PACK32) {
		retString = "A2B10G10R10_UINT_PACK32";
	}
	else if (format.format == VK_FORMAT_A2B10G10R10_SINT_PACK32) {
		retString = "A2B10G10R10_SINT_PACK32";
	}
	else if (format.format == VK_FORMAT_R16_UNORM) {
		retString = "R16_UNORM ";
	}
	else if (format.format == VK_FORMAT_R16_SNORM) {
		retString = "R16_SNORM";
	}
	else if (format.format == VK_FORMAT_R16_USCALED) {
		retString = "R16_USCALED";
	}
	else if (format.format == VK_FORMAT_R16_SSCALED) {
		retString = "R16_SSCALED";
	}
	else if (format.format == VK_FORMAT_R16_UINT) {
		retString = "R16_UINT";
	}
	else if (format.format == VK_FORMAT_R16_SINT) {
		retString = "R16_SINT";
	}
	else if (format.format == VK_FORMAT_R16_SFLOAT) {
		retString = "R16_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R16G16_UNORM) {
		retString = "R16G16_UNORM";
	}
	else if (format.format == VK_FORMAT_R16G16_SNORM) {
		retString = "R16G16_SNORM";
	}
	else if (format.format == VK_FORMAT_R16G16_USCALED) {
		retString = "R16G16_USCALED";
	}
	else if (format.format == VK_FORMAT_R16G16_SSCALED) {
		retString = "R16G16_SSCALED";
	}
	else if (format.format == VK_FORMAT_R16G16_UINT) {
		retString = "R16G16_UINT";
	}
	else if (format.format == VK_FORMAT_R16G16_SINT) {
		retString = "R16G16_SINT";
	}
	else if (format.format == VK_FORMAT_R16G16_SFLOAT) {
		retString = "R16G16_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R16G16B16_UNORM) {
		retString = "R16G16B16_UNORM";
	}
	else if (format.format == VK_FORMAT_R16G16B16_SNORM) {
		retString = "R16G16B16_SNORM";
	}
	else if (format.format == VK_FORMAT_R16G16B16_USCALED) {
		retString = "R16G16B16_USCALED";
	}
	else if (format.format == VK_FORMAT_R16G16B16_SSCALED) {
		retString = "R16G16B16_SSCALED";
	}
	else if (format.format == VK_FORMAT_R16G16B16_UINT) {
		retString = "R16G16B16_UINT";
	}
	else if (format.format == VK_FORMAT_R16G16B16_SINT) {
		retString = "R16G16B16_SINT";
	}
	else if (format.format == VK_FORMAT_R16G16B16_SFLOAT) {
		retString = "R16G16B16_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_UNORM) {
		retString = "R16G16B16A16_UNORM";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_SNORM) {
		retString = "R16G16B16A16_SNORM";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_USCALED) {
		retString = "R16G16B16A16_USCALED";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_SSCALED) {
		retString = "R16G16B16A16_SSCALED";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_UINT) {
		retString = "R16G16B16A16_UINT";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_SINT) {
		retString = "R16G16B16A16_SINT";
	}
	else if (format.format == VK_FORMAT_R16G16B16A16_SFLOAT) {
		retString = "R16G16B16A16_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R32_UINT) {
		retString = "R32_UINT";
	}
	else if (format.format == VK_FORMAT_R32_SINT) {
		retString = "R32_SINT";
	}
	else if (format.format == VK_FORMAT_R32_SFLOAT) {
		retString = "R32_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R32G32_UINT) {
		retString = "R32G32_UINT";
	}
	else if (format.format == VK_FORMAT_R32G32_SINT) {
		retString = "R32G32_SINT";
	}
	else if (format.format == VK_FORMAT_R32G32_SFLOAT) {
		retString = "R32G32_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R32G32B32_UINT) {
		retString = "R32G32B32_UINT";
	}
	else if (format.format == VK_FORMAT_R32G32B32_SINT) {
		retString = "R32G32B32_SINT";
	}
	else if (format.format == VK_FORMAT_R32G32B32_SFLOAT) {
		retString = "R32G32B32_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R32G32B32A32_UINT) {
		retString = "R32G32B32A32_UINT";
	}
	else if (format.format == VK_FORMAT_R32G32B32A32_SINT) {
		retString = "R32G32B32A32_SINT";
	}
	else if (format.format == VK_FORMAT_R32G32B32A32_SFLOAT) {
		retString = "R32G32B32A32_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R64_UINT) {
		retString = "R64_UINT";
	}
	else if (format.format == VK_FORMAT_R64_SINT) {
		retString = "R64_SINT";
	}
	else if (format.format == VK_FORMAT_R64_SFLOAT) {
		retString = "R64_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R64_SFLOAT) {
		retString = "R64_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R64G64_SINT) {
		retString = "R64G64_SINT";
	}
	else if (format.format == VK_FORMAT_R64G64_SFLOAT) {
		retString = "R64G64_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R64G64B64_UINT) {
		retString = "R64G64B64_UINT";
	}
	else if (format.format == VK_FORMAT_R64G64B64_SINT) {
		retString = "R64G64B64_SINT";
	}
	else if (format.format == VK_FORMAT_R64G64B64_SFLOAT) {
		retString = "R64G64B64_SFLOAT";
	}
	else if (format.format == VK_FORMAT_R64G64B64A64_UINT) {
		retString = "R64G64B64A64_UINT";
	}
	else if (format.format == VK_FORMAT_R64G64B64A64_SINT) {
		retString = "R64G64B64A64_SINT";
	}
	if (format.format == VK_FORMAT_R64G64B64A64_SFLOAT) {
		retString = "R64G64B64A64_SFLOAT";
	}
	else if (format.format == VK_FORMAT_B10G11R11_UFLOAT_PACK32) {
		retString = "B10G11R11_UFLOAT_PACK32";
	}
	else if (format.format == VK_FORMAT_E5B9G9R9_UFLOAT_PACK32) {
		retString = "E5B9G9R9_UFLOAT_PACK32";
	}
	else if (format.format == VK_FORMAT_D16_UNORM) {
		retString = "D16_UNORM";
	}
	else if (format.format == VK_FORMAT_X8_D24_UNORM_PACK32) {
		retString = "X8_D24_UNORM_PACK32";
	}
	else if (format.format == VK_FORMAT_D32_SFLOAT) {
		retString = "D32_SFLOAT";
	}
	else if (format.format == VK_FORMAT_S8_UINT) {
		retString = "S8_UINT";
	}
	else if (format.format == VK_FORMAT_D16_UNORM_S8_UINT) {
		retString = "D16_UNORM_S8_UINT";
	}
	else if (format.format == VK_FORMAT_D24_UNORM_S8_UINT) {
		retString = "D24_UNORM_S8_UINT";
	}
	else if (format.format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
		retString = "D32_SFLOAT_S8_UINT";
	}
	else if (format.format == VK_FORMAT_BC1_RGB_UNORM_BLOCK) {
		retString = "BC1_RGB_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC1_RGB_SRGB_BLOCK) {
		retString = "BC1_RGB_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK) {
		retString = "BC1_RGBA_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC1_RGBA_SRGB_BLOCK) {
		retString = "BC1_RGBA_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC2_UNORM_BLOCK) {
		retString = "BC2_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC2_SRGB_BLOCK) {
		retString = "BC2_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC3_UNORM_BLOCK) {
		retString = "BC3_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC3_SRGB_BLOCK) {
		retString = "BC3_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC4_UNORM_BLOCK) {
		retString = "BC4_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC4_SNORM_BLOCK) {
		retString = "BC4_SNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC5_UNORM_BLOCK) {
		retString = "BC5_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC5_SNORM_BLOCK) {
		retString = "BC5_SNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC6H_UFLOAT_BLOCK) {
		retString = "BC6H_UFLOAT_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC6H_SFLOAT_BLOCK) {
		retString = "BC6H_SFLOAT_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC7_UNORM_BLOCK) {
		retString = "BC7_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_BC7_SRGB_BLOCK) {
		retString = "BC7_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK) {
		retString = "ETC2_R8G8B8_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK) {
		retString = "ETC2_R8G8B8_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK) {
		retString = "ETC2_R8G8B8A1_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK) {
		retString = "ETC2_R8G8B8A1_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK) {
		retString = "ETC2_R8G8B8A8_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK) {
		retString = "ETC2_R8G8B8A8_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_EAC_R11_UNORM_BLOCK) {
		retString = "EAC_R11_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_EAC_R11_SNORM_BLOCK) {
		retString = "EAC_R11_SNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_EAC_R11G11_UNORM_BLOCK) {
		retString = "EAC_R11G11_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_EAC_R11G11_SNORM_BLOCK) {
		retString = "EAC_R11G11_SNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_4x4_UNORM_BLOCK) {
		retString = "ASTC_4x4_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_4x4_SRGB_BLOCK) {
		retString = "ASTC_4x4_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_5x4_UNORM_BLOCK) {
		retString = "ASTC_5x4_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_5x4_SRGB_BLOCK) {
		retString = "ASTC_5x4_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_5x5_UNORM_BLOCK) {
		retString = "ASTC_5x5_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_5x5_SRGB_BLOCK) {
		retString = "ASTC_5x5_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_6x5_UNORM_BLOCK) {
		retString = "ASTC_6x5_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_6x5_SRGB_BLOCK) {
		retString = "ASTC_6x5_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_6x6_UNORM_BLOCK) {
		retString = "ASTC_6x6_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_6x6_SRGB_BLOCK) {
		retString = "ASTC_6x6_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x5_UNORM_BLOCK) {
		retString = "ASTC_8x5_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x5_SRGB_BLOCK) {
		retString = "ASTC_8x5_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x6_UNORM_BLOCK) {
		retString = "ASTC_8x6_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x6_SRGB_BLOCK) {
		retString = "ASTC_8x6_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x8_UNORM_BLOCK) {
		retString = "ASTC_8x8_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_8x8_SRGB_BLOCK) {
		retString = "ASTC_8x8_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x5_UNORM_BLOCK) {
		retString = "ASTC_10x5_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x5_SRGB_BLOCK) {
		retString = "ASTC_10x5_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x6_UNORM_BLOCK) {
		retString = "ASTC_10x6_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x6_SRGB_BLOCK) {
		retString = "ASTC_10x6_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x8_UNORM_BLOCK) {
		retString = "ASTC_10x8_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x8_SRGB_BLOCK) {
		retString = "ASTC_10x8_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x10_UNORM_BLOCK) {
		retString = "ASTC_10x10_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_10x10_SRGB_BLOCK) {
		retString = "ASTC_10x10_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_12x10_UNORM_BLOCK) {
		retString = "ASTC_12x10_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_12x10_SRGB_BLOCK) {
		retString = "ASTC_12x10_SRGB_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_12x12_UNORM_BLOCK) {
		retString = "ASTC_12x12_UNORM_BLOCK";
	}
	else if (format.format == VK_FORMAT_ASTC_12x12_SRGB_BLOCK) {
		retString = "ASTC_12x12_SRGB_BLOCK";
	}
	else if (retString.length() < 1) {
		retString = "unidentified";
	}


	retString.append(",");


	if (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
		retString.append("SRGB_NONLINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT) {
		retString.append("DISPLAY_P3_NONLINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) {
		retString.append("EXTENDED_SRGB_LINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_DCI_P3_LINEAR_EXT) {
		retString.append("DCI_P3_LINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT) {
		retString.append("DCI_P3_NONLINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_BT709_LINEAR_EXT) {
		retString.append("BT709_LINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_BT709_NONLINEAR_EXT) {
		retString.append("BT709_NONLINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_BT2020_LINEAR_EXT) {
		retString.append("BT2020_LINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT) {
		retString.append("HDR10_ST2084");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_DOLBYVISION_EXT) {
		retString.append("DOLBYVISION");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_HDR10_HLG_EXT) {
		retString.append("HDR10_HLG");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT) {
		retString.append("ADOBERGB_LINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT) {
		retString.append("ADOBERGB_NONLINEAR");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_PASS_THROUGH_EXT) {
		retString.append("PASS_THROUGH");
	}
	else if (format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT) {
		retString.append("EXTENDED_SRGB_NONLINEAR");
	}
	else {
		retString.append("unidentified");
	}
	return retString;
}

//Present Mode To String
std::string presentModeToString(VkPresentModeKHR mode) {
	std::string retString = std::string();
	if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
		retString = "IMMEDIATE";
	}
	else if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
		retString = "MAILBOX";
	}
	else if (mode == VK_PRESENT_MODE_FIFO_KHR) {
		retString = "FIFO";
	}
	else if (mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
		retString = "FIFO_RELAXED";
	}
	else if (mode == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR) {
		retString = "SHARED_DEMAND_REFRESH";
	}
	else if (mode == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR) {
		retString = "SHARED_CONTINUOUS_REFRESH";
	}
	else {
		retString = "unidentified";
	}

	return retString;
}

//Returns whether or not the selected format contains a stencil component
bool hasStencilComponent(VkFormat format) {
	if (format == VK_FORMAT_UNDEFINED) {
		return false;
	}
	else if (format == VK_FORMAT_D16_UNORM) {
		return false;
	}
	else if (format == VK_FORMAT_X8_D24_UNORM_PACK32) {
		return false;
	}
	else if (format == VK_FORMAT_D32_SFLOAT) {
		return false;
	}
	else if (format == VK_FORMAT_S8_UINT) {
		return true;
	}
	else if (format == VK_FORMAT_D16_UNORM_S8_UINT) {
		return true;
	}
	else if (format == VK_FORMAT_D24_UNORM_S8_UINT) {
		return true;
	}
	else if (format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
		return true;
	}
	else {
		return true;
	}
}