#pragma once
#include <vector>

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
std::vector<float> planeCoords = {
	//Single face
	-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,
};


std::vector<float> planeCoordsOnly = {
	-1.0f, 0.0f, -1.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,
};
std::vector<unsigned int> planeIndeces = {
	0, 2, 1,
	2, 3, 1,
};

std::vector<float> squareCoordsOnly = {
	//Front face			Normal				Tangent				Bitangent			UV
	-1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 1.0,	//Front left top		0
	-1.0f, -1.0f, -1.0f,	0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0,	//Front left bottom		1
	1.0f, -1.0f, -1.0f,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		1.0, 0.0,	//Front right bottom	2
	1.0f, 1.0f, -1.0f,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		1.0, 1.0,	//Front right top		3

	//Back face
	-1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 1.0,	//Back left top			4
	-1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		0.0, 0.0,	//Back left bottom		5
	1.0f, -1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		1.0, 0.0,	//Back right bottom		6
	1.0f, 1.0f, 1.0f,		0.0, 0.0, 1.0,		0.0, 0.0, -1.0,		0.0, 0.0, -1.0,		1.0, 1.0,	//Back right top 		7																											
};
std::vector<unsigned int> squareIndices = {
	//Front
	1, 0, 2,
	0, 3, 2,

	//Back
	4, 5, 7,
	6, 7, 5,

	//Right
	2, 3, 7,
	2, 7, 6,

	//Left
	4, 0, 1,
	4, 1, 5,

	//Top
	0, 4, 7,
	0, 7, 3,

	//Bottom
	5, 1, 2,
	5, 2, 6,
};