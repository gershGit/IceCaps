#include "..\Headers\VulkanApplication.h"

std::unique_ptr<VulkanApplication> VulkanApplication::instance;
std::once_flag VulkanApplication::onlyOnce;

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;

VulkanApplication::VulkanApplication()
{
	instanceObj.layerExtension.getInstanceLayerProperties();

	deviceObj = NULL;
	debugFlag = true;
	rendererPointer = NULL;
}

VulkanApplication::~VulkanApplication()
{
	delete rendererPointer;
	rendererPointer = NULL;
}

VulkanApplication* VulkanApplication::GetInstance() {
	std::call_once(onlyOnce, []() {instance.reset(new VulkanApplication()); });
	return instance.get();
}

VkResult VulkanApplication::createVulkanInstance(std::vector<const char *>& layers, std::vector<const char *>& extensionNames, const char* applicationName) {
	return instanceObj.createInstance(layers, extensionNames, applicationName);
}

VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions) {
	deviceObj = new VulkanDevice(gpu);
	if (!deviceObj) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	//Print the devices available layer and their extensions
	deviceObj->layerExtension.getDeviceExtensionProperties(gpu);

	//Get the physical device or GPU properties
	vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProps);

	//Get the memory properties from the physical device or GPU
	vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

	// Query the availabe queues on the physical device and their properties.
	deviceObj->getPhysicalDeviceQueuesAndProperties();

	//Query the available queues on the physical device and their properties
	deviceObj->getGraphicsQueueHandle();

	//Create logical device, ensure that this device is connected to the graphics queue
	return deviceObj->createDevice(layers, extensions);
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList)
{
	//Holds the gpu count
	uint32_t gpuDeviceCount;

	//Get gpu count
	VkResult result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, NULL);
	assert(result == VK_SUCCESS);

	assert(gpuDeviceCount);

	//Make space for retrieval
	gpuList.resize(gpuDeviceCount);

	//Get Physical device object
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanApplication::initialize() {
	char title[] = "Project Icecaps";

	//Check if the supplied layers are supported or not
	if (debugFlag) {
		instanceObj.layerExtension.areLayersSupported(layerNames);
	}

	//Create the Vulkan instance with specified layer and extension names.
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	//Create debugging report if debugging is enabled
	if (debugFlag) {
		instanceObj.layerExtension.createDebugReportCallback();
	}

	//Get the list of physical devices on the system
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);
	int gpuCount = gpuList.size();

	//Handshake with chosen device
	//TODO implement proper choosing of device
	if (gpuList.size() > 0) {
		bool amdFound = false;
		std::vector<VkPhysicalDeviceProperties> deviceProps;
		deviceProps.resize(gpuCount);
		std::cout << "-------Detected " << gpuCount << " gpus on system----------" << std::endl;

		for (int i = 0; i < gpuCount; i++) {
			vkGetPhysicalDeviceProperties(gpuList[i], &deviceProps[i]);
			std::cout << "\n Checking device " << i << std::endl;
			std::cout << "\tDevice vendor ID: " << deviceProps[i].vendorID << std::endl;
			std::cout << "\tDevice name: " << deviceProps[i].deviceName << std::endl;
			std::cout << "\tSupported Vulkan API: " << deviceProps[i].apiVersion << std::endl;
			std::cout << "\tDriver version: " << deviceProps[i].driverVersion << std::endl;
			if (deviceProps[i].vendorID == 4098) { //4098 = AMD 
				amdFound = true;
				std::cout << "-----Handshaking with AMD gpu" << std::endl;
				handShakeWithDevice(&gpuList[1], layerNames, deviceExtensionNames);
				break;
			}
		}
		if (!amdFound) {
			std::cout << "Handshaking with gpu from vendor " << deviceProps[0].vendorID << std::endl;
			handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
		}
	}

	rendererPointer = new VulkanRenderer(this, deviceObj);
	rendererPointer->initialize(640, 480);
}
	
void VulkanApplication::deInitialize() {
	std::cout << "Destroying user-defined vulkan classes" << std::endl;

	//Destroy all the pipeline objects
	rendererPointer->destroyPipeline();

	//Destroy the associated pipeline cache
	rendererPointer->getPipelineObject()->destroyPipelineCache();

	rendererPointer->getShader()->destroyShaders();
	rendererPointer->destroyFramebuffer();
	rendererPointer->destroyRenderpass();
	rendererPointer->destroyDrawableVertexBuffer();
	rendererPointer->destroyDrawableCommandBuffer();
	rendererPointer->destroyDepthBuffer();
	rendererPointer->getSwapchain()->destroySwapchain();
	rendererPointer->destroyCommandBuffer();
	rendererPointer->destroyCommandPool();
	rendererPointer->destroyPresentationWindow();
	deviceObj->destroyDevice();
	if (debugFlag) {
		instanceObj.layerExtension.destroyDebugReportCallback();
	}
	instanceObj.destroyInstance();
}

void VulkanApplication::prepare() {
	std::cout << "Preparing application" << std::endl;
	rendererPointer->prepare();
}

void VulkanApplication::update() {
	std::cout << "Updating application. Game loop" << std::endl;
}

bool VulkanApplication::render() {
	std::cout << "Rendering..." << std::endl;
	return rendererPointer->render();
}