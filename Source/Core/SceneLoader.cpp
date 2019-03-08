#include "Core/SceneLoader.h"
#include "Core/TransformManager.h"
#include "Vulkan/V_Instance.h"
#include "Vulkan/V_Components.h"
#include "Core/ManagersFactories.h"
#include "Core/SystemsHeader.h"
#include "Core/StringTranslation.h"
#include <fstream>


SceneLoader::SceneLoader()
{
}

//Generates a vector from a string of floats separated by commas
template <class Vector>
Vector getVectorFromString(std::string value) {
	int start = 0, length = 0;
	int vectorIndex = 0;
	Vector returnVector = Vector();
	for (int i = 0; i < value.length(); i++) {
		if (value[i] == ',') {
			returnVector[vectorIndex] = strtof(value.substr(start, length).c_str(), nullptr);
			vectorIndex++;
			start = i + 1;
			length = 0;
		}
		else {
			length++;
		}
	}
	returnVector[vectorIndex] = strtof(value.substr(start, length).c_str(), nullptr);
	return returnVector;
}

//Builder Functions ---------------------------------------------------------------------------

//Adds managers to hold components for various component types
void addManagers(configurationStructure &config, std::vector<ComponentManager*> &managers, std::ifstream &fileStream) {
	std::cout << "Adding Managers" << std::endl;
	std::string line, value;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		if (getSceneKey(line) == END_STATEMENT) {
			break;
		}
		else if (getSceneKey(line) == ADD_MANAGER) {
			if (getComponentType(value) == TRANSFORM) {
				std::cout << "\tAdding Transform Manager" << std::endl;
				managers.push_back(new TransformManager());
			}
			else if (getComponentType(value) == PREFAB_COMPONENT) {
				std::cout << "\tAdding Prefab Manager" << std::endl;
				managers.push_back(new PrefabManager());
			}
			else if (getComponentType(value) == MATERIAL_COMPONENT) {
				std::cout << "\tAdding Material Manager" << std::endl;
				if (config.api = Vulkan) {
					managers.push_back(new V_MaterialManager());
				}
			}
			else if (getComponentType(value) == MESH_COMPONENT) {
				std::cout << "\tAdding Mesh Manager" << std::endl;
				if (config.api = Vulkan) {
					managers.push_back(new V_MeshManager());
				}
			}
			else if (getComponentType(value) == CAMERA) {
				std::cout << "\tAdding Camera Manager" << std::endl;
				managers.push_back(new CameraManager());
			}
			else if (getComponentType(value) == LIGHT_COMPONENT) {
				std::cout << "\tAdding Light Manager" << std::endl;
				managers.push_back(new LightManager());
			}
		}
	}
}

//Fills a system with all necessary managers
void fillSystemWithManagers(EntitySystem * system, std::vector<ComponentManager*> &managers) {
	for (component_type managerType : system->operatesOn) {
		if (managerType == TRANSFORM) {
			system->managers->push_back(getTransformManager(managers));
		}
		else if (managerType == PREFAB_COMPONENT) {
			system->managers->push_back(getPrefabManager(managers));
		}
		else if (managerType == CAMERA) {
			system->managers->push_back(getCameraManager(managers));
		}
		else if (managerType == LIGHT_COMPONENT) {
			system->managers->push_back(getLightManager(managers));
		}
		else if (managerType == V_MESH) {
			system->managers->push_back(getVulkanMeshManager(managers));
		}
		else if (managerType == V_MATERIAL) {
			system->managers->push_back(getVulkanMaterialManager(managers));
		}
	}
}

//Fills a systems entity list with all matching entities it runs on
void fillSystemWithEntities(EntitySystem * system, std::vector<ComponentManager*> &managers, int entityCount) {
	for (std::vector<component_type> requiredSet : system->entityListRequiredComponents) {
		for (int entityID = 0; entityID < entityCount; entityID++) {
			bool validEntity = true;
			for (component_type requiredComponent : requiredSet) {
				if (requiredComponent == TRANSFORM) {
					if (!getTransformManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				} 
				else if (requiredComponent == PREFAB_COMPONENT) {
					if (!getPrefabManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == V_MESH) {
		
					if (!getVulkanMeshManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == V_MATERIAL) {
					if (!getVulkanMaterialManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == CAMERA) {
					if (!getCameraManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == LIGHT_COMPONENT) {
					if (!getLightManager(managers)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
			}
			if (validEntity) {
				system->addEntity(entityID);
			}
		}
	}
}

//Adds systems and populates them with the necessary managers
void addSystems(configurationStructure &config, std::vector<EntitySystem*> &systems, std::vector<ComponentManager*> &managers, std::ifstream &fileStream) {
	std::cout << "Adding Systems" << std::endl;
	std::string line, value;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		if (getSceneKey(line) == END_STATEMENT) {
			break;
		}
		else if (getSceneKey(line) == ADD_SYSTEM) {
			if (getSystemType(value) == RENDERER) {
				if (config.api == Vulkan) {
					std::cout << "\tAdding render system" << std::endl;
					V_RenderSystem* renderSystem = new V_RenderSystem();		
					renderSystem->setSwapchain(config.apiInfo.v_Instance->getSwapchain());
					systems.push_back(renderSystem);
				}
			}
		}
	}
	for (EntitySystem* system : systems) {
		fillSystemWithManagers(system, managers);
		system->setConfig(config);
		system->initialize();
	}
}

// with the correct buffers and descriptors for enough lights
void addVulkanPipeline(V_Instance *instance, material_type mType, std::ifstream &fileStream) {
	int max_lights = 0;
	std::string line, value;

	while (std::getline(fileStream, line)) {
		if (strcmp(getSubComponent(line).c_str(), "MAX_LIGHTS") == 0) {
			max_lights = atoi(getValue(line).c_str());
		}
		else if (strcmp(getSubComponent(line).c_str(), "END") == 0) {
			break;
		}
	}
	instance->addGraphicsPipeline(PBR, max_lights);
}

//Adds a pipeline pased on the type of shaders it will use
void addPipeline(configurationStructure &config, std::string value, std::ifstream &fileStream) {
	material_type mType = stringToMaterialType(value);
	if (config.apiInfo.vulkan) {
		addVulkanPipeline(config.apiInfo.v_Instance, mType, fileStream);
	}
}

//Creates a transform component from a filestream until reaching END keyword
transform buildTransformComponent(std::ifstream &fileStream) {
	transform retTransform = transform();
	retTransform.scale = glm::vec3(1);
	std::string line, value, subComponent;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		subComponent = getSubComponent(line);
		if (strcmp(subComponent.c_str(), "POSITION") == 0) {
			retTransform.pos = getVectorFromString<glm::vec3>(value);
		}
		else if (strcmp(subComponent.c_str(), "ROTATION") == 0) {
			retTransform.rot = getVectorFromString<glm::vec3>(value);
		}
		else if (strcmp(subComponent.c_str(), "SCALE") == 0) {
			retTransform.scale = getVectorFromString<glm::vec3>(value);
		}
		else if (strcmp(subComponent.c_str(), "END") == 0) {
			break;
		}
	}
	return retTransform;
}

//Creates a vulkan mesh component with the help of a V_MeshFactory
v_mesh buildVulkanMeshComponent(std::ifstream &fileStream, configurationStructure &config) {
	v_mesh retMesh = v_mesh();
	std::string meshFile = config.gamePath;
	std::string line, value, subComponent;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		subComponent = getSubComponent(line);
		if (strcmp(subComponent.c_str(), "FILE_LOAD") == 0) {
			V_MeshFactory::loadFromFile(meshFile.append(value).c_str(), retMesh, config);
		}
		else if (strcmp(subComponent.c_str(), "END") == 0) {
			break;
		}
	}
	return retMesh;
}

//Creates a vulkan material component with the help of a V_MaterialFactory
v_material buildVulkanMaterialComponent(std::ifstream &fileStream, configurationStructure &config) {
	v_material retMaterial = v_material();
	std::string meshFile = config.gamePath;
	std::string line, value, subComponent;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		subComponent = getSubComponent(line);
		if (strcmp(subComponent.c_str(), "FILE_LOAD") == 0) {
			V_MaterialFactory::loadMaterialFromFile(meshFile.append(value), retMaterial, config);
		}
		else if (strcmp(subComponent.c_str(), "END") == 0) {
			break;
		}
	}
	return retMaterial;
}

//Creates a vulkan camera from the file stream and config information
camera buildCamera(std::ifstream &fileStream, configurationStructure &config) {
	camera retCamera = camera();
	std::string line, value, subComponent;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		subComponent = getSubComponent(line);
		if (strcmp(subComponent.c_str(), "FOV") == 0) {
			retCamera.fov = strtof(value.c_str(), nullptr);
		}
		else if (strcmp(subComponent.c_str(), "NEAR") == 0) {
			retCamera.near = strtof(value.c_str(), nullptr);
		}
		else if (strcmp(subComponent.c_str(), "FAR") == 0) {
			retCamera.far = strtof(value.c_str(), nullptr);
		}
		else if (strcmp(subComponent.c_str(), "LOOK") == 0) {
			retCamera.lookDirection = glm::normalize(getVectorFromString<glm::vec3>(value));
		}
		else if (strcmp(subComponent.c_str(), "UP") == 0) {
			retCamera.upDirection = glm::normalize(getVectorFromString<glm::vec3>(value));
		}
		else if (strcmp(subComponent.c_str(), "END") == 0) {
			break;
		}
	}
	return retCamera;
}

//Translates a string to a light type
light_type getLightTypeFromString(std::string inString) {
	if (strcmp(inString.c_str(), "POINT") == 0) {
		return POINT_LIGHT;
	}
	else if (strcmp(inString.c_str(), "DIRECTION") == 0) {
		return DIRECTION_LIGHT;
	}
	else if (strcmp(inString.c_str(), "SPOT") == 0) {
		return SPOT_LIGHT;
	}
	return POINT_LIGHT;
}

//Creates a light component from a file stream
light buildLight(std::ifstream &fileStream, configurationStructure &config) {
	light retLight = light();
	std::string line, value, subComponent;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		subComponent = getSubComponent(line);
		if (strcmp(subComponent.c_str(), "LIGHT_TYPE") == 0) {
			retLight.lType = getLightTypeFromString(value);
		}
		else if (strcmp(subComponent.c_str(), "COLOR") == 0) {
			retLight.color = getVectorFromString<glm::vec4>(value);
		}
		else if (strcmp(subComponent.c_str(), "END") == 0) {
			break;
		}
	}
	return retLight;
}

//Loads in an entity under the vulkan api
void loadVulkanEntity(int entityID, std::vector<ComponentManager*>& componentManagers, std::ifstream &fileStream, configurationStructure &config) {
	std::cout << "Adding entity: " << entityID << std::endl;
	std::string line, value;
	while (std::getline(fileStream, line)) {
		value = getValue(line);
		if (getSceneKey(line) == END_STATEMENT) {
			break;
		}
		else if (getSceneKey(line) == ADD_COMPONENT) {
			if (getComponentType(value) == TRANSFORM) {
				std::cout << "\tAdding Transform Component" << std::endl;
				transform tempTransform = buildTransformComponent(fileStream);
				getTransformManager(componentManagers)->setTransform(entityID, tempTransform);
			}
			else if (getComponentType(value) == MESH_COMPONENT) {
				v_mesh tempMesh = buildVulkanMeshComponent(fileStream, config);
				getVulkanMeshManager(componentManagers)->addComponent(entityID);
				getVulkanMeshManager(componentManagers)->setComponent(entityID, tempMesh);
			}
			else if (getComponentType(value) == MATERIAL_COMPONENT) {
				v_material tempMaterial = buildVulkanMaterialComponent(fileStream, config);
				getVulkanMaterialManager(componentManagers)->addComponent(entityID);
				getVulkanMaterialManager(componentManagers)->setComponent(entityID, tempMaterial);
			}
			else if (getComponentType(value) == PREFAB_COMPONENT) {
				std::cout << "\tAdding Prefab Component" << std::endl;
				prefab tempPrefab = prefab();
				std::getline(fileStream, line);
				tempPrefab.baseEntity = atoi(getValue(line).c_str());
				getPrefabManager(componentManagers)->addComponent(entityID);
				getPrefabManager(componentManagers)->setComponent(entityID, tempPrefab);
			}
			else if (getComponentType(value) == CAMERA) {
				std::cout << "\tAdding Camera Component" << std::endl;
				camera tempCam = buildCamera(fileStream, config);
				getCameraManager(componentManagers)->addComponent(entityID);
				getCameraManager(componentManagers)->setComponent(entityID, tempCam);
			}
			else if (getComponentType(value) == LIGHT_COMPONENT) {
				std::cout << "\tAdding Light Component" << std::endl;
				light tempLight = buildLight(fileStream, config);
				getLightManager(componentManagers)->addComponent(entityID);
				getLightManager(componentManagers)->setComponent(entityID, tempLight);
			}
		}
	}
}

//Determines which entity loading function to use and passes through variables
void loadEntity(int entityID, configurationStructure &config, std::vector<ComponentManager*>& componentManagers, std::ifstream &fileStream) {
	if (config.api == Vulkan) {
		loadVulkanEntity(entityID, componentManagers, fileStream, config);
	}
}

//Builder functions for descriptor sets
void buildVulkanDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure &config) {
	//Each pipeline has a descriptor set for the viewpersp+lights descriptor set and all the buffers for cameras and lights (buffers for cameras can be updated by currently rendered cam info)
	//Transform matrices are handled as push constants
	//Texture descriptor sets are stored with the relevant material

	//1 buffer for camera and 1 for lights (PER FRAME)
	int camLightDescCount = 2 * config.swapchainBuffering;

	//Only need as many descriptors as are textures as they can be rebound each draw call
	int imageBuffersNeeded = getVulkanMaterialManager(componentManagers)->getImageCount();

	//1 set per buffering (set includes viewPersp descriptor and lights descriptor) + 1 set per material
	int setsNeeded = getVulkanMaterialManager(componentManagers)->getSize() + config.swapchainBuffering;

	//Call to create a descriptor pool large enough for viewpersp descriptors, light descriptors, and image descriptors
	config.apiInfo.v_Instance->createStaticDescriptorPool(camLightDescCount, imageBuffersNeeded, setsNeeded);

	//Allocates descriptor sets within cameras, the set holds the 
	config.apiInfo.v_Instance->getStaticDescriptorPool()->allocateSets(config.apiInfo.v_Instance->getPipelineDescriptorSetLayout(PBR, LIGHT_CAM_SET),
		*config.apiInfo.v_Instance->getGraphicsPipeline(PBR)->getDescriptorSets(),
		config.swapchainBuffering);

	//Configures the pipelines descriptor sets to use the pipelines buffers split into 2 bindings
	config.apiInfo.v_Instance->getStaticDescriptorPool()->configureVulkanPipelineDescriptorSets(config.apiInfo.v_Instance->getGraphicsPipeline(PBR));

	//Creates and configures descriptor sets for each material
	for (v_material &material : *getVulkanMaterialManager(componentManagers)->getComponents()) {
		//Allocates a descriptor set for material
		config.apiInfo.v_Instance->getStaticDescriptorPool()->allocateTextureSet(config.apiInfo.v_Instance->getPipelineDescriptorSetLayout(PBR, MATERIAL_SET), material);

		//Configure descriptor set for material
		config.apiInfo.v_Instance->getStaticDescriptorPool()->configureTextureSet(material, config.apiInfo.v_Instance->textureSampler);
	}
	std::cout << "Finished configuring descriptor sets" << std::endl;
}
void buildDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure &config) {
	if (config.apiInfo.vulkan) {
		buildVulkanDescriptors(componentManagers, config);
	}
}

//Loads a scene into the component managers and builds necessary resources
void SceneLoader::loadScene(int sceneIndex, configurationStructure & config, std::vector<ComponentManager*>& componentManagers, std::vector<EntitySystem*> &systems)
{
	//Retrieve the scene file
	std::string gameFile = config.gamePath;
	gameFile.append(config.appName);
	gameFile.append(".gf");
	std::ifstream infile(gameFile.c_str());
	std::string line, value;
	int lineIndex = -1;
	while (std::getline(infile, line)) {
		if (sceneIndex == lineIndex) {
			value = getValue(line);
			break;
		}
		lineIndex++;
	}

	config.saveInfo.levelIndex = sceneIndex;

	std::string sceneFile = config.gamePath;
	sceneFile.append(value);

	scene_key key;
	std::ifstream sceneIn(sceneFile.c_str());
	int entityCount = 0;

	while (std::getline(sceneIn, line)) {
		key = getSceneKey(line);
		value = getValue(line);
		if (key == SCENE_NAME) {
			config.saveInfo.levelName = value;
		}
		else if (key == MANAGERS) {
			addManagers(config, componentManagers, sceneIn);
		}
		else if (key == SYSTEMS) {
			addSystems(config, systems, componentManagers, sceneIn);
		}
		else if (key == ADD_PIPELINE) {
			addPipeline(config, value, sceneIn);
		}
		else if (key == ENTITY_COUNT) {
			getTransformManager(componentManagers)->setSize(atoi(value.c_str()));
		}
		else if (key == ENTITY) {
			loadEntity(atoi(value.c_str()), config, componentManagers, sceneIn);
			entityCount++;
		}
		else if (key == END_ENTITIES) {
			break;
		}
	}

	buildDescriptors(componentManagers, config);
	for (EntitySystem* system : systems) {
		fillSystemWithEntities(system, componentManagers, entityCount);
	}

	std::cout << "Finished loading all entites in scene" << std::endl;
}

//Frees loaded memory
void SceneLoader::unloadScene(configurationStructure &config, std::vector<ComponentManager*>& managers)
{
	std::cout << "Unloading Scene" << std::endl;
	getTransformManager(managers)->cleanup();
	if (config.api == Vulkan) {
		std::cout << "\tCleaning vulkan meshes" << std::endl;
		getVulkanMeshManager(managers)->cleanup(config.apiInfo.v_Instance->getPrimaryDevice());
		std::cout << "\tCleaning vulkan materials" << std::endl;
		getVulkanMaterialManager(managers)->cleanup(config.apiInfo.v_Instance->getPrimaryDevice());
		std::cout << "\tCleaning vulkan graphics pipelines" << std::endl;
		for (V_GraphicsPipeline* pipeline : *config.apiInfo.v_Instance->getGraphicsPipelines()) {
			pipeline->cleanup();
		}
	}
}

SceneLoader::~SceneLoader()
{
}
