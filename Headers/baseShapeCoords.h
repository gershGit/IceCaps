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

std::vector<float> planeCoords = {
	//Single face
	-1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,
};
