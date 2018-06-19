/*
	A header only file containing the coordinate information for various shapes and their related indices
*/

#define _USE_MATH_DEFINES
#pragma once
#include <vector>
#include <math.h>
#include <cmath>

class CoordsSpawner {
public:
	//Coordinates for a colored cube
	std::vector<float> squareCoords = { //Front face
										-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, //Front left top
										-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, //Front left bottom
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, //Front left top
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, //Front right top


										//Back face
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, //Back left bottom
										1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, //Back right bottom
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, //Back right bottom
										1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, //Back right top 


										//Left face
										-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, //Front left top
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, //Front left bottom
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, //Front left bottom
										-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, //Back left bottom

										//Right face
										1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, //Front right top
										1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, //Back right top
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, //Back right top
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, //Back right bottom */


										//Top face
										-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, //Front left top
										1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, //Front right top
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, //Front right top
										-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //Back left top
										1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, //Back right top*/

										//Bottom face
										-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, //Front left bottom
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, //Back left bottom
										1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, //Front right bottom
										-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, //Back left bottom
										1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, //Back right bottom*/
	};

	//Coordinates for a cube with color and normals
	std::vector<float> squareCoordsNormal = {
		//Front face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		0.0, 0.0, -1.0,	//Front left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		0.0, 0.0, -1.0,	//Front left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f,		0.0, 0.0, -1.0,	//Front right bottom
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		0.0, 0.0, -1.0,	//Front left top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f,		0.0, 0.0, -1.0,	//Front right bottom
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f,		0.0, 0.0, -1.0,	//Front right top


		//Back face
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		0.0, 0.0, 1.0, //Back left top
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f,		0.0, 0.0, 1.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0, //Back right bottom
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		0.0, 0.0, 1.0, //Back left top
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0, //Back right bottom
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		0.0, 0.0, 1.0, //Back right top 


		//Left face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		-1.0, 0.0, 0.0, //Front left top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 0.0, 0.0, //Back left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		-1.0, 0.0, 0.0, //Front left bottom
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 0.0, 0.0, //Back left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		-1.0, 0.0, 0.0, //Front left bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f,		-1.0, 0.0, 0.0, //Back left bottom

		//Right face
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		1.0, 0.0, 0.0, //Front right top
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		1.0, 0.0, 0.0, //Back right top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, 0.0, 0.0, //Front right bottom
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		1.0, 0.0, 0.0, //Back right top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, 0.0, 0.0, //Front right bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 		1.0, 0.0, 0.0, //Back right bottom 


		//Top face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 		0.0, 1.0, 0.0, //Front left top
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		0.0, 1.0, 0.0, //Front right top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 		0.0, 1.0, 0.0, //Back left top
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		0.0, 1.0, 0.0, //Front right top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 		0.0, 1.0, 0.0, //Back left top
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		0.0, 1.0, 0.0, //Back right top*/

		//Bottom face
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, 		0.0, -1.0, 0.0, //Front left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		0.0, -1.0, 0.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 		0.0, -1.0, 0.0, //Back left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		0.0, -1.0, 0.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 		0.0, -1.0, 0.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 		0.0, -1.0, 0.0, //Back right bottom*/
	};

	//Coordinates for a cube with color and uv coordinates
	std::vector<float> squareCoordsColorUV = {
		//Front face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		-1.0, 1.0,	//Front left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		-1.0, -1.0,	//Front left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f,		1.0, -1.0,	//Front right bottom
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		-1.0, 1.0,	//Front left top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f,		1.0, -1.0,	//Front right bottom
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f,		1.0, 1.0,	//Front right top


		//Back face
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f,		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0, -1.0, //Back right bottom
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 1.0, //Back left top
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		-1.0, -1.0, //Back right bottom
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0, 1.0, //Back right top 


		//Left face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		1.0, 1.0, //Front left top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		1.0, -1.0, //Front left bottom
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		1.0, -1.0, //Front left bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f,		-1.0, -1.0, //Back left bottom

		//Right face
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		1.0, 1.0, //Front right top
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		-1.0, 1.0, //Back right top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, -1.0, //Front right bottom
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		-1.0, 1.0, //Back right top
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, -1.0, //Front right bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 		-1.0, -1.0, //Back right bottom 


		//Top face
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 		1.0, -1.0, //Front left top
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		1.0, 1.0, //Front right top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 		-1.0, -1.0, //Back left top
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 		1.0, 1.0, //Front right top
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 		-1.0, -1.0, //Back left top
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 		-1.0, 1.0, //Back right top*/

		//Bottom face
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f, 		1.0, -1.0, //Front left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, 1.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 		1.0, 1.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 		-1.0, 1.0, //Back right bottom*/
	};

	//Coordinates for a cube with uv coordinates
	std::vector<float> squareCoordsUV = {
		//Front face
		-1.0f, 1.0f, -1.0f,		-1.0, 1.0,	//Front left top
		-1.0f, -1.0f, -1.0f,	-1.0, -1.0,	//Front left bottom
		1.0f, -1.0f, -1.0f,		1.0, -1.0,	//Front right bottom
		-1.0f, 1.0f, -1.0f,		-1.0, 1.0,	//Front left top
		1.0f, -1.0f, -1.0f,		1.0, -1.0,	//Front right bottom
		1.0f, 1.0f, -1.0f,		1.0, 1.0,	//Front right top


		//Back face
		-1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		1.0, -1.0, //Back right bottom
		-1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back left top
		1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back right bottom
		1.0f, 1.0f, 1.0f,		1.0, 1.0, //Back right top 


		//Left face
		-1.0f, 1.0f, -1.0f,		1.0, 1.0, //Front left top
		-1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, -1.0f,	1.0, -1.0, //Front left bottom
		-1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back left top
		-1.0f, -1.0f, -1.0f,	1.0, -1.0, //Front left bottom
		-1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back left bottom

		//Right face
		1.0f, 1.0f, -1.0f,		1.0, 1.0, //Front right top
		1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back right top
		1.0f, -1.0f, -1.0f,		1.0, -1.0, //Front right bottom
		1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back right top
		1.0f, -1.0f, -1.0f,		1.0, -1.0, //Front right bottom
		1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back right bottom 


		//Top face
		-1.0f, 1.0f, -1.0f,		1.0, -1.0, //Front left top
		1.0f, 1.0f, -1.0f,		1.0, 1.0, //Front right top
		-1.0f, 1.0f, 1.0f,		-1.0, -1.0, //Back left top
		1.0f, 1.0f, -1.0f,		1.0, 1.0, //Front right top
		-1.0f, 1.0f, 1.0f,		-1.0, -1.0, //Back left top
		1.0f, 1.0f, 1.0f,		-1.0, 1.0, //Back right top*/

		//Bottom face
		-1.0f, -1.0f, -1.0f,	1.0, -1.0, //Front left bottom
		1.0f, -1.0f, -1.0f,		1.0, 1.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, -1.0f,		1.0, 1.0, //Front right bottom
		-1.0f, -1.0f, 1.0f,		-1.0, -1.0, //Back left bottom
		1.0f, -1.0f, 1.0f,		-1.0, 1.0, //Back right bottom*/
	};

	//Coordinates for a cube with normals and uv coordinates
	std::vector<float> squareCoordsNormalUV = {
		//Front face
		-1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		0.0, 1.0,	//Front left top
		-1.0f, -1.0f, -1.0f,	0.0, 0.0, -1.0,		0.0, 0.0, //Front left bottom
		1.0f, -1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 0.0,	//Front right bottom
		-1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		0.0, 1.0,	//Front left top
		1.0f, -1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 0.0,	//Front right bottom
		1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 1.0,	//Front right top


		//Back face
		-1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 1.0,	//Back left top
		-1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 0.0,	//Back left bottom
		1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		1.0, 0.0,	//Back right bottom
		-1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 1.0,	//Back left top
		1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		1.0, 0.0,	//Back right bottom
		1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		1.0, 1.0,	//Back right top 


		//Left face
		-1.0f, 1.0f, -1.0f,		-1.0, 0.0, 0.0,		1.0, 1.0,	//Front left top
		-1.0f, 1.0f, 1.0f,		-1.0, 0.0, 0.0,		0.0, 1.0,	//Back left top
		-1.0f, -1.0f, -1.0f,	-1.0, 0.0, 0.0,		1.0, 0.0,	//Front left bottom
		-1.0f, 1.0f, 1.0f,		-1.0, 0.0, 0.0,		0.0, 1.0,	//Back left top
		-1.0f, -1.0f, -1.0f,	-1.0, 0.0, 0.0,		1.0, 0.0,	//Front left bottom
		-1.0f, -1.0f, 1.0f,		-1.0, 0.0, 0.0,		0.0, 0.0, //Back left bottom

		//Right face
		1.0f, 1.0f, -1.0f,		1.0, 0.0, 0.0,		1.0, 1.0,	//Front right top
		1.0f, 1.0f, 1.0f,		1.0, 0.0, 0.0,		0.0, 1.0,	//Back right top
		1.0f, -1.0f, -1.0f,		1.0, 0.0, 0.0,		1.0, 0.0,	//Front right bottom
		1.0f, 1.0f, 1.0f,		1.0, 0.0, 0.0,		0.0, 1.0,	//Back right top
		1.0f, -1.0f, -1.0f,		1.0, 0.0, 0.0,		1.0, 0.0,	//Front right bottom
		1.0f, -1.0f, 1.0f,		1.0, 0.0, 0.0,		0.0, 0.0,	//Back right bottom 


		//Top face
		-1.0f, 1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front left top
		1.0f, 1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right top
		-1.0f, 1.0f, 1.0f,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back left top
		1.0f, 1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right top
		-1.0f, 1.0f, 1.0f,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back left top
		1.0f, 1.0f, 1.0f,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back right top*/

		//Bottom face
		-1.0f, -1.0f, -1.0f,	0.0, -1.0, 0.0,		1.0, 0.0,	//Front left bottom
		1.0f, -1.0f, -1.0f,		0.0, -1.0, 0.0,		1.0, 1.0,	//Front right bottom
		-1.0f, -1.0f, 1.0f,		0.0, -1.0, 0.0,		0.0, 0.0,	//Back left bottom
		1.0f, -1.0f, -1.0f,		0.0, -1.0, 0.0,		1.0, 1.0,	//Front right bottom
		-1.0f, -1.0f, 1.0f,		0.0, -1.0, 0.0,		0.0, 0.0,	//Back left bottom
		1.0f, -1.0f, 1.0f,		0.0, -1.0, 0.0,		0.0, 1.0,	//Back right bottom*/
	};

	//Coordinates for a plane with colors
	std::vector<float> planeCoords = {
		//Single face
		-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,
	};


	//----------------------------------------------Above this line is deprecated ---------------------------------------------------------------------
	//Coordinates for a plane to be indexed
	std::vector<float> planeCoordsOnly = {
		-1.0f, 0.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 0.0, -1.0,		0.0, 0.0,
		1.0f, 0.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 0.0, -1.0,		1.0, 0.0,
		-1.0f, 0.0f, 1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 0.0, -1.0,		0.0, 1.0,
		1.0f, 0.0f, 1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 0.0, -1.0,		1.0, 1.0,
	};

	//Coordinates for a plane to be indexed (lacks tangents and bitangents)
	std::vector<float> planeCoordsNoNMOnly = {
		-1.0f, 0.0f, -1.0f,		0.0, 1.0, 0.0,		0.0, 0.0,
		1.0f, 0.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0,
		-1.0f, 0.0f, 1.0f,		0.0, 1.0, 0.0,		0.0, 1.0,
		1.0f, 0.0f, 1.0f,		0.0, 1.0, 0.0,		1.0, 1.0,
	};
	//Indices for a plane
	std::vector<unsigned int> planeIndeces = {
		0, 2, 1,
		2, 3, 1,
	};


	//Coordinates for a cube to be indexed
	std::vector<float> squareCoordsOnly = {
		//Front face			Normal				Tangent				Bitangent			UV
		-1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Front left top		0
		-1.0f, -1.0f, -1.0f,	0.0, 0.0, -1.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Front left bottom		1
		1.0f, -1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front right bottom	2
		1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right top		3

		//Back face
		-1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		-1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Back left top			4
		-1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		-1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Back left bottom		5
		1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		-1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back right bottom		6
		1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		-1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back right top 		7	

		//Left face
		-1.0f, 1.0f, -1.0f,		-1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front left top		8
		-1.0f, 1.0f, 1.0f,		-1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back left top			9
		-1.0f, -1.0f, -1.0f,	-1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front left bottom		10
		-1.0f, -1.0f, 1.0f,		-1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back left bottom		11

		//Right face
		1.0f, 1.0f, -1.0f,		1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right top		12
		1.0f, 1.0f, 1.0f,		1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back right top		13
		1.0f, -1.0f, -1.0f,		1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front right bottom	14
		1.0f, -1.0f, 1.0f,		1.0, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back right bottom		15

		//Top face
		-1.0f, 1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front left top		16
		1.0f, 1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right top		17
		-1.0f, 1.0f, 1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back left top			18
		1.0f, 1.0f, 1.0f,		0.0, 1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back right top*/		19

		//Bottom face
		-1.0f, -1.0f, -1.0f,	0.0, -1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 0.0,	//Front left bottom		20
		1.0f, -1.0f, -1.0f,		0.0, -1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		1.0, 1.0,	//Front right bottom	21
		-1.0f, -1.0f, 1.0f,		0.0, -1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 0.0,	//Back left bottom		22
		1.0f, -1.0f, 1.0f,		0.0, -1.0, 0.0,		1.0, 0.0, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,	//Back right bottom*/	23
	};
	//Indices for a cube
	std::vector<unsigned int> squareIndices = {
		//Front
		1, 0, 2,
		0, 3, 2,

		//Back
		4, 5, 7,
		6, 7, 5,

		//Right
		12, 13, 14,
		14, 13, 15,

		//Left
		9, 8, 10,
		10, 11, 9,

		//Top
		16, 18, 17,
		18, 19, 17,

		//Bottom
		22, 20, 21,
		22, 21, 23,
	};

	//Sphere helper functions
	double x(float r, float phi, float theta) {
		return r * cos(theta)*sin(phi);
	}
	double y(float r, float phi, float theta) {
		return r * sin(theta)*sin(phi);
	}
	double z(float r, float phi, float theta) {
		return r * cos(phi);
	}
	//Coordinates for a sphere to be indexed
	std::vector<float> sphereCoords(int n) {
		std::vector<float> coords = std::vector<float>();
		int radius = 1;
		int n_steps = (n % 2 == 0) ? n : n + 1;
		double step_size = 2 * M_PI / n_steps;

		for (int i = 0; i < n_steps / 2.0; ++i) {
			for (int j = 0; j < n_steps; ++j) {
				double phi_i = i * step_size;
				double phi_ip1 = ((i + 1) % n_steps)*step_size;
				double theta_j = j * step_size;
				double theta_jp1 = ((j + 1) % n_steps)*step_size;

				// vertex i,j
				double vij_x = x(radius, phi_i, theta_j);
				double vij_y = z(radius, phi_i, theta_j);
				double vij_z = y(radius, phi_i, theta_j);
				glm::vec3 nij = glm::normalize(glm::vec3(vij_x, vij_y, vij_z));
				glm::vec3 uv_d0 = glm::normalize(-glm::vec3(vij_x, vij_y, vij_z));
				glm::vec2 uv_0 = glm::vec2(atan2(uv_d0.x, uv_d0.z) / (2 * M_PI) + 0.5, uv_d0.y * 0.5 + 0.5);

				// vertex i+1,j
				double vip1j_x = x(radius, phi_ip1, theta_j);
				double vip1j_y = z(radius, phi_ip1, theta_j);
				double vip1j_z = y(radius, phi_ip1, theta_j);
				glm::vec3 nip1j(vip1j_x, vip1j_y, vip1j_z);
				glm::vec3 uv_d1 = glm::normalize(-glm::vec3(vip1j_x, vip1j_y, vip1j_z));
				glm::vec2 uv_1 = glm::vec2(atan2(uv_d1.x, uv_d1.z) / (2 * M_PI) + 0.5, uv_d1.y * 0.5 + 0.5);

				// vertex i,j+1
				double vijp1_x = x(radius, phi_i, theta_jp1);
				double vijp1_y = z(radius, phi_i, theta_jp1);
				double vijp1_z = y(radius, phi_i, theta_jp1);
				glm::vec3 nijp1 = glm::normalize(glm::vec3(vijp1_x, vijp1_y, vijp1_z));
				glm::vec3 uv_d2 = glm::normalize(-glm::vec3(vijp1_x, vijp1_y, vijp1_z));
				glm::vec2 uv_2 = glm::vec2(atan2(uv_d2.x, uv_d2.z) / (2 * M_PI) + 0.5, uv_d2.y * 0.5 + 0.5);

				// vertex i+1,j+1
				double vip1jp1_x = x(radius, phi_ip1, theta_jp1);
				double vip1jp1_y = z(radius, phi_ip1, theta_jp1);
				double vip1jp1_z = y(radius, phi_ip1, theta_jp1);
				glm::vec3 nip1jp1 = glm::normalize(glm::vec3(vip1jp1_x, vip1jp1_y, vip1jp1_z));
				glm::vec3 uv_d3 = glm::normalize(-glm::vec3(vip1jp1_x, vip1jp1_y, vip1jp1_z));
				glm::vec2 uv_3 = glm::vec2(atan2(uv_d3.x, uv_d3.z) / (2 * M_PI) + 0.5, uv_d3.y * 0.5 + 0.5);

				//Calculate tangents
				glm::vec3 v2 = glm::vec3(vijp1_x, vijp1_y, vijp1_z);
				glm::vec3 v1 = glm::vec3(vip1j_x, vip1j_y, vip1j_z);
				glm::vec3 v0 = glm::vec3(vij_x, vij_y, vij_z);

				//TODO fix texture coords
				glm::vec3 d2 = glm::normalize(-v2);
				glm::vec3 d1 = glm::normalize(-v1);
				glm::vec3 d0 = glm::normalize(-v0);

				glm::vec2 uv2 = glm::vec2(atan2(d2.x, d2.z) / (2 * M_PI) + 0.5, d2.y * 0.5 + 0.5);
				glm::vec2 uv1 = glm::vec2(atan2(d1.x, d1.z) / (2 * M_PI) + 0.5, d1.y * 0.5 + 0.5);
				glm::vec2 uv0 = glm::vec2(atan2(d0.x, d0.z) / (2 * M_PI) + 0.5, d0.y * 0.5 + 0.5);

				glm::vec3 deltaPos1 = v1 - v0;
				glm::vec3 deltaPos2 = v2 - v0;

				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

				// add triangle
				coords.push_back(v0.x);
				coords.push_back(v0.y);
				coords.push_back(v0.z);
				coords.push_back(nij.x);
				coords.push_back(nij.y);
				coords.push_back(nij.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv0.x);
				coords.push_back(uv0.y);

				coords.push_back(v1.x);
				coords.push_back(v1.y);
				coords.push_back(v1.z);
				coords.push_back(nip1j.x);
				coords.push_back(nip1j.y);
				coords.push_back(nip1j.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv1.x);
				coords.push_back(uv1.y);

				coords.push_back(v2.x);
				coords.push_back(v2.y);
				coords.push_back(v2.z);
				coords.push_back(nijp1.x);
				coords.push_back(nijp1.y);
				coords.push_back(nijp1.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv2.x);
				coords.push_back(uv2.y);

				v2 = glm::vec3(vip1j_x, vip1j_y, vip1j_z);
				v1 = glm::vec3(vip1jp1_x, vip1jp1_y, vip1jp1_z);
				v0 = glm::vec3(vijp1_x, vijp1_y, vijp1_z);

				//TODO fix texture coords
				d2 = glm::normalize(-v2);
				d1 = glm::normalize(-v1);
				d0 = glm::normalize(-v0);

				uv2 = glm::vec2(atan2(d2.x, d2.z) / (2 * M_PI) + 0.5, d2.y * 0.5 + 0.5);
				uv1 = glm::vec2(atan2(d1.x, d1.z) / (2 * M_PI) + 0.5, d1.y * 0.5 + 0.5);
				uv0 = glm::vec2(atan2(d0.x, d0.z) / (2 * M_PI) + 0.5, d0.y * 0.5 + 0.5);

				deltaPos1 = v1 - v0;
				deltaPos2 = v2 - v0;

				deltaUV1 = uv1 - uv0;
				deltaUV2 = uv2 - uv0;

				r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;


				// add triange
				coords.push_back(v0.x);
				coords.push_back(v0.y);
				coords.push_back(v0.z);
				coords.push_back(nijp1.x);
				coords.push_back(nijp1.y);
				coords.push_back(nijp1.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv0.x);
				coords.push_back(uv0.y);

				coords.push_back(v1.x);
				coords.push_back(v1.y);
				coords.push_back(v1.z);
				coords.push_back(nip1jp1.x);
				coords.push_back(nip1jp1.y);
				coords.push_back(nip1jp1.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv1.x);
				coords.push_back(uv1.y);

				coords.push_back(v2.x);
				coords.push_back(v2.y);
				coords.push_back(v2.z);
				coords.push_back(nip1j.x);
				coords.push_back(nip1j.y);
				coords.push_back(nip1j.z);
				coords.push_back(tangent.x);
				coords.push_back(tangent.y);
				coords.push_back(tangent.z);
				coords.push_back(bitangent.x);
				coords.push_back(bitangent.y);
				coords.push_back(bitangent.z);
				coords.push_back(uv2.x);
				coords.push_back(uv2.y);
			}
		}
		std::cout << "Total vertices: " << coords.size() / 14 << std::endl;
		return coords;
	}
	//Indeces for a sphere
	std::vector<unsigned int> sphereIndeces(int n) {
		std::vector<unsigned int> indices = std::vector<unsigned int>();
		int n_steps = (n % 2 == 0) ? n : n + 1;
		for (int i = 0; i < n_steps*n_steps * 2 * 3; i++) {
			indices.push_back(i);
		}
		return indices;
	}
};