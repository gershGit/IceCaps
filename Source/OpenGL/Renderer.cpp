#include "Renderer.h"

void GLRenderer::cullObjects(std::vector<GameObject*>* objectsToCull, std::vector<GameObject*>* renderList)
{
	for (GameObject* object : *objectsToCull) {

		renderList->push_back(object);


		//Broad phase culling
		//glm::vec3 camToObj = object->pos - myCamera->pos;
		//if (glm::dot(camToObj, myCamera->forward()) > 0) {
		//	renderList->push_back(object);
		//}

		//TODO add frustrum culling with AABB
		//TODO add culling conditions
	}
}
