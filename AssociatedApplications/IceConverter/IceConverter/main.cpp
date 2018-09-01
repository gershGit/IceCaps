#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include "string.h"

class vnIndex {
public:
	unsigned int vertices[3];
	unsigned int normals[3];
	unsigned int texCoords[3];

	vnIndex() {

	}

	void addVertex(unsigned int vertex, int pos) {
		vertices[pos] = vertex;
	}

	void addNormal(unsigned int normal, int pos) {
		normals[pos] = normal;
	}
};
struct fullVertex {
	unsigned int position;
	unsigned int normal;
	unsigned int uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};
unsigned int getIndex(fullVertex checkVertex, std::vector<fullVertex> &tempVertices) {
	for (unsigned int i = 0; i < tempVertices.size(); i++) {
		//If that vertex already exists then return that index
		if (tempVertices[i].position == checkVertex.position && tempVertices[i].uv == checkVertex.uv && tempVertices[i].normal == checkVertex.normal) {
			return i;
		}
	}
	//Add the new vertex and return it's position
	tempVertices.push_back(checkVertex);
	return tempVertices.size() - 1;
}
void createCoordsIndices_UV_NORMAL_MAPPING(const char* file_name) {
	std::vector<float> finalVertices = std::vector<float>();
	std::vector<unsigned int> finalIndices = std::vector<unsigned int>();

	std::cout << "Converting model at " << file_name << " to .ice file" << std::endl;

	std::vector<glm::vec3> vertices_array = std::vector<glm::vec3>();
	std::vector<glm::vec3> normal_array = std::vector<glm::vec3>();
	std::vector<glm::vec2> texture_array = std::vector<glm::vec2>();

	std::vector<fullVertex> tempVertexList = std::vector<fullVertex>();

	FILE* file;
	errno_t err;
	err = fopen_s(&file, file_name, "r");

	if (file == NULL) {
		std::cout << "File open error!" << std::endl;
		return;
	}
	glm::vec3 tempVec = glm::vec3();
	glm::vec2 tempVecUV = glm::vec2();
	std::cout << "Building intial vertices..." << std::endl;
	while (true) {
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF) {
			break;
		}
		else {
			if (strcmp(lineHeader, "v") == 0) {
				fscanf_s(file, "%f %f %f\n", &tempVec.x, &tempVec.y, &tempVec.z, sizeof(float), sizeof(float), sizeof(float));
				vertices_array.push_back(tempVec);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				fscanf_s(file, "%f %f %f\n", &tempVec.x, &tempVec.y, &tempVec.z, sizeof(float), sizeof(float), sizeof(float));
				normal_array.push_back(tempVec);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				fscanf_s(file, "%f %f %f\n", &tempVecUV.x, &tempVecUV.y, sizeof(float), sizeof(float));
				texture_array.push_back(tempVecUV);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				//VERTEX / TEXTURE / NORMAL

				fullVertex vertex1 = fullVertex(), vertex2 = fullVertex(), vertex3 = fullVertex();

				fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertex1.position, &vertex1.uv, &vertex1.normal, &vertex2.position, &vertex2.uv, &vertex2.normal, &vertex3.position, &vertex3.uv, &vertex3.normal,
					sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int), sizeof(unsigned int));

				glm::vec3 tangent1 = glm::vec3();
				glm::vec3 bitangent1 = glm::vec3();

				glm::vec3 pos1 = vertices_array[vertex1.position - 1];
				glm::vec3 pos2 = vertices_array[vertex2.position - 1];
				glm::vec3 pos3 = vertices_array[vertex3.position - 1];

				glm::vec2 uv1 = texture_array[vertex1.uv - 1];
				glm::vec2 uv2 = texture_array[vertex2.uv - 1];
				glm::vec2 uv3 = texture_array[vertex3.uv - 1];

				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y *edge2.x);
				tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangent1 = glm::normalize(tangent1);

				bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
				bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
				bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
				bitangent1 = glm::normalize(bitangent1);

				vertex1.tangent = tangent1;
				vertex2.tangent = tangent1;
				vertex3.tangent = tangent1;

				vertex1.bitangent = bitangent1;
				vertex2.bitangent = bitangent1;
				vertex3.bitangent = bitangent1;

				tempVertexList.push_back(vertex1);
				tempVertexList.push_back(vertex2);
				tempVertexList.push_back(vertex3);
			}
		}
	}

	std::cout << "Building tangents and bitangents..." << std::endl;
	int tenPercent = 0;
	int percentage = 0;
	int lastPercentage = 0;
	//combine tangents and bitangents	
	for (int firstIndex = 0; firstIndex < tempVertexList.size(); firstIndex++) {
		float decimal = ((float)firstIndex / (float)tempVertexList.size()) * 100;
		percentage = (int) decimal;
		if (percentage > 0 && percentage !=lastPercentage) {
			lastPercentage = percentage;
			std::cout << "\t" << percentage << "%" << std::endl;
		}

		std::vector<fullVertex> verticesToCombine = std::vector<fullVertex>();
		verticesToCombine.push_back(tempVertexList[firstIndex]);
		for (int otherIndices = 0; otherIndices < tempVertexList.size(); otherIndices++) {
			if (tempVertexList[otherIndices].position == tempVertexList[firstIndex].position &&
				tempVertexList[otherIndices].normal == tempVertexList[firstIndex].normal &&
				tempVertexList[otherIndices].uv == tempVertexList[firstIndex].uv) {
				verticesToCombine.push_back(tempVertexList[otherIndices]);
			}
		}
		glm::vec3 totalTangent = glm::vec3();
		glm::vec3 totalbiTangent = glm::vec3();
		for (int i = 0; i < verticesToCombine.size(); i++) {
			totalTangent += verticesToCombine[i].tangent;
			totalbiTangent += verticesToCombine[i].bitangent;
		}

		totalTangent.x = totalTangent.x / verticesToCombine.size();
		totalTangent.y = totalTangent.y / verticesToCombine.size();
		totalTangent.z = totalTangent.z / verticesToCombine.size();
		totalTangent = glm::normalize(totalTangent);

		totalbiTangent.x = totalbiTangent.x / verticesToCombine.size();
		totalbiTangent.y = totalbiTangent.y / verticesToCombine.size();
		totalbiTangent.z = totalbiTangent.z / verticesToCombine.size();
		totalbiTangent = glm::normalize(totalbiTangent);

		tempVertexList[firstIndex].tangent = totalTangent;
		tempVertexList[firstIndex].bitangent = totalbiTangent;
	}

	std::cout << "Building index list..." << std::endl;
	//build indices list
	for (unsigned int index = 0; index < tempVertexList.size(); index++) {
		float decimal = ((float)index / (float)tempVertexList.size()) * 100;
		percentage = (int)decimal;
		if (percentage > 0 && percentage != lastPercentage && percentage%10==0) {
			lastPercentage = percentage;
			std::cout << "\t" << percentage << "%" << std::endl;
		}
		for (unsigned int firstIndex = 0; firstIndex < tempVertexList.size(); firstIndex++) {
			if (tempVertexList[index].position == tempVertexList[firstIndex].position &&
				tempVertexList[index].normal == tempVertexList[firstIndex].normal &&
				tempVertexList[index].uv == tempVertexList[firstIndex].uv) {
				finalIndices.push_back(firstIndex);
				break;
			}
		}
	}

	std::cout << "Building final vertex list..." << std::endl;
	//building of vertex list by using index list
	for (int i = 0; i < finalIndices.size(); i++) {
		finalVertices.push_back(vertices_array[tempVertexList[finalIndices[i]].position - 1].x);
		finalVertices.push_back(vertices_array[tempVertexList[finalIndices[i]].position - 1].y);
		finalVertices.push_back(vertices_array[tempVertexList[finalIndices[i]].position - 1].z);

		finalVertices.push_back(normal_array[tempVertexList[finalIndices[i]].normal - 1].x);
		finalVertices.push_back(normal_array[tempVertexList[finalIndices[i]].normal - 1].y);
		finalVertices.push_back(normal_array[tempVertexList[finalIndices[i]].normal - 1].z);

		finalVertices.push_back(tempVertexList[finalIndices[i]].tangent.x);
		finalVertices.push_back(tempVertexList[finalIndices[i]].tangent.y);
		finalVertices.push_back(tempVertexList[finalIndices[i]].tangent.z);

		finalVertices.push_back(tempVertexList[finalIndices[i]].bitangent.x);
		finalVertices.push_back(tempVertexList[finalIndices[i]].bitangent.y);
		finalVertices.push_back(tempVertexList[finalIndices[i]].bitangent.z);

		finalVertices.push_back(texture_array[tempVertexList[finalIndices[i]].uv - 1].x);
		finalVertices.push_back(texture_array[tempVertexList[finalIndices[i]].uv - 1].y);
	}

	int i = 0;
	char file_prefix[128];
	for (i = 0; i < sizeof(file_name) / sizeof(char); i++) {
		if (file_name[i] != '.') {
			file_prefix[i] = file_name[i];
		}
		else {
			file_prefix[i] = '\0';
		}
	}
	file_prefix[i] = '\0';
	
	char finalFileName[128];

	strcpy(finalFileName, file_prefix);
	strcat(finalFileName, ".ice");

	std::cout << "Saving file " << finalFileName << "..." << std::endl;
	FILE* outfile;
	errno_t err_out;
	err_out = fopen_s(&outfile, finalFileName, "w");

	if (outfile == NULL) {
		std::cout << "File open error!" << std::endl;
		return;
	}
	
	fprintf_s(outfile, "%s", "#Vertices\n");
	for (int v = 0; v < finalVertices.size()/14; v++) {
		fprintf(outfile, "%s %f %f %f, %f %f %f, %f %f %f, %f %f %f, %f %f%s", "v", finalVertices[v * 14 + 0], finalVertices[v * 14 + 1], finalVertices[v * 14 + 2],
			finalVertices[v * 14 + 3], finalVertices[v * 14 + 4], finalVertices[v * 14 + 5],
			finalVertices[v * 14 + 6], finalVertices[v * 14 + 7], finalVertices[v * 14 + 8],
			finalVertices[v * 14 + 9], finalVertices[v * 14 + 10], finalVertices[v * 14 + 11],
			finalVertices[v * 14 + 12], finalVertices[v * 14 + 13], "\n");
	}
	fprintf_s(outfile, "%s", "#Indices\n");
	for (int e = 0; e < finalIndices.size()/3; e++) {
		fprintf_s(outfile, "%s %u %u %u%s", "i", finalIndices[e*3+0], finalIndices[e * 3 + 1], finalIndices[e * 3 + 2], "\n");
	}

	std::cout << "Finished" << std::endl;
}

int main() {
	char in_file[128];
	std::cout << "Enter the path of the file you would like to convert (including file name and extension): " << std::endl;
	scanf_s("%s", in_file, sizeof(in_file));

	std::cout << "Checking compatability of file: " << in_file << std::endl;
	for (int i = 0; i < sizeof(in_file) / sizeof(char); i++) {
		if (in_file[i] == '.') {
			if (in_file[i + 1] == 'o' && in_file[i + 2] == 'b' && in_file[i + 3] == 'j') {
				std::cout << "OBJ file supported" << std::endl;
				break;
			}
			else {
				std::cout << "File not supported" << std::endl;
				return 1;
			}
		}
	}

	createCoordsIndices_UV_NORMAL_MAPPING(in_file);
	return 0;
}