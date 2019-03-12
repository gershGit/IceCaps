#include "Core/AnimationFactory.h"
#include "Core/StringTranslation.h"
#include <fstream>

template <class Vector>
Vector getDeltaVector(std::string value) {
	int start = 0, length = 0;
	int vectorIndex = 0;
	Vector returnVector = Vector();
	for (int i = 0; i < value.length(); i++) {
		if (value[i] == ',') {
			returnVector[vectorIndex] = strtof(value.substr(start, length).c_str(), nullptr);
			vectorIndex++;
			start = i + 1;
			length = 0;
		}
		else {
			length++;
		}
	}
	returnVector[vectorIndex] = strtof(value.substr(start, length).c_str(), nullptr);
	return returnVector;
}

//Loads an animation from a file into an animation reference
void AnimationFactory::loadFromFile(std::string fileIn, animation &anim, configurationStructure & config)
{
	std::cout << "\tLoading Animation from file: " << fileIn << std::endl;
	anim.animationWeight = 1.0f;
	anim.state = ANIMATION_PAUSED;
	anim.repeat = false;

	std::string line, key, value;
	std::ifstream fileStream(fileIn.c_str());
	while (std::getline(fileStream, line)) {
		key = getSubComponent(line);
		value = getValue(line);
		if (strcmp(key.c_str(), "NAME") == 0) {
			anim.name = value;
		}
		else if (strcmp(key.c_str(), "FRAME_COUNT") == 0) {
			anim.frameCount = atoi(value.c_str());
			anim.frames = (key_frame*)malloc(sizeof(key_frame) * anim.frameCount);
		}
		else if (strcmp(key.c_str(), "LENGTH") == 0) {
			anim.length = strtod(value.c_str(), nullptr);
		}
		else if (strcmp(key.c_str(), "FRAME") == 0) {
			int index = atoi(value.c_str());
			anim.frames[index].index = index;
			anim.frames[index].deltaPosition = glm::vec3(0);
			anim.frames[index].deltaRotation = glm::vec3(0);
			anim.frames[index].deltaScale = glm::vec3(0);

			while (std::getline(fileStream, line)) {
				key = getSubComponent(line);
				value = getValue(line);
				if (strcmp(key.c_str(), "T") == 0) {
					anim.frames[index].t = strtod(value.c_str(), nullptr);
				}
				else if (strcmp(key.c_str(), "D_POS") == 0) {
					anim.frames[index].deltaPosition = getDeltaVector<glm::vec3>(value);
				}
				else if (strcmp(key.c_str(), "D_ROT") == 0) {
					anim.frames[index].deltaRotation = getDeltaVector<glm::vec3>(value);
				}
				else if (strcmp(key.c_str(), "D_SCA") == 0) {
					anim.frames[index].deltaScale = getDeltaVector<glm::vec3>(value);
				}
				else if (strcmp(key.c_str(), "END")==0) {
					break;
				}
			}
		}
	}
}