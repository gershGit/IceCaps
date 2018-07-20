#include "ObjectFactory.h"
#include "stb_image.h"

ObjectFactory::ObjectFactory()
{
	light_material = new GLMaterial();
	light_material->color = glm::vec3(1, 1, 1);
	light_material->setMaterialType(SIMPLE);

	purple_material = new GLMaterial();
	purple_material->color = glm::vec3(1, 0.2, 1);
	purple_material->setMaterialType(SIMPLE);

	dialectric_standard_material = new GLMaterial();
	dialectric_standard_material->color = glm::vec3(0.65, 0.65, 0.65);
	dialectric_standard_material->setMaterialType(DIALECTRIC);

	mCoordsSpawner = new CoordsSpawner();
}

GameObject* ObjectFactory::createObject(primitive primitive_type) {
	if (primitive_type == CUBE_PRIMITIVE) {
		GLDrawable* squareMesh = new GLDrawable();
		squareMesh->ptype = DRAWABLE;
		squareMesh->renderFlag = true;
		squareMesh->dtype = MESH;
		squareMesh->usingEBO = true;
		squareMesh->indices.push_back(std::vector<unsigned int>());
		squareMesh->coords.push_back(std::vector<float>());
		squareMesh->coords[0] = mCoordsSpawner->squareCoordsOnly;
		squareMesh->indices[0] = mCoordsSpawner->squareIndices;
		squareMesh->material = purple_material;

		GameObject* cube = new GameObject();
		cube->name = "CUBE";
		cube->properties.push_back(squareMesh);
		cube->glDrawable = squareMesh;
		cube->drawFlag = true;
		RigidBody* squareRigid = new RigidBody();
		squareRigid->setStart(cube->pos, glm::vec3(0), glm::vec3(0));
		cube->rigidBody = squareRigid;
		cube->usingRigid = true;
		SphereCollider* squareCollider = new SphereCollider();
		squareCollider->position = cube->pos;
		squareCollider->radius = 1;
		cube->usingCollider = true;
		cube->sCollider = squareCollider;

		return cube;
	}
	else if (primitive_type == SPHERE_PRIMITVE) {
		GLDrawable* sphereMesh = new GLDrawable();
		sphereMesh->ptype = DRAWABLE;
		sphereMesh->renderFlag = true;
		sphereMesh->dtype = MESH;
		sphereMesh->usingEBO = true;
		sphereMesh->indices.push_back(std::vector<unsigned int>());
		sphereMesh->coords.push_back(std::vector<float>());
		sphereMesh->coords[0] = mCoordsSpawner->sphereCoords(18);
		sphereMesh->indices[0] = mCoordsSpawner->sphereIndeces(18);
		sphereMesh->material = purple_material;

		GameObject* sphere = new GameObject();
		sphere->name = "SPHERE";
		sphere->properties.push_back(sphereMesh);
		sphere->glDrawable = sphereMesh;
		sphere->drawFlag = true;

		return sphere;
	}
	return nullptr;
}

GameObject * ObjectFactory::createObject(primitive primitive_type, int id)
{
	if (primitive_type == SPHERE_PRIMITVE) {
		GLDrawable* sphereMesh = new GLDrawable();
		sphereMesh->ptype = DRAWABLE;
		sphereMesh->renderFlag = true;
		sphereMesh->dtype = MESH;
		sphereMesh->usingEBO = true;
		sphereMesh->indices.push_back(std::vector<unsigned int>());
		sphereMesh->coords.push_back(std::vector<float>());
		sphereMesh->coords[0] = mCoordsSpawner->sphereCoords(18);
		sphereMesh->indices[0] = mCoordsSpawner->sphereIndeces(18);
		sphereMesh->material = purple_material;

		GameObject* sphere = new GameObject();
		sphere->name = "SPHERE";
		sphere->properties.push_back(sphereMesh);
		sphere->glDrawable = sphereMesh;
		sphere->drawFlag = true;
		sphere->globalId = id;

		return sphere;
	}
	return nullptr;
}

void AddPoint(int length, float resolution, int xVal, int zVal, unsigned char * image, int width, float maxHeight, std::vector<float> &coordinates)
{
	float x = (length / resolution) * xVal;
	float z = (length / resolution) * zVal;
	float y = (float)image[xVal*width + zVal]/255 * maxHeight;
	float center_l, center_r, center_u, center_d, uX, uY;
	glm::vec3 tangent, bitangent, normal;

	//Do point 2 stuff
	if (xVal == 0) {
		center_l = y/maxHeight;
	}
	else {
		center_l = (float)image[(xVal - 1) * width + zVal] / 255;
	}
	if (xVal == resolution) {
		center_r = y/maxHeight;
	}
	else {
		center_r = (float)image[(xVal + 1) * width + zVal]/255;
	}
	if (zVal == 0) {
		center_u = y/maxHeight;
	}
	else {
		center_u = (float)image[xVal * width + zVal - 1]/255;
	}
	if (zVal == resolution) {
		center_d = y/maxHeight;
	}
	else {
		center_d = (float)image[xVal * width + zVal + 1]/255;
	}


	//TODO fix this
	tangent = glm::vec3(2.0, center_r - center_l, 0.0);
	bitangent = glm::vec3(0.0, center_d - center_u, 2.0);
	float x_diff = center_l - center_r;
	float z_diff = center_d - center_u;
	float y_val = 0.04 - (abs(center_l - center_r) + abs(center_d - center_u));
	normal = glm::normalize(glm::vec3(x_diff, y_val, z_diff));


	uX = xVal / resolution;
	uY = zVal / resolution;

	coordinates.push_back(x);
	coordinates.push_back(y);
	coordinates.push_back(z);

	coordinates.push_back(normal.x);
	coordinates.push_back(normal.y);
	coordinates.push_back(normal.z);

	coordinates.push_back(tangent.x);
	coordinates.push_back(tangent.y);
	coordinates.push_back(tangent.z);

	coordinates.push_back(bitangent.x);
	coordinates.push_back(bitangent.y);
	coordinates.push_back(bitangent.z);

	coordinates.push_back(uX);
	coordinates.push_back(uY);
}

GameObject * ObjectFactory::createTerrain(float length, float maxHeight, float waterlevel, const char * image_path)
{
	int width, height, nrComponents;
	unsigned char *image = stbi_load(image_path, &width, &height, &nrComponents, 0);
	if (image) {
		std::cout << "Heightmap loaded. Generating terrain..." << std::endl;
	}
	else {
		std::cout << "Heightmap failed to load at path: " << image_path << std::endl;
		stbi_image_free(image);
		return nullptr;
	}

	GameObject* terrainObject = new GameObject();
	terrainObject->name = "TERRAIN";
	terrainObject->drawFlag = true;

	GLDrawable* terrainMesh = new GLDrawable();
	terrainMesh->ptype = DRAWABLE;
	terrainMesh->renderFlag = true;
	terrainMesh->dtype = MESH;
	terrainMesh->usingEBO = true;
	std::vector<float> coordinates = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	//TODO optimize
	int indexTracker = 0;
	int xVal, zVal;
	for (int i = 0; i < width-1; i++) {
		for (int j = 0; j < width-1; j++) {

			if (i == 0 && j == 0) {
				AddPoint(length, width, 0, 0, image, width, maxHeight, coordinates);
				AddPoint(length, width, 0, 1, image, width, maxHeight, coordinates);

				//Do Triangle 1 stuff
				indices.push_back(0);
				indices.push_back(width);
				indices.push_back(width+1);

				
				//Do Triangle 2 stuff
				indices.push_back(0);
				indices.push_back(width+1);
				indices.push_back(1);
				indexTracker = 1;
			}
			else if (i == 0) {
				xVal = i;
				zVal = j+1;
				AddPoint(length, width, xVal, zVal, image, width, maxHeight, coordinates);

				indices.push_back(indexTracker);
				indices.push_back(indexTracker + width);
				indices.push_back(indexTracker + width + 1);

				indices.push_back(indexTracker);
				indices.push_back(indexTracker + width + 1);
				indices.push_back(++indexTracker);
				if (j == width - 2) {
					for (int k = 0; k < width; k++) {
						AddPoint(length, width, 1, k, image, width, maxHeight, coordinates);
						indexTracker++;
					}
				}
			}
			else if (j == 0) {
				xVal = i + 1;
				zVal = j;
				AddPoint(length, width, xVal, zVal, image, width, maxHeight, coordinates);

				xVal = i + 1;
				zVal = j + 1;
				AddPoint(length, width, xVal, zVal, image, width, maxHeight, coordinates);

				indices.push_back(++indexTracker - width);
				indices.push_back(indexTracker);
				indices.push_back(++indexTracker);

				indices.push_back(indexTracker - (width+1));
				indices.push_back(indexTracker);
				indices.push_back(indexTracker - width);
			}
			else {
				xVal = i + 1;
				zVal = j + 1;
				AddPoint(length, width, xVal, zVal, image, width, maxHeight, coordinates);

				indices.push_back(indexTracker - width);
				indices.push_back(indexTracker);
				indices.push_back(++indexTracker);

				indices.push_back(indexTracker - (width + 1));
				indices.push_back(indexTracker);
				indices.push_back(indexTracker - width);
			}
		}
	}

	terrainMesh->indices.push_back(std::vector<unsigned int>());
	terrainMesh->coords.push_back(std::vector<float>());
	terrainMesh->coords[0] = coordinates;
	terrainMesh->indices[0] = indices;
	terrainMesh->material = dialectric_standard_material;

	stbi_image_free(image);
	terrainObject->glDrawable = terrainMesh;
	terrainObject->pos = glm::vec3(-length / 2, -waterlevel, -length / 2);
	return terrainObject;
}

GameObject * ObjectFactory::createTerrainSaveMap(float length, float maxHeight, float waterlevel, const char * image_path, HeightMap * map)
{
	int width, height, nrComponents;
	map->image = stbi_load(image_path, &width, &height, &nrComponents, 0);
	if (map->image) {
		std::cout << "Heightmap loaded. Generating terrain..." << std::endl;
	}
	else {
		std::cout << "Heightmap failed to load at path: " << image_path << std::endl;
		stbi_image_free(map->image);
		return nullptr;
	}

	GameObject* terrainObject = new GameObject();
	terrainObject->name = "TERRAIN";
	terrainObject->drawFlag = true;

	GLDrawable* terrainMesh = new GLDrawable();
	terrainMesh->ptype = DRAWABLE;
	terrainMesh->renderFlag = true;
	terrainMesh->dtype = MESH;
	terrainMesh->usingEBO = true;
	std::vector<float> coordinates = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	//TODO optimize
	int indexTracker = 0;
	int xVal, zVal;
	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < width - 1; j++) {

			if (i == 0 && j == 0) {
				AddPoint(length, width, 0, 0, map->image, width, maxHeight, coordinates);
				AddPoint(length, width, 0, 1, map->image, width, maxHeight, coordinates);

				//Do Triangle 1 stuff
				indices.push_back(0);
				indices.push_back(width);
				indices.push_back(width + 1);


				//Do Triangle 2 stuff
				indices.push_back(0);
				indices.push_back(width + 1);
				indices.push_back(1);
				indexTracker = 1;
			}
			else if (i == 0) {
				xVal = i;
				zVal = j + 1;
				AddPoint(length, width, xVal, zVal, map->image, width, maxHeight, coordinates);

				indices.push_back(indexTracker);
				indices.push_back(indexTracker + width);
				indices.push_back(indexTracker + width + 1);

				indices.push_back(indexTracker);
				indices.push_back(indexTracker + width + 1);
				indices.push_back(++indexTracker);
				if (j == width - 2) {
					for (int k = 0; k < width; k++) {
						AddPoint(length, width, 1, k, map->image, width, maxHeight, coordinates);
						indexTracker++;
					}
				}
			}
			else if (j == 0) {
				xVal = i + 1;
				zVal = j;
				AddPoint(length, width, xVal, zVal, map->image, width, maxHeight, coordinates);

				xVal = i + 1;
				zVal = j + 1;
				AddPoint(length, width, xVal, zVal, map->image, width, maxHeight, coordinates);

				indices.push_back(++indexTracker - width);
				indices.push_back(indexTracker);
				indices.push_back(++indexTracker);

				indices.push_back(indexTracker - (width + 1));
				indices.push_back(indexTracker);
				indices.push_back(indexTracker - width);
			}
			else {
				xVal = i + 1;
				zVal = j + 1;
				AddPoint(length, width, xVal, zVal, map->image, width, maxHeight, coordinates);

				indices.push_back(indexTracker - width);
				indices.push_back(indexTracker);
				indices.push_back(++indexTracker);

				indices.push_back(indexTracker - (width + 1));
				indices.push_back(indexTracker);
				indices.push_back(indexTracker - width);
			}
		}
	}

	terrainMesh->indices.push_back(std::vector<unsigned int>());
	terrainMesh->coords.push_back(std::vector<float>());
	terrainMesh->coords[0] = coordinates;
	terrainMesh->indices[0] = indices;
	terrainMesh->material = dialectric_standard_material;

	map->height = height;
	map->maxHeight = maxHeight;
	map->width = width;
	map->waterlevel = waterlevel;

	terrainObject->glDrawable = terrainMesh;
	terrainObject->pos = glm::vec3(-length / 2, -waterlevel, -length / 2);
	return terrainObject;
}

void ObjectFactory::addFoliage(GameObject * foliage_ptr, float x_pos, float z_pos, HeightMap * heightmap)
{
	int xVal = heightmap->width/2 + x_pos;
	int zVal = heightmap->width/2 + z_pos;
	foliage_ptr->pos = glm::vec3(x_pos, ((float)heightmap->image[xVal*heightmap->width + zVal] / 255) * (heightmap->maxHeight) - heightmap->waterlevel - 0.5f, z_pos);
}

GameObject* ObjectFactory::createLight(lightType type_of_light, glm::vec3 position, glm::vec3 color, float strength, bool visible = true) {
	if (type_of_light == POINT_LIGHT) {
		GameObject* cube = new GameObject();
		cube->name = "POINT_LIGHT_";
		cube->pos = position;

		if (visible == true) {
			GLDrawable* squareMesh = new GLDrawable();
			squareMesh->ptype = DRAWABLE;
			squareMesh->renderFlag = true;
			squareMesh->dtype = MESH;
			squareMesh->usingEBO = true;
			squareMesh->indices.push_back(std::vector<unsigned int>());
			squareMesh->coords.push_back(std::vector<float>());
			squareMesh->coords[0] = mCoordsSpawner->squareCoordsOnly;
			squareMesh->indices[0] = mCoordsSpawner->squareIndices;
			squareMesh->material = light_material;

			cube->properties.push_back(squareMesh);
			cube->glDrawable = squareMesh;
			cube->drawFlag = true;
			cube->scale = glm::vec3(.25f, .25f, .25f);
		}

		Light* lightInfo = new Light();
		lightInfo->color = color;
		lightInfo->strength = strength;
		cube->light = lightInfo;

		return cube;
	}
	return nullptr;
}