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

    vnIndex(){

    }

    void addVertex(unsigned int vertex, int pos){
        vertices[pos] = vertex;
    }

    void addNormal(unsigned int normal, int pos){
        normals[pos] = normal;
    }
};

std::vector<float> createCoordVector(const char* file_name){
    std::vector<float> vertices_array= std::vector<float>();
    std::vector<float> normal_array= std::vector<float>();
    std::vector<vnIndex> indices_array = std::vector<vnIndex>();

	FILE* file;
	errno_t err;
	err = fopen_s(&file, file_name, "r");

	std::cout << "Opening file\n\t-->";
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

#endif //GLFW_EXAMPLE_BADOBJLOADER_H