#include "ObjectFactory.h"

ObjectFactory::ObjectFactory()
{
	light_material = new GLMaterial();
	light_material->type = SIMPLE;
	light_material->color = glm::vec3(1, 1, 1);
	ShaderProgram pShader = ShaderProgram("base.vert", "base.frag");
	light_material->shader = pShader;

	purple_material = new GLMaterial();
	purple_material->type = SIMPLE;
	purple_material->color = glm::vec3(1, 0.2, 1);
	purple_material->shader = pShader;

	mCoordsSpawner = new CoordsSpawner();
}

GameObject* ObjectFactory::createObject(primitive primitive_type) {
	if (primitive_type == CUBE_PRIMITIVE) {
		GLDrawable* squareMesh = new GLDrawable();
		squareMesh->ptype = DRAWABLE;
		squareMesh->renderFlag = true;
		squareMesh->dtype = MESH;
		squareMesh->usingEBO = true;
		squareMesh->coords = mCoordsSpawner->squareCoordsOnly;
		squareMesh->indices = mCoordsSpawner->squareIndices;
		squareMesh->material = purple_material;

		GameObject* cube = new GameObject();
		cube->name = "CUBE";
		cube->properties.push_back(squareMesh);
		cube->glDrawable = squareMesh;
		cube->drawFlag = true;
		RigidBody* squareRigid = new RigidBody();
		squareRigid->setStart(cube->pos, glm::vec3(0), glm::vec3(0));
		cube->rigidBody = squareRigid;
		cube->usingRigid = true;
		SphereCollider* squareCollider = new SphereCollider();
		squareCollider->position = cube->pos;
		squareCollider->radius = 1;
		cube->usingCollider = true;
		cube->sCollider = squareCollider;

		return cube;
	}
}

GameObject* ObjectFactory::createLight(lightType type_of_light, glm::vec3 position, glm::vec3 color, float strength, bool visible = true) {
	if (type_of_light == POINT_LIGHT) {
		GameObject* cube = new GameObject();
		cube->name = "POINT_LIGHT_";
		cube->pos = position;

		if (visible == true) {
			GLDrawable* squareMesh = new GLDrawable();
			squareMesh->ptype = DRAWABLE;
			squareMesh->renderFlag = true;
			squareMesh->dtype = MESH;
			squareMesh->usingEBO = true;
			squareMesh->coords = mCoordsSpawner->squareCoordsOnly;
			squareMesh->indices = mCoordsSpawner->squareIndices;
			squareMesh->material = light_material;

			cube->properties.push_back(squareMesh);
			cube->glDrawable = squareMesh;
			cube->drawFlag = true;
			cube->scale = glm::vec3(.25f, .25f, .25f);
		}

		Light* lightInfo = new Light();
		lightInfo->color = color;
		lightInfo->strength = strength;
		cube->light = lightInfo;

		return cube;
	}
}