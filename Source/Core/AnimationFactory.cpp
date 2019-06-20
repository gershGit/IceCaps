#include "Core/AnimationFactory.h"
#include "Core/StringTranslation.h"
#include <fstream>

//Loads an animation from a file into an animation reference
void AnimationFactory::loadFromFile(char * fileName, animation &anim, configurationStructure & config)
{
	std::cout << "\tLoading Animation from file: " << fileName << std::endl;
	anim.animationWeight = 1.0f;
	anim.state = ANIMATION_PAUSED;
	anim.repeat = false;

	FILE * animFile;
	char buffer[256];
	char keyString[128];
	char value[128];
	int valSize;
	animFile = fopen(fileName, "r");

	while (fscanf(animFile, "%s", buffer) > 0) {
		getSubComponent(buffer, keyString, 256);
		getValue(buffer, value, 256);
		if (strcmp(keyString, "NAME") == 0) {
			anim.name = value;
		}
		else if (strcmp(keyString, "FRAME_COUNT") == 0) {
			anim.frameCount = atoi(value);
			anim.frames = (key_frame*)malloc(sizeof(key_frame) * anim.frameCount);
		}
		else if (strcmp(keyString, "LENGTH") == 0) {
			anim.length = strtod(value, nullptr);
		}
		else if (strcmp(keyString, "FRAME") == 0) {
			int index = atoi(value);
			anim.frames[index].index = index;
			anim.frames[index].deltaPosition = glm::vec3(0);
			anim.frames[index].deltaRotation = glm::vec3(0);
			anim.frames[index].deltaScale = glm::vec3(0);

			while (fscanf(animFile, "%s", buffer) > 0) {
				getSubComponent(buffer, keyString, 256);
				valSize = getValue(buffer, value, 256);
				if (strcmp(keyString, "T") == 0) {
					anim.frames[index].t = strtod(value, nullptr);
				}
				else if (strcmp(keyString, "D_POS") == 0) {
					anim.frames[index].deltaPosition = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "D_ROT") == 0) {
					anim.frames[index].deltaRotation = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "D_SCA") == 0) {
					anim.frames[index].deltaScale = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "END")==0) {
					break;
				}
			}
		}
	}
	fclose(animFile);
}