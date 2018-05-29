/*
	A header only file containing the coordinate information for various shapes and their related indices
*/

#pragma once
#include <vector>

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