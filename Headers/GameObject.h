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
		glm::quat q = glm::quat(rot);
		glm::vec3 forwardVec(2*(q.x*q.z + q.w*q.y), 2 * (q.y*q.z - q.w*q.x), 1 - 2 * (q.x*q.x + q.y*q.y));

		return glm::normalize(forwardVec);
	};
	glm::vec3 up() {
		glm::quat q = glm::quat(rot);
		glm::vec3 upVec(2 * (q.x*q.y - q.w*q.z), 1 - 2 * (q.x*q.x + q.z*q.z), 2 * (q.y*q.z + q.w*q.x));

		return glm::normalize(upVec);
	};
	glm::vec3 right() {
		glm::quat q = glm::quat(rot);
		glm::vec3 rightVec(1- 2 * (q.y*q.y + q.z*q.z), 2 * (q.x*q.y + q.w*q.z), 2 * (q.x*q.z - q.w*q.y));

		return glm::normalize(-rightVec);
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
