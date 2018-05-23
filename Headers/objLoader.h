//
// Created by garret on 3/31/18.
//

#ifndef GLFW_EXAMPLE_BADOBJLOADER_H
#define GLFW_EXAMPLE_BADOBJLOADER_H

#include "Drawable.h"
#include "string.h"

class vnIndex {
public:
    unsigned int vertices[3];
    unsigned int normals[3];
	unsigned int texCoords[3];

    vnIndex(){

    }

    void addVertex(unsigned int vertex, int pos){
        vertices[pos] = vertex;
    }

    void addNormal(unsigned int normal, int pos){
        normals[pos] = normal;
    }
};
struct fullVertex {
	unsigned int position;
	unsigned int normal;
	unsigned int uv;
};
unsigned int getIndex(fullVertex checkVertex, std::vector<fullVertex> &tempVertices) {
	for (unsigned int i=0; i < tempVertices.size(); i++) {
		//If that vertex already exists then return that index
		if (tempVertices[i].position == checkVertex.position && tempVertices[i].uv == checkVertex.uv && tempVertices[i].normal == checkVertex.normal) {
			return i;
		}
	}
	//Add the new vertex and return it's position
	tempVertices.push_back(checkVertex);
	return tempVertices.size() - 1;
}

std::vector<float> createCoordVector_NORMAL(const char* file_name){
    std::vector<float> vertices_array= std::vector<float>();
    std::vector<float> normal_array= std::vector<float>();
    std::vector<vnIndex> indices_array = std::vector<vnIndex>();

	FILE* file;
	errno_t err;
	err = fopen_s(&file, file_name, "r");

    if (file == NULL){
        std::cout << "File open error!" << std::endl;
        return std::vector<float>();
    }
    while(true) {
		char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
        if (res == EOF){
            break;
        } else {
            float x,y,z;
            if (strcmp(lineHeader, "v") == 0) {
				fscanf_s(file, "%f %f %f\n", &x, &y, &z, sizeof(x), sizeof(y), sizeof(z));
                vertices_array.push_back(x);
                vertices_array.push_back(y);
                vertices_array.push_back(z);
            } else if (strcmp(lineHeader, "vn") == 0) {
				fscanf_s(file, "%f %f %f\n", &x, &y, &z, sizeof(x), sizeof(y), sizeof(z));
                normal_array.push_back(x);
                normal_array.push_back(y);
                normal_array.push_back(z);
            } else if (strcmp(lineHeader, "f") == 0){
                vnIndex myIndices = vnIndex();
				fscanf_s(file, "%u//%u %u//%u %u//%u\n", &myIndices.vertices[0], &myIndices.normals[0], &myIndices.vertices[1], &myIndices.normals[1], &myIndices.vertices[2], &myIndices.normals[2], sizeof(myIndices.vertices[0]), sizeof(myIndices.normals[0]), sizeof(myIndices.vertices[1]), sizeof(myIndices.normals[1]), sizeof(myIndices.vertices[2]), sizeof(myIndices.normals[2]));
                indices_array.push_back(myIndices);
            }
        }
    }
    std::vector<float> coords = std::vector<float>();
    for (vnIndex index : indices_array){
        coords.push_back(vertices_array[3*(index.vertices[0]-1)]);
        coords.push_back(vertices_array[3*(index.vertices[0]-1)+1]);
        coords.push_back(vertices_array[3*(index.vertices[0]-1)+2]);
        coords.push_back(1);
        coords.push_back(0);
        coords.push_back(1);
        coords.push_back(normal_array[3*(index.normals[0]-1)]);
        coords.push_back(normal_array[3*(index.normals[0]-1)+1]);
        coords.push_back(normal_array[3*(index.normals[0]-1)+2]);

        coords.push_back(vertices_array[3*(index.vertices[1]-1)]);
        coords.push_back(vertices_array[3*(index.vertices[1]-1)+1]);
        coords.push_back(vertices_array[3*(index.vertices[1]-1)+2]);
        coords.push_back(1);
        coords.push_back(0);
        coords.push_back(1);
        coords.push_back(normal_array[3*(index.normals[1]-1)]);
        coords.push_back(normal_array[3*(index.normals[1]-1)+1]);
        coords.push_back(normal_array[3*(index.normals[1]-1)+2]);

        coords.push_back(vertices_array[3*(index.vertices[2]-1)]);
        coords.push_back(vertices_array[3*(index.vertices[2]-1)+1]);
        coords.push_back(vertices_array[3*(index.vertices[2]-1)+2]);
        coords.push_back(1);
        coords.push_back(0);
        coords.push_back(1);
        coords.push_back(normal_array[3*(index.normals[2]-1)]);
        coords.push_back(normal_array[3*(index.normals[2]-1)+1]);
        coords.push_back(normal_array[3*(index.normals[2]-1)+2]);
    }

    return coords;
}

void createCoordsIndices_UV_NORMAL(const char* file_name, std::vector<float> &finalVertices, std::vector<unsigned int> &finalIndices){
	std::cout << "Loading model at " << file_name << std::endl;

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
		finalVertices.push_back(vertices_array[tempVertexList[i].position-1].x);
		finalVertices.push_back(vertices_array[tempVertexList[i].position-1].y);
		finalVertices.push_back(vertices_array[tempVertexList[i].position-1].z);

		finalVertices.push_back(normal_array[tempVertexList[i].normal-1].x);
		finalVertices.push_back(normal_array[tempVertexList[i].normal-1].y);
		finalVertices.push_back(normal_array[tempVertexList[i].normal-1].z);

		finalVertices.push_back(texture_array[tempVertexList[i].uv-1].x);
		finalVertices.push_back(texture_array[tempVertexList[i].uv-1].y);
	}
}


#endif //GLFW_EXAMPLE_BADOBJLOADER_H