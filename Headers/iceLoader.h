#include "string.h"
#include "glm\glm.hpp"
#include <vector>
#include <iostream>

//Creates coordinate and indices for an object that does not need to be normal mapped
void loadICE(const char* file_name, std::vector<float> &finalVertices, std::vector<unsigned int> &finalIndices) {
	//Output information
	std::cout << "Loading model at " << file_name << std::endl;

	//Attempt to open file
	FILE* file;
	errno_t err;
	err = fopen_s(&file, file_name, "r");

	if (file == NULL) {
		std::cout << "File open error!" << std::endl;
		return;
	}

	//Temporary variables for lines of the obj
	glm::vec3 tempVec = glm::vec3();
	glm::vec3 tempVecNormal = glm::vec3();
	glm::vec3 tempTangent = glm::vec3();
	glm::vec3 tempBitangent = glm::vec3();
	glm::vec2 tempVecUV = glm::vec2();

	unsigned int i1, i2, i3;

	//Loop through the lines of the obj and store them into vectors
	while (true) {
		//Determine the type of each line
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF) {
			break;
		}
		else {
			if (strcmp(lineHeader, "v") == 0) {
				//Store vertex information into the vertex vector
				fscanf_s(file, "%f %f %f, %f %f %f, %f %f %f, %f %f %f, %f %f\n", &tempVec.x, &tempVec.y, &tempVec.z,
					&tempVecNormal.x, &tempVecNormal.y, &tempVecNormal.z,
					&tempTangent.x, &tempTangent.y, &tempTangent.z,
					&tempBitangent.x, &tempBitangent.y, &tempBitangent.z,
					&tempVecUV.x, &tempVecUV.y,
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float));
				finalVertices.push_back(tempVec.x);
				finalVertices.push_back(tempVec.y);
				finalVertices.push_back(tempVec.z);

				finalVertices.push_back(tempVecNormal.x);
				finalVertices.push_back(tempVecNormal.y);
				finalVertices.push_back(tempVecNormal.z);

				finalVertices.push_back(tempTangent.x);
				finalVertices.push_back(tempTangent.y);
				finalVertices.push_back(tempTangent.z);

				finalVertices.push_back(tempBitangent.x);
				finalVertices.push_back(tempBitangent.y);
				finalVertices.push_back(tempBitangent.z);

				finalVertices.push_back(tempVecUV.x);
				finalVertices.push_back(tempVecUV.y);
			}
			else if (strcmp(lineHeader, "i") == 0) {
				//Store vertex normal information into the vertex vector
				fscanf_s(file, "%u %u %u\n", &i1, &i2, &i3, sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int));
				finalIndices.push_back(i1);
				finalIndices.push_back(i2);
				finalIndices.push_back(i3);
			}
		}
	}
}
GameObject* loadICEasGameObject(const char* file_name) {
	
	GameObject * returnObject = new GameObject();
	returnObject->drawFlag = true;
	GLDrawable * drawable = new GLDrawable();
	GLMaterial * mat = new GLMaterial();
	mat->color = glm::vec3(1.0, 0.2, 1.0);
	mat->setMaterialType(DIALECTRIC);
	
	drawable->ptype = DRAWABLE;
	drawable->renderFlag = true;
	drawable->dtype = MESH;
	drawable->usingEBO = true;
	drawable->material = mat;
	returnObject->glDrawable = drawable;

	//Output information
	std::cout << "Loading model at " << file_name << std::endl;

	//Attempt to open file
	FILE* file;
	errno_t err;
	err = fopen_s(&file, file_name, "r");

	if (file == NULL) {
		std::cout << "File open error!" << std::endl;
		return nullptr;
	}

	//Temporary variables for lines of the obj
	glm::vec3 tempVec = glm::vec3();
	glm::vec3 tempVecNormal = glm::vec3();
	glm::vec3 tempTangent = glm::vec3();
	glm::vec3 tempBitangent = glm::vec3();
	glm::vec2 tempVecUV = glm::vec2();

	unsigned int i1, i2, i3;

	//Loop through the lines of the obj and store them into vectors
	int LOD = -1;
	int totalLevels = 0;
	while (true) {
		//Determine the type of each line
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF) {
			break;
		}
		else {
			if (strcmp(lineHeader, "Levels")==0) {
				fscanf_s(file, "%d\n", &drawable->LODs, sizeof(int));
			}
			else if (strcmp(lineHeader, "LOD")==0) {
				LOD++;
				drawable->indices.push_back(std::vector<unsigned int>());
				drawable->coords.push_back(std::vector<float>());
			}
			else if (strcmp(lineHeader, "v") == 0) {
				//Store vertex information into the vertex vector
				fscanf_s(file, "%f %f %f, %f %f %f, %f %f %f, %f %f %f, %f %f\n", &tempVec.x, &tempVec.y, &tempVec.z,
					&tempVecNormal.x, &tempVecNormal.y, &tempVecNormal.z,
					&tempTangent.x, &tempTangent.y, &tempTangent.z,
					&tempBitangent.x, &tempBitangent.y, &tempBitangent.z,
					&tempVecUV.x, &tempVecUV.y,
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float), sizeof(float),
					sizeof(float), sizeof(float));
				drawable->coords[LOD].push_back(tempVec.x);
				drawable->coords[LOD].push_back(tempVec.y);
				drawable->coords[LOD].push_back(tempVec.z);

				drawable->coords[LOD].push_back(tempVecNormal.x);
				drawable->coords[LOD].push_back(tempVecNormal.y);
				drawable->coords[LOD].push_back(tempVecNormal.z);

				drawable->coords[LOD].push_back(tempTangent.x);
				drawable->coords[LOD].push_back(tempTangent.y);
				drawable->coords[LOD].push_back(tempTangent.z);

				drawable->coords[LOD].push_back(tempBitangent.x);
				drawable->coords[LOD].push_back(tempBitangent.y);
				drawable->coords[LOD].push_back(tempBitangent.z);

				drawable->coords[LOD].push_back(tempVecUV.x);
				drawable->coords[LOD].push_back(tempVecUV.y);
			}
			else if (strcmp(lineHeader, "i") == 0) {
				//Store vertex normal information into the vertex vector
				fscanf_s(file, "%u %u %u\n", &i1, &i2, &i3, sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int));
				drawable->indices[LOD].push_back(i1);
				drawable->indices[LOD].push_back(i2);
				drawable->indices[LOD].push_back(i3);
			}
		}
	}
	return returnObject;
}