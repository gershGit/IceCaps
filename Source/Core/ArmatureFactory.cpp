#include "Core/ArmatureFactory.h"
#include "Core/StringTranslation.h"
#include "Core/Global_Callbacks.h"
#include <fstream>

void calculateInverseBinds(int boneID, armature& arm, glm::mat4 parentBindTransform) {
	glm::mat4 bindTransform = parentBindTransform * arm.bones.at(boneID).localBindTransform;
	arm.bones.at(boneID).inverseBindTransform = glm::inverse(bindTransform);

	for (int i = 0; i < arm.bones.at(boneID).childrenIDs.size(); i++) {
		calculateInverseBinds(arm.bones.at(boneID).childrenIDs.at(i), arm, bindTransform);
	}
}

void ArmatureFactory::loadFromFile(char* fileIn, armature& arm, configurationStructure& config)
{
	std::cout << "\tLoading Armature from file: " << fileIn << std::endl;

	FILE* armFile;
	char buffer[256];
	char keyString[128];
	char value[128];
	int valSize;
	armFile = fopen(fileIn, "r");

	while (fscanf(armFile, "%s", buffer) > 0) {
		getSubComponent(buffer, keyString, 256);
		getValue(buffer, value, 256);
		if (strcmp(keyString, "COUNT") == 0) {
			arm.bones = std::vector<bone>(atoi(value));
			arm.finalTransforms = std::vector<glm::mat4>(atoi(value));
		}
		else if (strcmp(keyString, "WEIGHTS") == 0) {
			arm.maxWeights = atoi(value);
		}
		else if (strcmp(keyString, "TREE") == 0) {
			if (strcmp(value, "CUSTOM") == 0) {
				while (fscanf(armFile, "%s", buffer) > 0) {
					getSubComponent(buffer, keyString, 256);
					valSize = getValue(buffer, value, 256);

					if (strcmp(keyString, "END") == 0) {
						break;
					}

					int parent = atoi(keyString);
					std::vector<int> children = getIntVector(value, valSize);

					arm.bones.at(parent).childrenIDs = children;
				}
			}
		}
		else if (strcmp(keyString, "BONE") == 0) {
			int idVal = atoi(value);
			arm.bones.at(idVal).id = idVal;
			transform tempTransform = transform();

			while (fscanf(armFile, "%s", buffer) > 0) {
				getSubComponent(buffer, keyString, 256);
				valSize = getValue(buffer, value, 256);

				if (strcmp(keyString, "POSITION") == 0) {
					tempTransform.pos = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "ROTATION") == 0) {
					tempTransform.rot = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "SCALE") == 0) {
					tempTransform.scale = getVectorFromString<glm::vec3>(value, valSize);
				}
				else if (strcmp(keyString, "END") == 0) {
					break;
				}
			}

			arm.bones.at(idVal).poseTransform = tempTransform;
			arm.bones.at(idVal).localBindTransform = getTransformationMatrix(tempTransform);
		}
		else if (strcmp(keyString, "END_ARMATURE") == 0) {
			break;
		}
	}
	fclose(armFile);

	calculateInverseBinds(0, arm , glm::mat4(1));
}