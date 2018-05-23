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
void createCoordsIndices_UV_NORMAL_MAPPING(const char* file_name, const char* output_name) {
	std::vector<float> finalVertices = std::vector<float>();
	std::vector<unsigned int> finalIndices = std::vector<unsigned int>();

	std::cout << "Converting model at " << file_name << " to .ice file at " << output_name << std::endl;

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

				finalIndices.push_back(getIndex(vertex1, tempVertexList));
				finalIndices.push_back(getIndex(vertex2, tempVertexList));
				finalIndices.push_back(getIndex(vertex3, tempVertexList));
			}
		}
	}
	for (int i = 0; i < tempVertexList.size(); i++) {
		finalVertices.push_back(vertices_array[tempVertexList[i].position - 1].x);
		finalVertices.push_back(vertices_array[tempVertexList[i].position - 1].y);
		finalVertices.push_back(vertices_array[tempVertexList[i].position - 1].z);

		finalVertices.push_back(normal_array[tempVertexList[i].normal - 1].x);
		finalVertices.push_back(normal_array[tempVertexList[i].normal - 1].y);
		finalVertices.push_back(normal_array[tempVertexList[i].normal - 1].z);
		if (i>0 && (i + 1) % 3 == 0) {
			glm::vec3 v2 = vertices_array[tempVertexList[i].position - 1];
			glm::vec3 v1 = vertices_array[tempVertexList[i].position - 2];
			glm::vec3 v0 = vertices_array[tempVertexList[i].position - 3];

			glm::vec2 uv2 = texture_array[tempVertexList[i].uv - 1];
			glm::vec2 uv1 = texture_array[tempVertexList[i].uv - 2];
			glm::vec2 uv0 = texture_array[tempVertexList[i].uv - 3];

			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;

			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			finalVertices.push_back(tangent.x);
			finalVertices[finalVertices.size() - (1 + 14)] = tangent.x;
			finalVertices[finalVertices.size() - (1 + 28)] = tangent.x;
			finalVertices.push_back(tangent.y);
			finalVertices[finalVertices.size() - (1 + 14)] = tangent.y;
			finalVertices[finalVertices.size() - (1 + 28)] = tangent.y;
			finalVertices.push_back(tangent.z);
			finalVertices[finalVertices.size() - (1 + 14)] = tangent.z;
			finalVertices[finalVertices.size() - (1 + 28)] = tangent.z;

			finalVertices.push_back(bitangent.x);
			finalVertices[finalVertices.size() - (1 + 14)] = bitangent.x;
			finalVertices[finalVertices.size() - (1 + 28)] = bitangent.x;
			finalVertices.push_back(bitangent.y);
			finalVertices[finalVertices.size() - (1 + 14)] = bitangent.y;
			finalVertices[finalVertices.size() - (1 + 28)] = bitangent.y;
			finalVertices.push_back(bitangent.z);
			finalVertices[finalVertices.size() - (1 + 14)] = bitangent.z;
			finalVertices[finalVertices.size() - (1 + 28)] = bitangent.z;
		}
		else {
			finalVertices.push_back(999);
			finalVertices.push_back(999);
			finalVertices.push_back(999);

			finalVertices.push_back(999);
			finalVertices.push_back(999);
			finalVertices.push_back(999);
		}


		finalVertices.push_back(texture_array[tempVertexList[i].uv - 1].x);
		finalVertices.push_back(texture_array[tempVertexList[i].uv - 1].y);
	}

	char file_prefix[128];
	for (int i = 0; i < sizeof(file_name) / sizeof(char); i++) {
		if (file_name[i] != '.') {
			file_prefix[i] = file_name[i];
		}
		else {
			file_prefix[i] = '\0';
		}
	}

	/*
	char finalFileName[256];

	strcpy(finalFileName, output_name);
	strcat(finalFileName, "/");
	strcat(finalFileName, file_prefix);
	strcat(finalFileName, ".ice");
	strcat(finalFileName, "/0");*/

	std::cout << "Saving file..." << std::endl;
	FILE* outfile;
	errno_t err_out;
	err_out = fopen_s(&outfile, "output.ice", "w");

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
	char out_file[128];
	std::cout << "Enter the path of the file you would like to convert (including file name and extension): " << std::endl;
	scanf_s("%s", in_file, sizeof(in_file));
	std::cout << "Enter the path of where you would like to save the ice file: " << std::endl;
	scanf_s("%s", out_file, sizeof(out_file));

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

	createCoordsIndices_UV_NORMAL_MAPPING(in_file, out_file);
	return 0;
}