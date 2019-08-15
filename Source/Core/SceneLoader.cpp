#include "Core/SceneLoader.h"
#include "Vulkan/V_Instance.h"
#include "Vulkan/V_Components.h"
#include "Core/ManagersFactories.h"
#include "Core/SystemsHeader.h"
#include "Core/StringTranslation.h"
#include "Core/MemoryManager.h"
#include "Vulkan/V_BufferHelper.h"
#include <fstream>
#include <numeric>
#include <climits>

char SceneLoader::buffer[BUF_SIZE];
char SceneLoader::value[BUF_SIZE];
char SceneLoader::keyString[BUF_SIZE];
scene_key SceneLoader::key;
char SceneLoader::nT[1];

SceneLoader::SceneLoader()
{
}

//Builder Functions ---------------------------------------------------------------------------

//TODO separate descriptor pool for scene nodes and for material descriptors

//Moves the static entities into arrays as they do not need to be dynamic
void collapseStaticEntities(SceneNode* scene_node, int childCount, std::vector<std::vector<int>*>* staticTemp) {
	//TODO record static render buffers
	scene_node->staticEntityCount = (int) staticTemp->at(scene_node->id)->size();
	if (scene_node->staticEntityCount > 0) {
		scene_node->staticEntities = (int*)malloc(sizeof(int) * scene_node->staticEntityCount);
	}
	for (int i = 0; i < scene_node->staticEntityCount; i++) {
		scene_node->staticEntities[i] = staticTemp->at(scene_node->id)->at(i);
	}
	delete staticTemp->at(scene_node->id);
}

//Places an entity into the master scene tree
bool SceneLoader::insertEntity(SceneNode* scene_node, int entityID, AABB * bounds, bool isDynamic, configurationStructure* config, 
	int pipelineIndex, std::vector<NodeManager<VulkanSceneNode>*> * renderNodes, std::vector<std::vector<int>*>* staticTemp) {
	if (isInside(&scene_node->bounds, bounds)) {
		if (!scene_node->isLeaf) {
			//Place inside children if possible
			for (int i = 0; i < config->sceneChildren; i++) {
				if (insertEntity(&scene_node->children[i], entityID, bounds, isDynamic, config, pipelineIndex, renderNodes, staticTemp)) {
					return true;
				}
			}
		}
		//Place in static or dynamic entities based on type
		if (isDynamic) {
			scene_node->dynamicEntities->push_back(entityID);
		}
		else {
			staticTemp->at(scene_node->id)->push_back(entityID);
		}
		if (pipelineIndex >= 0 && isDynamic) {
			renderNodes->at(pipelineIndex)->getComponentAddress(scene_node->id)->dynamicEntities.push_back(entityID);
		}
		return true;
	}
	return false;
}

//Inserts a light into every node that is affected by the light
bool SceneLoader::insertLight(SceneNode* scene_node, int entityID, LightObject light, float range, configurationStructure* config, int maxLights) {
	if (lightAffects(scene_node->bounds, light, range)) {
		if (!scene_node->isLeaf) {
			//Place inside children if possible
			for (int i = 0; i < config->sceneChildren; i++) {
				insertLight(&scene_node->children[i], entityID, light, range, config, maxLights);
			}
		}	
		scene_node->lightIDs->push_back(entityID);
		if (scene_node->lightCount < maxLights) {
			scene_node->lights[scene_node->lightCount] = light;
			scene_node->lightCount++;
		}
		return true;
	}
	return false;
}

//Allocates the command buffers and light buffers to the vulkan scene
void SceneLoader::allocateVulkanCommandObjects(std::vector<NodeManager<VulkanSceneNode>*>* renderNodes, configurationStructure * config) {
	for (int i = 0; i < renderNodes->size(); i++) {
		for (int j = 0; j < config->sceneNodesCount; j++) {
			VulkanSceneNode nodeInfo;
			nodeInfo.dynamicBuffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * config->swapchainBuffering);
			for (int sc = 0; sc < config->swapchainBuffering; sc++) {
				nodeInfo.dynamicBuffers[sc] = config->apiInfo.v_Instance->getGraphicsCommandPool(i)->getNextCommandBuffer();
			}
			renderNodes->at(i)->addComponent(j, nodeInfo);
		}
	}
}
void SceneLoader::allocateLightBuffers(SceneNode* scene_node, int bufferSize, int childCount) {
	scene_node->lights = (LightObject*) malloc(sizeof(LightObject) * bufferSize);
	scene_node->lightIDs = new std::vector<int>();
	scene_node->lightCount = 0;
	if (!scene_node->isLeaf) {
		for (int i = 0; i < childCount; i++) {
			allocateLightBuffers(&scene_node->children[i], bufferSize, childCount);
		}
	}
}

//Places all lights into the scene
void SceneLoader::placeLightsInScene(SceneNode* scene, MappedManager<light>* lManager, ArrayManager<transform>* tManager, configurationStructure* config, int maxLights) {
	std::vector<int> lightIDs = lManager->getEntities();
	for (int id : lightIDs) {
		insertLight(scene, id, toLightObject(lManager->getComponent(id), tManager->getComponent(id).pos), lManager->getComponent(id).range, config, maxLights);
	}
}

//Recursively allocates children and sets some variables for them
void SceneLoader::createChildren(SceneNode* scene_node, int currentDepth, int maxDepth, int childCount) {
	scene_node->dynamicEntities = new std::vector<int>();
	if (currentDepth == maxDepth) {
		return;
	}
	if (currentDepth < maxDepth) {
		scene_node->isLeaf = false;
		scene_node->children = (SceneNode*)malloc(sizeof(SceneNode) * childCount);
		for (int i = 0; i < childCount; i++) {
			scene_node->children[i].bounds.size = glm::vec3(scene_node->bounds.size.x/2.0f);
			if (childCount == 4) {
				scene_node->children[i].bounds.size.y = std::numeric_limits<float>::max();
			}
			scene_node->children[i].isLeaf = true;
		}
		if (childCount == 4) {
			// ---------
			// | 3 | 4 |
			//  -------
			// | 1 | 2 |
			// ---------
			scene_node->children[0].id = scene_node->id * currentDepth + 1;
			scene_node->children[1].id = scene_node->id * currentDepth + 2;
			scene_node->children[2].id = scene_node->id * currentDepth + 3;
			scene_node->children[3].id = scene_node->id * currentDepth + 4;
			glm::vec3 offset = glm::vec3(scene_node->bounds.size.x/2.0f, 0, scene_node->bounds.size.z/2.0f);
			scene_node->children[0].bounds.pos = scene_node->bounds.pos - offset;
			scene_node->children[1].bounds.pos = scene_node->bounds.pos + glm::vec3(offset.x, 0, -offset.z);
			scene_node->children[2].bounds.pos = scene_node->bounds.pos + glm::vec3(-offset.x, 0, offset.z);
			scene_node->children[3].bounds.pos = scene_node->bounds.pos + offset;
		}
	}
	printNode(scene_node, childCount);
	for (int i = 0; i < childCount; i++) {
		printNode(&scene_node->children[i], childCount);
		createChildren(&scene_node->children[i], currentDepth + 1, maxDepth, childCount);
	}
}

//Recursively creates bounds for all the scene_nodes based on size and position
void SceneLoader::setBounds(SceneNode* scene_node, int childCount) {
	scene_node->bounds.points[0] = scene_node->bounds.pos + glm::vec3(-scene_node->bounds.size.x, -scene_node->bounds.size.y, -scene_node->bounds.size.z);
	scene_node->bounds.points[1] = scene_node->bounds.pos + glm::vec3(-scene_node->bounds.size.x, scene_node->bounds.size.y, -scene_node->bounds.size.z);
	scene_node->bounds.points[2] = scene_node->bounds.pos + glm::vec3(scene_node->bounds.size.x, -scene_node->bounds.size.y, -scene_node->bounds.size.z);
	scene_node->bounds.points[3] = scene_node->bounds.pos + glm::vec3(scene_node->bounds.size.x, scene_node->bounds.size.y, -scene_node->bounds.size.z);
	scene_node->bounds.points[4] = scene_node->bounds.pos + glm::vec3(-scene_node->bounds.size.x, -scene_node->bounds.size.y, scene_node->bounds.size.z);
	scene_node->bounds.points[5] = scene_node->bounds.pos + glm::vec3(-scene_node->bounds.size.x, scene_node->bounds.size.y, scene_node->bounds.size.z);
	scene_node->bounds.points[6] = scene_node->bounds.pos + glm::vec3(scene_node->bounds.size.x, -scene_node->bounds.size.y, scene_node->bounds.size.z);
	scene_node->bounds.points[7] = scene_node->bounds.pos + glm::vec3(scene_node->bounds.size.x, scene_node->bounds.size.y, scene_node->bounds.size.z);
	if (!scene_node->isLeaf) {
		for (int i = 0; i < childCount; i++) {			
			setBounds(&scene_node->children[i], childCount);
		}
	}
}

//Builds the scene tree
void SceneLoader::buildTree(configurationStructure &config, SceneNode * scene, FILE * fp) {
	std::cout << "Creating scene tree" << std::endl;
	int depth = 1;
	while (fscanf(fp, "%s", buffer) > 0) {
		getValue(buffer, value, BUF_SIZE);
		key = getSceneKey(buffer, BUF_SIZE);
		if (key == END_STATEMENT) {
			break;
		}
		else if (key == SCENE_SIZE) {
			scene->bounds.size = glm::vec3(strtof(value, nullptr) / 2.0f);
			scene->bounds.pos = glm::vec3(0);
		}
		else if (key == TREE_DEPTH) {
			depth = atoi(value);
		}
	}
	if (config.sceneChildren == 4) {
		scene->bounds.size.y = std::numeric_limits<float>::max();
	}
	setNodeCount(config, config.sceneChildren, depth);
	createChildren(scene, 1, depth, config.sceneChildren);
	setBounds(scene, config.sceneChildren);
	printSceneAsArea(depth);
}

//Adds managers to hold components for various component types
void SceneLoader::addManagers(configurationStructure &config, std::vector<ComponentManager*> &managers, FILE * fp) {
	std::cout << "Adding Managers" << std::endl;
	while (fscanf(fp, "%s", buffer) > 0) {
		key = getSceneKey(buffer, BUF_SIZE);
		getValue(buffer, value, BUF_SIZE);
		if (key == END_STATEMENT) {
			break;
		}
		else if (key == ADD_MANAGER) {
			component_type cType = getComponentType(value);
			if (cType == TRANSFORM) {
				std::cout << "\tAdding Transform Manager" << std::endl;
				managers.push_back(new ArrayManager<transform>(TRANSFORM));
			}
			else if (cType == PREFAB_COMPONENT) {
				std::cout << "\tAdding Prefab Manager" << std::endl;
				managers.push_back(new MappedManager<prefab>(PREFAB_COMPONENT));
			}
			else if (cType == MATERIAL_COMPONENT) {
				std::cout << "\tAdding Material Manager" << std::endl;
				if (config.api == Vulkan) {
					managers.push_back(new MappedManager<v_material>(V_MATERIAL));
				}
				else if (config.api == openGL) {
					managers.push_back(new MappedManager<gl_material>(GL_MATERIAL));
				}
			}
			else if (cType == MESH_COMPONENT) {
				std::cout << "\tAdding Mesh Manager" << std::endl;
				if (config.api == Vulkan) {
					managers.push_back(new MappedManager<v_mesh>(V_MESH));
				}
			}
			else if (cType == CAMERA) {
				std::cout << "\tAdding Camera Manager" << std::endl;
				if (config.api == Vulkan) {
					managers.push_back(new MappedManager<v_camera>(V_CAMERA));
				}
				else {
					managers.push_back(new MappedManager<camera>(CAMERA));
				}
			}
			else if (cType == LIGHT_COMPONENT) {
				std::cout << "\tAdding Light Manager" << std::endl;
				managers.push_back(new MappedManager<light>(LIGHT_COMPONENT));
			}
			else if (cType == RIGID_BODY) {
				std::cout << "\tAdding Rigid Body Manager" << std::endl;
				managers.push_back(new MappedManager<rigid_body>(RIGID_BODY));
			}
			else if (cType == COLLIDER) {
				std::cout << "\tAdding Collider and Collision Manager" << std::endl;
				managers.push_back(new MappedManager<collider>(COLLIDER));
				managers.push_back(new VectorManager<collision>(COLLISION));
			}
			else if (cType == ANIMATION_COMPONENT) {
				std::cout << "\tAdding Animation Manager" << std::endl;
				managers.push_back(new MappedManager<animation>(ANIMATION_COMPONENT));
			}
			else if (cType == ARMATURE_ANIMATION) {
				std::cout << "\tAdding Armature Animation Manager" << std::endl;
				managers.push_back(new MappedManager<armature_animation>(ARMATURE_ANIMATION));
			}
			else if (cType == ARMATURE_COMPONENT) {
				std::cout << "\tAdding Armature Manager" << std::endl;
				managers.push_back(new MappedManager<armature>(ARMATURE_COMPONENT));
			}
			else if (cType == SKINNED_MESH) {
				std::cout << "\tAdding Skinned Mesh Manager" << std::endl;
				if (config.api == Vulkan) {
					managers.push_back(new MappedManager<vk_skinned_mesh>(V_SKINNED_MESH));
				}
				else {
					managers.push_back(new MappedManager<skinned_mesh>(SKINNED_MESH));
				}
			}
			else if (cType == TAGS_COMPONENT) {
				std::cout << "\tAdding Tags Manager" << std::endl;
				managers.push_back(new ArrayManager<uint64_t>(TAGS_COMPONENT));
			}
			else if (cType == AABB_COMPONENT) {
				std::cout << "\tAdding Bounds Manager" << std::endl;
				managers.push_back(new ArrayManager<AABB>(AABB_COMPONENT));
			}
		}
	}
}

//Fills a system with all necessary managers
void SceneLoader::fillSystemWithManagers(EntitySystem * system, std::vector<ComponentManager*> &managers) {
	for (component_type managerType : system->operatesOn) {
		if (managerType == TRANSFORM) {
			system->managers->push_back(getCManager<transform>(managers, TRANSFORM));
		}
		else if (managerType == PREFAB_COMPONENT) {
			system->managers->push_back(getCManager<prefab>(managers, PREFAB_COMPONENT));
		}
		else if (managerType == CAMERA) {
			system->managers->push_back(getCManager<camera>(managers, CAMERA));
		}
		else if (managerType == V_CAMERA) {
			system->managers->push_back(getCManager<v_camera>(managers, V_CAMERA));
		}
		else if (managerType == LIGHT_COMPONENT) {
			system->managers->push_back(getCManager<light>(managers, LIGHT_COMPONENT));
		}
		else if (managerType == V_MESH) {
			system->managers->push_back(getCManager<v_mesh>(managers, V_MESH));
		}
		else if (managerType == V_MATERIAL) {
			system->managers->push_back(getCManager<v_material>(managers, V_MATERIAL));
		}
		else if (managerType == RIGID_BODY) {
			system->managers->push_back(getCManager<rigid_body>(managers, RIGID_BODY));
		}
		else if (managerType == COLLIDER) {
			system->managers->push_back(getCManager<collider>(managers, COLLIDER));
			system->managers->push_back(getCManager<collision>(managers, COLLISION));
		}
		else if (managerType == ANIMATION_COMPONENT) {
			system->managers->push_back(getCManager<animation>(managers, ANIMATION_COMPONENT));
		}
		else if (managerType == ARMATURE_COMPONENT) {
			system->managers->push_back(getCManager<armature>(managers, ARMATURE_COMPONENT));
		}
		else if (managerType == ARMATURE_ANIMATION) {
			system->managers->push_back(getCManager<armature_animation>(managers, ARMATURE_ANIMATION));
		}
		else if (managerType == SKINNED_MESH) {
			system->managers->push_back(getCManager<armature>(managers, SKINNED_MESH));
		}
		else if (managerType == V_SKINNED_MESH) {
			system->managers->push_back(getCManager<vk_skinned_mesh>(managers, V_SKINNED_MESH));
		}
		else if (managerType == TAGS_COMPONENT) {
			system->managers->push_back(getCManager<uint64_t>(managers, TAGS_COMPONENT));
		}
		else if (managerType == AABB_COMPONENT) {
			system->managers->push_back(getCManager<AABB>(managers, AABB_COMPONENT));
		}
	}
}

//Fills a systems entity list with all matching entities it runs on
void SceneLoader::fillSystemWithEntities(EntitySystem * system, std::vector<ComponentManager*> &managers, int entityCount) {
	if (system->systemType == RENDER_SYSTEM) {
		MappedManager<v_camera>* cManager = dynamic_cast<MappedManager<v_camera>*>(getCManager<v_camera>(managers, V_CAMERA));
		V_RenderSystem* rSystem = dynamic_cast<V_RenderSystem*>(system);
		rSystem->setActiveCamera(cManager->getFirst().first, cManager->getFirst().second);
	}
	for (std::vector<component_type> requiredSet : system->entityListRequiredComponents) {
		for (int entityID = 0; entityID < entityCount; entityID++) {
			bool validEntity = true;
			for (component_type requiredComponent : requiredSet) {
				if (requiredComponent == TRANSFORM) {
					if (!getCManager<transform>(managers, TRANSFORM)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				} 
				else if (requiredComponent == PREFAB_COMPONENT) {
					if (!getCManager<prefab>(managers, PREFAB_COMPONENT)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == V_MESH) {
					if (!getCManager<v_mesh>(managers, V_MESH)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == V_MATERIAL) {
					if (!getCManager<v_material>(managers, V_MATERIAL)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == CAMERA) {
					if (!getCManager<camera>(managers, CAMERA)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == LIGHT_COMPONENT) {
					if (!getCManager<light>(managers, LIGHT_COMPONENT)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == RIGID_BODY) {
					if (!getCManager<rigid_body>(managers, RIGID_BODY)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == COLLIDER) {
					if (!getCManager<collider>(managers, COLLIDER)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == ANIMATION_COMPONENT) {
					if (!getCManager<animation>(managers, ANIMATION_COMPONENT)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == ARMATURE_ANIMATION) {
					if (!getCManager<armature_animation>(managers, ARMATURE_ANIMATION)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == ARMATURE_COMPONENT) {
					if (!getCManager<armature>(managers, ARMATURE_COMPONENT)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == V_SKINNED_MESH) {
					if (!getCManager<vk_skinned_mesh>(managers, V_SKINNED_MESH)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == SKINNED_MESH) {
					if (!getCManager<skinned_mesh>(managers, SKINNED_MESH)->hasEntity(entityID)) {
						validEntity = false;
						break;
					}
				}
				else if (requiredComponent == TAGS_COMPONENT) {
					if (!hasTag(getCManager<uint64_t>(managers, TAGS_COMPONENT)->getComponent(entityID), system->requiredTags)) {
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
void SceneLoader::addSystems(configurationStructure &config, std::vector<EntitySystem*> &systems, std::vector<ComponentManager*> &managers, FILE * fp) {
	std::cout << "Adding Systems" << std::endl;
	while (fscanf(fp, "%s", buffer) > 0) {
		key = getSceneKey(buffer, BUF_SIZE);
		getValue(buffer, value, BUF_SIZE);
		if (key == END_STATEMENT) {
			break;
		}
		else if (key == ADD_SYSTEM) {
			system_type type = getSystemType(value);
			if (type == RENDER_SYSTEM) {
				if (config.api == Vulkan) {
					std::cout << "\tAdding render system" << std::endl;
					V_RenderSystem* renderSystem = new V_RenderSystem();		
					renderSystem->setSwapchain(config.apiInfo.v_Instance->getSwapchain());
					systems.push_back(renderSystem);
				}
			}
			else if (type == RIGID_BODY_SYSTEM) {
				std::cout << "\tAdding rigid body system" << std::endl;
				RigidBodySystem* rigidBodySystem = new RigidBodySystem();
				systems.push_back(rigidBodySystem);
			}
			else if (type == COLLISION_SYSTEM) {
				std::cout << "\tAdding collision system" << std::endl;
				CollisionSystem* collisionSystem = new CollisionSystem();
				systems.push_back(collisionSystem);
			}
			else if (type == ANIMATION_SYSTEM) {
				std::cout << "\tAdding animation system" << std::endl;
				AnimationSystem* animationSystem = new AnimationSystem();
				systems.push_back(animationSystem);
			}
			else if (type == ARMATURE_SYSTEM) {
				std::cout << "\tAdding armature system" << std::endl;
				ArmatureSystem* armSystem = new ArmatureSystem();
				armSystem->setVulkan(config.api == Vulkan);
				systems.push_back(armSystem);
			}
			else if (type == INPUT_SYSTEM) {
				std::cout << "\tAdding input system" << std::endl;
				InputSystem* inputSystem = new InputSystem();
				inputSystem->setEntityComponents(managers, { {TAGS_COMPONENT} }, ICE_TAG_INPUT);
				systems.push_back(inputSystem);
			}
			else if (type == SCENE_TREE_SYSTEM) {
				std::cout << "\tAdding scene tree system" << std::endl;
				SceneTreeSystem* treeSys = new SceneTreeSystem();
				if (config.api == Vulkan) {
					treeSys->usingVulkan = true;

				}
				systems.push_back(treeSys);
			}
			else if (type == BOUNDS_SYSTEM) {
				std::cout << "\tAdding bounds system" << std::endl;
				BoundsSystem* bSys = new BoundsSystem();
				systems.push_back(bSys);
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
void SceneLoader::addVulkanPipeline(configurationStructure &config, char * value, FILE * fp, std::vector<NodeManager<VulkanSceneNode>*> * renderNodes) {
	int max_lights = 0;
	material_type mType = stringToMaterialType(value);
	while (fscanf(fp, "%s", buffer) > 0) {
		getSubComponent(buffer, keyString, BUF_SIZE);
		getValue(buffer, value, BUF_SIZE);
		if (strcmp(keyString, "MAX_LIGHTS") == 0) {
			max_lights = atoi(value);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	config.apiInfo.v_Instance->addGraphicsPipeline(mType, max_lights);
	renderNodes->push_back(new NodeManager<VulkanSceneNode>(VULKAN_SCENE_NODE_COMPONENT, mType));
}

//Adds a pipeline pased on the type of shaders it will use
void SceneLoader::addPipeline(configurationStructure &config, char * value, FILE * fp) {
	material_type mType = stringToMaterialType(value);
}

//Creates a transform component from a filestream until reaching END keyword
transform SceneLoader::buildTransformComponent(FILE * fp) {
	transform retTransform = transform();
	int valSize;
	retTransform.scale = glm::vec3(1);
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "POSITION") == 0) {
			retTransform.pos = getVectorFromString<glm::vec3>(value, valSize);
		}
		else if (strcmp(keyString, "ROTATION") == 0) {
			retTransform.rot = getVectorFromString<glm::vec3>(value, valSize);
		}
		else if (strcmp(keyString, "SCALE") == 0) {
			retTransform.scale = getVectorFromString<glm::vec3>(value, valSize);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retTransform;
}

//Creates a vulkan mesh component with the help of a V_MeshFactory
v_mesh SceneLoader::buildVulkanMeshComponent(FILE * fp, configurationStructure &config, AABB * bounds) {
	v_mesh retMesh = v_mesh();
	char meshFile[BUF_SIZE * 2];
	int valSize;
	memcpy(meshFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "FILE_LOAD") == 0) {
			memcpy(&value[valSize], nT, sizeof(char));
			memcpy(&meshFile[config.gamePath.length()], value, (valSize + 1) * sizeof(char));
			V_MeshFactory::loadFromFile(meshFile, retMesh, config, bounds);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retMesh;
}

//Creates a vulkan material component with the help of a V_MaterialFactory
v_material SceneLoader::buildVulkanMaterialComponent(FILE * fp, configurationStructure &config) {
	v_material retMaterial = v_material();
	char matFile[BUF_SIZE * 2];
	int valSize;
	memcpy(matFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "FILE_LOAD") == 0) {
			memcpy(&value[valSize], nT, sizeof(char));
			memcpy(&matFile[config.gamePath.length()], value, (valSize+1) * sizeof(char));
			V_MaterialFactory::loadMaterialFromFile(matFile, retMaterial, config);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retMaterial;
}

//Creates a vulkan camera from the file stream and config information
camera SceneLoader::buildCamera(FILE * fp, configurationStructure &config) {
	camera retCamera = camera();
	int valSize;
	while (fscanf(fp, "%s", buffer)>0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "FOV") == 0) {
			retCamera.fov = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "NEAR") == 0) {
			retCamera.near = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "FAR") == 0) {
			retCamera.far = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "LOOK") == 0) {
			retCamera.lookDirection = glm::normalize(getVectorFromString<glm::vec3>(value, valSize));
		}
		else if (strcmp(keyString, "UP") == 0) {
			retCamera.upDirection = glm::normalize(getVectorFromString<glm::vec3>(value, valSize));
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retCamera;
}
v_camera SceneLoader::buildVulkanCamera(FILE * fp, configurationStructure &config) {
	v_camera retCamera = v_camera();
	int valSize;
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "FOV") == 0) {
			retCamera.fov = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "NEAR") == 0) {
			retCamera.near = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "FAR") == 0) {
			retCamera.far = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "LOOK") == 0) {
			retCamera.lookDirection = glm::normalize(getVectorFromString<glm::vec3>(value, valSize));
		}
		else if (strcmp(keyString, "UP") == 0) {
			retCamera.upDirection = glm::normalize(getVectorFromString<glm::vec3>(value, valSize));
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retCamera;
}

//Creates a light component from a file stream
light SceneLoader::buildLight(FILE * fp, configurationStructure &config) {
	light retLight = light();
	int valSize;
	while (fscanf(fp, "%s", buffer)>0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "LIGHT_TYPE") == 0) {
			retLight.lType = getLightTypeFromString(value);
		}
		else if (strcmp(keyString, "COLOR") == 0) {
			retLight.color = getVectorFromString<glm::vec4>(value, valSize);
		}
		else if (strcmp(keyString, "RANGE") == 0) {
			retLight.range = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retLight;
}

//Creates a rigid body component from a file stream
rigid_body SceneLoader::buildRigidBody(FILE * fp, configurationStructure &config) {
	rigid_body retBody = rigid_body();
	int valSize;
	retBody.lastVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "MASS") == 0) {
			retBody.mass = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "ELASTICITY") == 0) {
			retBody.elasticity = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "START_VELOCITY") == 0) {
			retBody.lastVelocity = getVectorFromString<glm::vec3>(value, valSize);
		}
		else if (strcmp(keyString, "STATIC") == 0) {
			retBody.isStatic = getBool(value);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retBody;
}

//Creates a collider from a file stream
collider SceneLoader::buildCollider(FILE * fp, configurationStructure &config) {
	collider retCollider = collider();
	while (fscanf(fp, "%s", buffer)) {
		getSubComponent(buffer, keyString, BUF_SIZE);
		getValue(buffer, value, BUF_SIZE);
		if (strcmp(keyString, "TYPE") == 0) {
			retCollider.type = getColliderType(value);
		}
		else if (strcmp(keyString, "RADIUS") == 0) {
			retCollider.radius = strtof(value, nullptr);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retCollider;
}

//Creates an animation using an animation factory
animation SceneLoader::buildAnimation(FILE * fp, configurationStructure &config) {
	animation retAnim = animation();
	char animFile[BUF_SIZE*2];
	memcpy(animFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	int valueCount = 0;
	while (fscanf(fp, "%s", buffer)) {
		key = getSceneKey(buffer, BUF_SIZE);
		valueCount = getValue(buffer, value, BUF_SIZE);
		if (key == FILE_LOAD_KEY) {
			memcpy(&value[valueCount], nT, sizeof(char));
			memcpy(&animFile[config.gamePath.length()], value, (valueCount+1) * sizeof(char));
			AnimationFactory::loadFromFile(animFile, retAnim, config);
		}
		else if (key == END_STATEMENT) {
			break;
		}
	}
	return retAnim;
}

//Creates an armature from a glac file
armature SceneLoader::buildArmature(FILE* fp, configurationStructure& config) {
	armature retArm = armature();
	char armFile[BUF_SIZE * 2];
	memcpy(armFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	int valueCount = 0;
	while (fscanf(fp, "%s", buffer)) {
		key = getSceneKey(buffer, BUF_SIZE);
		valueCount = getValue(buffer, value, BUF_SIZE);
		if (key == FILE_LOAD_KEY) {
			memcpy(&value[valueCount], nT, sizeof(char));
			memcpy(&armFile[config.gamePath.length()], value, (valueCount + 1) * sizeof(char));
			ArmatureFactory::loadFromFile(armFile, retArm, config);
		}
		else if (key == END_STATEMENT) {
			break;
		}
	}
	return retArm;
}

//Creates an animation for an armature from an spl file
armature_animation SceneLoader::buildArmatureAnimation(FILE* fp, configurationStructure& config) {
	armature_animation retAnim = armature_animation();
	char armFile[BUF_SIZE * 2];
	memcpy(armFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	int valueCount = 0;
	while (fscanf(fp, "%s", buffer)) {
		key = getSceneKey(buffer, BUF_SIZE);
		valueCount = getValue(buffer, value, BUF_SIZE);
		if (key == FILE_LOAD_KEY) {
			memcpy(&value[valueCount], nT, sizeof(char));
			memcpy(&armFile[config.gamePath.length()], value, (valueCount + 1) * sizeof(char));
			AnimationFactory::loadArmatureAnimationFromFile(armFile, retAnim, config);
		}
		else if (key == END_STATEMENT) {
			break;
		}
	}
	return retAnim;
}

//Creates a skinned mesh for vulkan cpu skinning and rendering
vk_skinned_mesh SceneLoader::buildVulkanSkinnedMesh(FILE* fp, configurationStructure& config, AABB* bounds) {
	vk_skinned_mesh retMesh = vk_skinned_mesh();
	char meshFile[BUF_SIZE * 2];
	int valSize;
	memcpy(meshFile, config.gamePath.c_str(), sizeof(char) * config.gamePath.length());
	while (fscanf(fp, "%s", buffer) > 0) {
		valSize = getValue(buffer, value, BUF_SIZE);
		getSubComponent(buffer, keyString, BUF_SIZE);
		if (strcmp(keyString, "FILE_LOAD") == 0) {
			memcpy(&value[valSize], nT, sizeof(char));
			memcpy(&meshFile[config.gamePath.length()], value, (valSize + 1) * sizeof(char));
			V_MeshFactory::loadSkinnedMeshFromFile(meshFile, retMesh, config, bounds, false);
		}
		else if (strcmp(keyString, "END") == 0) {
			break;
		}
	}
	return retMesh;
}

//Loads in an entity under the vulkan api
void SceneLoader::loadVulkanEntity(int entityID, std::vector<ComponentManager*>& componentManagers, FILE * fp, configurationStructure &config, 
	SceneNode* scene, std::vector<NodeManager<VulkanSceneNode>*> * renderNodes, std::vector<std::vector<int>*> * staticTemp) {
	std::cout << "Adding entity: " << entityID << std::endl;
	bool isDynamic = false, inScene = false, renderable = false;
	AABB bounds = AABB();
	scene_key key;
	while (fscanf(fp, "%s", buffer) > 0) {
		key = getSceneKey(buffer, BUF_SIZE);
		getValue(buffer, value, BUF_SIZE);
		if (key == END_STATEMENT) {
			break;
		}
		else if (key == ADD_COMPONENT) {
			component_type cType = getComponentType(value);
			if (cType == TRANSFORM) {
				std::cout << "\tAdding Transform Component" << std::endl;
				getCManager<transform>(componentManagers, TRANSFORM)->addComponent(entityID, buildTransformComponent(fp));
			}
			else if (cType == MESH_COMPONENT) {
				std::cout << "\tAdding Mesh Component" << std::endl;
				getCManager<v_mesh>(componentManagers, V_MESH)->addComponent(entityID, buildVulkanMeshComponent(fp, config, &bounds));
				inScene = true;
				renderable = true;
			}
			else if (cType == SKINNED_MESH) {
				std::cout << "\tAdding Skinned Mesh Component" << std::endl;
				getCManager<vk_skinned_mesh>(componentManagers, V_SKINNED_MESH)->addComponent(entityID, buildVulkanSkinnedMesh(fp, config, &bounds));
				inScene = true;
				renderable = true;
			}
			else if (cType == MATERIAL_COMPONENT) {
				getCManager<v_material>(componentManagers, V_MATERIAL)->addComponent(entityID, buildVulkanMaterialComponent(fp, config));
			}
			else if (cType == PREFAB_COMPONENT) {
				std::cout << "\tAdding Prefab Component" << std::endl;
				prefab tempPrefab = prefab();
				fscanf(fp, "%s", buffer);
				getValue(buffer, value, BUF_SIZE);
				tempPrefab.baseEntity = atoi(value);
				getCManager<prefab>(componentManagers, PREFAB_COMPONENT)->addComponent(entityID, tempPrefab);
			}
			else if (cType == CAMERA) {
				std::cout << "\tAdding Camera Component" << std::endl;
				getCManager<v_camera>(componentManagers, V_CAMERA)->addComponent(entityID, buildVulkanCamera(fp, config));
			}
			else if (cType == LIGHT_COMPONENT) {
				std::cout << "\tAdding Light Component" << std::endl;
				getCManager<light>(componentManagers, LIGHT_COMPONENT)->addComponent(entityID, buildLight(fp, config));
				inScene = true;
			}
			else if (cType == RIGID_BODY) {
				std::cout << "\tAdding Rigid Body Component" << std::endl;
				rigid_body tempRigid = buildRigidBody(fp, config);
				tempRigid.lastPosition = getCManager<transform>(componentManagers, TRANSFORM)->getComponent(entityID).pos;
				tempRigid.lastRotation = getCManager<transform>(componentManagers, TRANSFORM)->getComponent(entityID).rot;
				getCManager<rigid_body>(componentManagers, RIGID_BODY)->addComponent(entityID, tempRigid);
			}
			else if (cType == COLLIDER) {
				std::cout << "\tAdding Collider Component" << std::endl;
				getCManager<collider>(componentManagers, COLLIDER)->addComponent(entityID, buildCollider(fp, config));
				inScene = true;
			}
			else if (cType == ANIMATION_COMPONENT) {
				std::cout << "\tAdding Animation Component" << std::endl;
				getCManager<animation>(componentManagers, ANIMATION_COMPONENT)->addComponent(entityID, buildAnimation(fp, config));
			}
			else if (cType == ARMATURE_ANIMATION) {
				std::cout << "\tAdding Armature Animation Component" << std::endl;
				getCManager<armature_animation>(componentManagers, ARMATURE_ANIMATION)->addComponent(entityID, buildArmatureAnimation(fp, config));
			}
			else if (cType == ARMATURE_COMPONENT) {
				std::cout << "\tAdding Armature Component" << std::endl;
				getCManager<armature>(componentManagers, ARMATURE_COMPONENT)->addComponent(entityID, buildArmature(fp, config));
			}
		}
		else if (key == ADD_TAG) {
			if (strcmp(value, "ICE_TAG_INPUT") == 0) {
				std::cout << "\tAdding input tag" << std::endl;
				uint64_t new_tag = addTag<uint64_t>(getCManager<uint64_t>(componentManagers, TAGS_COMPONENT)->getComponent(entityID), ICE_TAG_INPUT);
				getCManager<uint64_t>(componentManagers, TAGS_COMPONENT)->setComponent(entityID, new_tag);
			}
		}
		else if (key == IS_DYNAMIC) {
			isDynamic = getBool(value);
		}
	}

	if (getCManager<collider>(componentManagers, COLLIDER)->hasEntity(entityID)) {
		collider col = getCManager<collider>(componentManagers, COLLIDER)->getComponent(entityID);
		AABB colBounds = getBounds(col, getCManager<transform>(componentManagers, TRANSFORM)->getComponent(entityID).pos);
		AABB meshBounds = getMeshBounds(&bounds, getCManager<transform>(componentManagers, TRANSFORM)->getComponent(entityID).pos);
		getCManager<AABB>(componentManagers, AABB_COMPONENT)->setComponent(entityID, getMaxBounds(&meshBounds, &colBounds));
	}
	else if (getCManager<vk_skinned_mesh>(componentManagers, V_SKINNED_MESH)->hasEntity(entityID)) {
		getCManager<AABB>(componentManagers, AABB_COMPONENT)->setComponent(entityID, getMeshBounds(&bounds, getCManager<transform>(componentManagers, TRANSFORM)->getComponent(entityID).pos));
	}
	if (inScene) {
		int pipelineIndex = -2;
		if (renderable) {
			material_type mType = getCManager<v_material>(componentManagers, V_MATERIAL)->getComponent(entityID).matType;
			pipelineIndex = getPipelineIndex(renderNodes, mType);
			if (pipelineIndex == -1) {
				std::cout << "ERROR LOADING SCENE. CORRUPT FILE!" << std::endl;
			}
		}
		insertEntity(scene, entityID, getCManager<AABB>(componentManagers, AABB_COMPONENT)->getComponentAddress(entityID), isDynamic, &config, 0, renderNodes, staticTemp);
	}
}

//Determines which entity loading function to use and passes through variables
void SceneLoader::loadEntity(int entityID, configurationStructure &config, std::vector<ComponentManager*>& componentManagers, FILE * fp, SceneNode* scene) {
	if (config.api == Vulkan) {
		//loadVulkanEntity(entityID, componentManagers, fp, config, scene, renderNodes);
	}
}

//Builder functions for descriptor sets
void SceneLoader::buildVulkanDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure &config, std::vector<NodeManager<VulkanSceneNode>*> * renderNodes) {
	//Each pipeline has a descriptor set for the viewpersp+lights descriptor set and all the buffers for cameras and lights (buffers for cameras can be updated by currently rendered cam info)
	//Transform matrices are handled as push constants
	//Texture descriptor sets are stored with the relevant material

	//Number of total scene scene_nodes that could have a descriptor
	//Some pipelines may use a different number of lights than others so they will need their own descriptos
	int renderNodeCount = config.sceneNodesCount * (int) renderNodes->size();

	//One light descriptor per frame per scene_node (per pipeline)
	int lightDescriptorsCount = config.swapchainBuffering * renderNodeCount;

	//One camera descriptor per frame per camera
	int camDescriptors = config.swapchainBuffering * getCManager<v_camera>(componentManagers, V_CAMERA)->getSize();

	//One descriptor for each of the images used for textures
	int imageBuffersNeeded = countImages(dynamic_cast<MappedManager<v_material>*>(getCManager<v_material>(componentManagers, V_MATERIAL)));

	//1 set per material + 1 set per renderNode for each frame in flight
	int setsNeeded = getCManager<v_material>(componentManagers, V_MATERIAL)->getSize() + lightDescriptorsCount;

	//Call to create a descriptor pool large enough for viewpersp descriptors, light descriptors, and image descriptors
	//Allow the scene to double in size by allocating twice the number of descriptors at initialization. Further growth requires new pools
	config.apiInfo.v_Instance->createStaticDescriptorPool((camDescriptors + lightDescriptorsCount)*2, imageBuffersNeeded*2, setsNeeded*2);

	//Loop over each render map and allocate the set for each scene_node
	for (int i = 0; i < renderNodes->size(); i++) {
		config.apiInfo.v_Instance->getStaticDescriptorPool()->allocateSetsInMap(
		config.apiInfo.v_Instance->getGraphicsPipelines()->at(i)->getDescriptorSetLayout(LIGHT_CAM_SET),
			renderNodes->at(i),
			config.swapchainBuffering
			);
	}

	//Allocate camera buffers
	CManager<v_camera>* camManager = getCManager<v_camera>(componentManagers, V_CAMERA);
	std::vector<int> camIDs = camManager->getEntities();
	VkDeviceSize camBufSize = sizeof(ViewPersp);
	for (int i = 0; i < camIDs.size(); i++) {
		v_camera* cam = camManager->getComponentAddress(camIDs.at(i));
		cam->camBuffers = (VkBuffer*)malloc(sizeof(VkBuffer) * config.swapchainBuffering);
		cam->camVRAM = (VkDeviceMemory*)malloc(sizeof(VkBuffer) * config.swapchainBuffering);
		for (int j = 0; j < config.swapchainBuffering; j++) {
			//Creates a host visible buffer for updating			
			V_BufferHelper::createBuffer(config.apiInfo.v_Instance->getPrimaryDevice(), camBufSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				cam->camBuffers[j], cam->camVRAM[j]);
		}
	}

	//Allocate the buffers for the scene_nodes
	for (int i = 0; i < renderNodes->size(); i++) {
		VkDeviceSize lightsBufferSize = sizeof(LightObject) * config.apiInfo.v_Instance->getGraphicsPipelines()->at(i)->max_lights;
		for (int j = 0; j < renderNodes->at(i)->getEntities().size(); j++) {
			VulkanSceneNode* scene_node = renderNodes->at(i)->getComponentAddress(renderNodes->at(i)->getEntities().at(j));
			scene_node->lightBuffers = (VkBuffer*)malloc(sizeof(VkBuffer) * config.swapchainBuffering);
			scene_node->lightBufferVRAM = (VkDeviceMemory*)malloc(sizeof(VkBuffer) * config.swapchainBuffering);
			scene_node->lightIDs = (int*)malloc(sizeof(int) * config.apiInfo.v_Instance->getGraphicsPipelines()->at(i)->max_lights);
			scene_node->lightMax = config.apiInfo.v_Instance->getGraphicsPipelines()->at(i)->max_lights;
			for (int k = 0; k < config.swapchainBuffering; k++) {
				//Creates a host visible buffer for updating			
				V_BufferHelper::createBuffer(config.apiInfo.v_Instance->getPrimaryDevice(), lightsBufferSize,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					scene_node->lightBuffers[k], scene_node->lightBufferVRAM[k]);
			}
		}
	}

	//Create camera descriptor infos to pass so we don't need extra descriptors per camera
	v_camera* cam = camManager->getComponentAddress(camIDs.at(0));
	VkDescriptorBufferInfo* camDescInfos = (VkDescriptorBufferInfo*)malloc(sizeof(VkDescriptorBufferInfo) * config.swapchainBuffering);
	for (int i = 0; i < config.swapchainBuffering; i++) {
		VkDescriptorBufferInfo camInfo = {};
		camInfo.buffer = cam->camBuffers[i];
		camInfo.offset = 0;
		camInfo.range = sizeof(ViewPersp);
		camDescInfos[i] = camInfo;
	}

	//Configures the pipelines descriptor sets to use the pipelines buffers split into 2 bindings
	for (int i = 0; i < renderNodes->size(); i++) {
		config.apiInfo.v_Instance->getStaticDescriptorPool()->configureNodeSets(
			renderNodes->at(i),
			camDescInfos,
			config.apiInfo.v_Instance->getGraphicsPipelines()->at(i)->max_lights,
			config.swapchainBuffering
		);
	}
	free(camDescInfos);

	//Creates and configures descriptor sets for each material
	MappedManager<v_material>* matManager = dynamic_cast<MappedManager<v_material>*>(getCManager<v_material>(componentManagers, V_MATERIAL));
	for (int i = 0; i < matManager->getEntities().size(); i++){
		v_material* mat = matManager->getComponentAddress(matManager->getEntities().at(i));
		config.apiInfo.v_Instance->getStaticDescriptorPool()->allocateTextureSet(config.apiInfo.v_Instance->getPipelineDescriptorSetLayout(mat->matType, MATERIAL_SET), mat);

		config.apiInfo.v_Instance->getStaticDescriptorPool()->configureTextureSet(mat, config.apiInfo.v_Instance->textureSampler);
	}
	std::cout << "Finished configuring descriptor sets" << std::endl;
}
void SceneLoader::buildDescriptors(std::vector<ComponentManager*>& componentManagers, configurationStructure &config) {
	if (config.apiInfo.vulkan) {
		//buildVulkanDescriptors(componentManagers, config);
	}
}

//Loads a scene into the component managers and builds necessary resources
void SceneLoader::loadScene(int sceneIndex, configurationStructure & config, std::vector<ComponentManager*>& componentManagers, std::vector<EntitySystem*> &systems, SceneNode* scene)
{
	//Retrieve the scene file
	int valueSize = 0;
	char gameString[512];
	nT[0] = '\0';

	memcpy(gameString, config.gamePath.c_str(), config.gamePath.length() * sizeof(char));
	memcpy(&gameString[config.gamePath.length()], config.appName.c_str(), config.appName.length() * sizeof(char));
	memcpy(&gameString[config.gamePath.length() + config.appName.length()], ".gf", 3 * sizeof(char));
	memcpy(&gameString[config.gamePath.length() + config.appName.length() + 3], nT, sizeof(char));

	FILE *gameFile;
	int lineIndex = -1;
	gameFile = fopen(gameString, "r");
	while (fscanf(gameFile, "%s", buffer) == 1) {
		if (sceneIndex = lineIndex) {
			valueSize = getValue(buffer, value, BUF_SIZE);
			lineIndex++;
		}
	}
	fclose(gameFile);

	std::vector<NodeManager<VulkanSceneNode>*> * renderNodes = new std::vector<NodeManager<VulkanSceneNode>*>();
	std::vector<std::vector<int>*>* staticTemp = new std::vector<std::vector<int>*>();
	if (config.api != Vulkan) {
		delete renderNodes;
	}

	config.saveInfo.levelIndex = sceneIndex;

	memcpy(&gameString[config.gamePath.length()], value, valueSize * sizeof(char));
	gameFile = fopen(gameString, "r");

	int entityCount = 0;
	while (fscanf(gameFile, "%s", buffer) == 1) {
		key = getSceneKey(buffer, BUF_SIZE);
		valueSize = getValue(buffer, value, BUF_SIZE);
		if (key == SCENE_NAME) {
			config.saveInfo.levelName = std::string(value);
		}
		else if (key == MANAGERS) {
			addManagers(config, componentManagers, gameFile);
		}
		else if (key == SYSTEMS) {
			addSystems(config, systems, componentManagers, gameFile);
		}
		else if (key == SCENE_STRUCTURE) {
			scene->id = 0;
			config.sceneChildren = getChildCount(value);
			buildTree(config, scene, gameFile);
			for (int i = 0; i < config.sceneNodesCount; i++) {
				staticTemp->push_back(new std::vector<int>());
			}
			if (config.api == Vulkan) {
				int renderNodeCount = config.sceneNodesCount * (int)renderNodes->size();
				config.apiInfo.v_Instance->createCommandPools(config.cpu_info->coreCount, renderNodeCount);
				allocateVulkanCommandObjects(renderNodes, &config);
			}
		}
		else if (key == ADD_PIPELINE) {
			if (config.api == Vulkan) {
				addVulkanPipeline(config, value, gameFile, renderNodes);
			}
			else {
				addPipeline(config, value, gameFile);
			}
		}
		else if (key == ENTITY_COUNT) {
			dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(componentManagers, TRANSFORM))->setSize(atoi(value));
			dynamic_cast<ArrayManager<uint64_t>*>(getCManager<uint64_t>(componentManagers, TAGS_COMPONENT))->setSize(atoi(value));
			dynamic_cast<ArrayManager<AABB>*>(getCManager<AABB>(componentManagers, AABB_COMPONENT))->setSize(atoi(value));
		}
		else if (key == ENTITY) {
			if (config.api == Vulkan) {
				loadVulkanEntity(atoi(value), componentManagers, gameFile, config, scene, renderNodes, staticTemp);
			}
			entityCount++;
		}
		else if (key == END_ENTITIES) {
			break;
		}
	}
	fclose(gameFile);

	if (config.api == Vulkan) {
		buildVulkanDescriptors(componentManagers, config, renderNodes);
	}
	for (EntitySystem* system : systems) {
		fillSystemWithEntities(system, componentManagers, entityCount);
		if (system->usesScene) {
			system->setScene(scene);
		}
		if (system->systemType == RENDER_SYSTEM) {
			dynamic_cast<V_RenderSystem*>(system)->setRenderNodes(renderNodes);
		}
		if (system->systemType == SCENE_TREE_SYSTEM) {
			dynamic_cast<SceneTreeSystem*>(system)->setRenderNodes(renderNodes);
		}
	}

	collapseStaticEntities(scene, config.sceneChildren, staticTemp);
	delete staticTemp;

	allocateLightBuffers(scene, getMaxLights(config.apiInfo.v_Instance->getGraphicsPipelines()), config.sceneChildren);
	placeLightsInScene(scene, dynamic_cast<MappedManager<light>*>(getCManager<light>(componentManagers, LIGHT_COMPONENT)), 
		dynamic_cast<ArrayManager<transform>*>(getCManager<transform>(componentManagers, TRANSFORM)), &config, 
		getMaxLights(config.apiInfo.v_Instance->getGraphicsPipelines()));

	printSceneTreeWithEntities(scene, config.sceneChildren, 0, 1);
	if (locateEntity(scene, 0, config.sceneChildren) == false) {
		std::cout << "Couldn't find entity 0!" << std::endl;
	}

	std::cout << "Finished loading all entites in scene" << std::endl;
}

//Frees loaded memory
void SceneLoader::unloadScene(configurationStructure &config, std::vector<ComponentManager*>& managers, std::vector<EntitySystem*> &systems)
{
	std::cout << "Unloading Scene" << std::endl;
	managerCleanup(dynamic_cast<ArrayManager<transform>*>( getCManager<transform>(managers,TRANSFORM) ));
	if (config.api == Vulkan) {
		std::cout << "\tCleaning vulkan meshes" << std::endl;
		managerCleanup(dynamic_cast<MappedManager<v_mesh>*>( getCManager<v_mesh>(managers,V_MESH)), config.apiInfo.v_Instance->getPrimaryDevice());
		managerCleanup(dynamic_cast<MappedManager<vk_skinned_mesh>*>(getCManager<vk_skinned_mesh>(managers, V_SKINNED_MESH)), config.apiInfo.v_Instance->getPrimaryDevice());
		std::cout << "\tCleaning vulkan materials" << std::endl;
		managerCleanup(dynamic_cast<MappedManager<v_material>*>(getCManager<v_material>(managers, V_MATERIAL)), config.apiInfo.v_Instance->getPrimaryDevice());
		std::cout << "\tCleaning vulkan cameras" << std::endl;
		managerCleanup(dynamic_cast<MappedManager<v_camera>*>(getCManager<v_camera>(managers, V_CAMERA)), config.apiInfo.v_Instance->getPrimaryDevice(), config.swapchainBuffering);
		std::cout << "\tCleaning vulkan graphics pipelines" << std::endl;
		for (V_GraphicsPipeline* pipeline : *config.apiInfo.v_Instance->getGraphicsPipelines()) {
			pipeline->cleanup();
		}
	}

	//Delete the actual managers
	for (ComponentManager* man : managers) {
		delete man;
	}

	//Delete the actual systems
	for (EntitySystem* sys : systems) {
		sys->cleanup();
		delete sys;
	}
}

SceneLoader::~SceneLoader()
{
}