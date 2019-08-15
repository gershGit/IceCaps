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
			anim.frames[index].snap.deltaPosition = glm::vec3(0);
			anim.frames[index].snap.deltaRotation = glm::vec3(0);
			anim.frames[index].snap.deltaScale = glm::vec3(0);

			while (fscanf(animFile, "%s", buffer) > 0) {
				getSubComponent(buffer, keyString, 256);
				valSize = getValue(buffer, value, 256);
				if (strcmp(keyString, "T") == 0) {
					anim.frames[index].t = strtod(value, nullptr);
				}
				else if (strcmp(keyString, "D_POS") == 0) {
					anim.frames[index].snap.deltaPosition = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "D_ROT") == 0) {
					anim.frames[index].snap.deltaRotation = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "D_SCA") == 0) {
					anim.frames[index].snap.deltaScale = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "END")==0) {
					break;
				}
			}
		}
	}
	fclose(animFile);
}

int findBoneIndex(int* boneIDs, int size, int idVal) {
	for (int i = 0; i < size; i++) {
		if (boneIDs[i] == idVal) {
			return i;
		}
	}
	return -1;
}

void AnimationFactory::loadArmatureAnimationFromFile(char* fileIn, armature_animation& anim, configurationStructure& config)
{
	std::cout << "\tLoading Armature Animation from file: " << fileIn << std::endl;
	anim.animationWeight = 1.0f;
	anim.state = ANIMATION_PAUSED;
	anim.repeat = false;

	FILE* animFile;
	char buffer[256];
	char keyString[128];
	char value[128];
	int valSize;
	animFile = fopen(fileIn, "r");

	while (fscanf(animFile, "%s", buffer) > 0) {
		getSubComponent(buffer, keyString, 256);
		getValue(buffer, value, 256);
		if (strcmp(keyString, "NAME") == 0) {
			anim.name = value;
		}
		else if (strcmp(keyString, "FRAME_COUNT") == 0) {
			anim.frameCount = atoi(value);
			size_t frames_size = sizeof(armature_key) * anim.frameCount;
			anim.frames = (armature_key*)malloc(frames_size);
		}
		else if (strcmp(keyString, "LENGTH") == 0) {
			anim.length = strtod(value, nullptr);
		}
		else if (strcmp(keyString, "BONES") == 0) {
			anim.boneIDs = std::vector<int>(atoi(value));
			int pos = 0;
			while (fscanf(animFile, "%s", buffer) > 0) {
				getSubComponent(buffer, keyString, 256);
				getValue(buffer, value, 256);

				if (strcmp(keyString, "ID") == 0) {
					anim.boneIDs[pos] = atoi(value);
					pos++;
				}
				else if (strcmp(keyString, "END") == 0) {
					for (int i = 0; i < anim.frameCount; i++) {
						anim.frames[i].boneIDs = (int*)malloc(sizeof(int) * anim.boneIDs.size());
						for (int j = 0; j < anim.boneIDs.size(); j++) {
							anim.frames[i].boneIDs[j] = anim.boneIDs.at(j);
						}
						anim.frames[i].snaps = (snapshot*)malloc(sizeof(snapshot) * anim.boneIDs.size());
					}
					break;
				}
			}
		}
		else if (strcmp(keyString, "FRAME") == 0) {
			int frameIndex = atoi(value);
			anim.frames[frameIndex].index = frameIndex;
			int currBoneIndex = 0;

			while (fscanf(animFile, "%s", buffer) > 0) {
				getSubComponent(buffer, keyString, 256);
				getValue(buffer, value, 256);

				if (strcmp(keyString, "ID") == 0) {
					currBoneIndex = findBoneIndex(anim.frames[frameIndex].boneIDs, anim.boneIDs.size(),atoi(value));

					while (fscanf(animFile, "%s", buffer) > 0) {
						getSubComponent(buffer, keyString, 256);
						valSize = getValue(buffer, value, 256);

						if (strcmp(keyString, "D_POS") == 0) {
							anim.frames[frameIndex].snaps[currBoneIndex].deltaPosition = getVectorFromString<glm::vec3>(value, valSize);
						}
						else if (strcmp(keyString, "D_ROT") == 0) {
							anim.frames[frameIndex].snaps[currBoneIndex].deltaRotation = getVectorFromString<glm::vec3>(value, valSize);
						}
						else if (strcmp(keyString, "D_SCA") == 0) {
							anim.frames[frameIndex].snaps[currBoneIndex].deltaScale = getVectorFromString<glm::vec3>(value, valSize);
						}
						else if (strcmp(keyString, "END") == 0) {
							break;
						}
					}
				} else if (strcmp(keyString, "T") == 0) {
					anim.frames[frameIndex].t = strtod(value, nullptr);
				}
				else if (strcmp(keyString, "END") == 0) {
					break;
				}
			}
		}
		else if (strcmp(keyString, "END_FRAMES") == 0) {
			break;
		}
	}
	fclose(animFile);
}
