#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Headers.h"
#include "Property.h"
#include "Drawable.h"
#include "Camera.h"
#include <vector>

class GameObject
{
public:
	GameObject() {
		children = std::vector<GameObject*>();
		parentTransform[0][0] = 1.0f;
		parentTransform[1][1] = 1.0f;
		parentTransform[2][2] = 1.0f;
		parentTransform[3][3] = 1.0f;
	};
	~GameObject() {};
private:
	glm::mat4 lastTransform;

public:
	const char* name = "UNNAMED";
	std::vector<GameObject*> children;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::mat4 parentTransform;


	bool moved = true;
	bool drawFlag = false;

	std::vector<Property*> properties;
	GLDrawable* glDrawable;
	GLCamera* camera;

	glm::vec3 forward() {
		//TODO calculate forward
		return glm::vec3(0, 0, 1);
	};
	glm::vec3 up() {
		//TODO calculate up
		return glm::vec3(0, 1, 0);
	};
	glm::vec3 right() {
		//TODO calculate right
		return glm::vec3(1, 0, 0);
	};
	glm::mat4 getTransform() {
		if (moved) {
			glm::mat4 myTranslate = glm::mat4();
			myTranslate[0][0] = 1;
			myTranslate[1][1] = 1;
			myTranslate[2][2] = 1;
			myTranslate[3][3] = 1;
			myTranslate[3][0] = pos.x;
			myTranslate[3][1] = pos.y;
			myTranslate[3][2] = pos.z;

			glm::quat myRotQuat = glm::quat(rot);
			glm::mat4 myRotate = glm::toMat4(myRotQuat);

			glm::mat4 myScale = glm::mat4();
			myScale[0][0] = scale.x>0 ? scale.x : 1;
			myScale[1][1] = scale.y>0 ? scale.y : 1;
			myScale[2][2] = scale.z>0 ? scale.z : 1;
			myScale[3][3] = 1;

			moved = false;
			glm::mat4 myTransform = myTranslate * myRotate * myScale;

			lastTransform = myTransform;
			for (GameObject* child : children) {
				child->parentTransform = myTransform;
				child->moved = true;
			}
			return parentTransform * myTransform;
		}
		else {
			return lastTransform;
		}
	};
	void addChild(GameObject* newChild) {
		children.push_back(newChild);
	};
	std::vector<GameObject*> getChildren() {
		return children;
	};
};
