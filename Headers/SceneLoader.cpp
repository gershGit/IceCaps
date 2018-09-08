#include "SceneLoader.h"

void SceneLoader::loadScene(const char * scene_file, std::vector<GameObject*>& objects)
{
	std::cout << "Loading scene from " << scene_file << std::endl;

	//Attempt to open file
	FILE* file;
	errno_t err;
	err = fopen_s(&file, scene_file, "r");

	if (file == NULL) {
		std::cout << "File open error!" << std::endl;
		return;
	}

	while (true) {
		//Determine the type of each line
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF) {
			break;
		}
		else {
			if (strcmp(lineHeader, "ice") == 0) {
				
			} else if (strcmp(lineHeader, "obj") == 0) {
				
			} else if (strcmp(lineHeader, "std") == 0) {

			} else if (strcmp(lineHeader, "terrain") == 0) {

			} else if (strcmp(lineHeader, "draw") == 0) {

			} else if (strcmp(lineHeader, "pSys") == 0) {

			} else if (strcmp(lineHeader, "mat") == 0) {

			} 
		}
	}
}
