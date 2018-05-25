/*
	The class all game objects must use
	The main purpose is positioning of an object 
	This class uses references to others to fill out necessary information
*/

#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtx/matrix_decompose.hpp>
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
	//Constructor and destructor
	GameObject() {
		children = std::vector<GameObject*>();
		parentTransform[0][0] = 1.0f;
		parentTransform[1][1] = 1.0f;
		parentTransform[2][2] = 1.0f;
		parentTransform[3][3] = 1.0f;
	};
	~GameObject() {};
private:
	//Private matrix used to return the position quickly when an object hasn't moved
	glm::mat4 lastTransform;

public:
	//Name of the object, primarily used for debugging
	const char* name = "UNNAMED";

	//Object's children
	std::vector<GameObject*> children;

	//Object's parent
	GameObject* parent;

	//Vector for the position, rotation, and scale of an object in x,y,z format
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;

	//Matrix to be added to when this object has a parent
	glm::mat4 parentTransform;

	//Bool to determine when a new transformation matrix must be created
	bool moved = true;

	//Determines whether or not the object needs to be considered for drawing
	bool drawFlag = false;

	//Vector of properties of the game object
	//TODO deprecate in favor of specific pointers
	std::vector<Property*> properties;

	//Reference to a drawable
	GLDrawable* glDrawable;
	//Reference to a camera
	GLCamera* camera;

	//Functions to return a normalized vector pointing in the respective directions from the object
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

	//Function to return a transformation matrix for the object
	glm::mat4 getTransform() {
		//Only calculate a new matrix if the object has moved
		if (moved) {
			//Generate a translation matrix from the translation vector
			glm::mat4 myTranslate = glm::mat4(1);
			myTranslate[3][0] = pos.x;
			myTranslate[3][1] = pos.y;
			myTranslate[3][2] = pos.z;

			//Create a quaternion from the rotation vector and build a rotation matrix from it
			glm::quat myRotQuat = glm::quat(rot);
			glm::mat4 myRotate = glm::toMat4(myRotQuat);

			//Generate a scale matrix from the scale vector, defaulting to 1
			glm::mat4 myScale = glm::mat4();
			myScale[0][0] = scale.x>0 ? scale.x : 1;
			myScale[1][1] = scale.y>0 ? scale.y : 1;
			myScale[2][2] = scale.z>0 ? scale.z : 1;
			myScale[3][3] = 1;

			//Update the boolean for quicker returns
			moved = false;

			//Generate the final transform matrix
			glm::mat4 myTransform = parentTransform * myTranslate * myRotate * myScale;

			//Update the objects lastTransform for quick access
			lastTransform = myTransform;

			//Update this objects children with it's transformation
			for (GameObject* child : children) {
				child->parentTransform = myTransform;
				child->moved = true;
			}
			//return the transformation matrix
			return myTransform;
		}
		else {
			return lastTransform;
		}
	};

	//Add a reference to a new child for this object
	void addChild(GameObject* newChild) {
		children.push_back(newChild);
	};

	//Returns the children of this game object
	std::vector<GameObject*> getChildren() {
		return children;
	};
};
