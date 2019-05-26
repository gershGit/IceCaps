#include "Core/InputSystem.h"
#include "Core/KeyDefines.h"

key_state* InputSystem::keys = (key_state*)malloc(sizeof(key_state) * 350);
key_state* InputSystem::mouse_buttons = (key_state*)malloc(sizeof(key_state) * 8);
int* InputSystem::pressedKeys = (int*)malloc(sizeof(int) * 32); //32 possible keys pressed at once
int InputSystem::pressedKeysSize = 0;
int* InputSystem::releasedKeys = (int*)malloc(sizeof(int) * 32); //32 possible keys released at once
int InputSystem::releasedKeysSize = 0;
int* InputSystem::pressedButtons = (int*)malloc(sizeof(int) * 8); //8 possible mouse buttons pressed at once
int InputSystem::pressedButtonsSize = 0;
int* InputSystem::releasedButtons = (int*)malloc(sizeof(int) * 8); //8 possible mouse buttons released at once
int InputSystem::releasedButtonsSize = 0;
int* InputSystem::atKey = (int*)malloc(sizeof(int) * 100);
int* InputSystem::atButton = (int*)malloc(sizeof(int) * 8);
double InputSystem::cursorX = 0.0;
double InputSystem::cursorY = 0.0;

void InputSystem::showCursor(GLFWwindow * window)
{
}

void InputSystem::setKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		keys[scancode] = PRESS;
		pressedKeys[pressedKeysSize] = scancode;
		pressedKeysSize++;
	}
	else if (action == GLFW_RELEASE) {
		keys[scancode] = RELEASE;
		releasedKeys[releasedKeysSize] = scancode;
		releasedKeysSize++;
	}
}

void InputSystem::setMouseButton(int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		mouse_buttons[button] = PRESS;
		pressedButtons[pressedButtonsSize] = button;
		pressedButtonsSize++;
	}
	else if (action == GLFW_RELEASE) {
		mouse_buttons[button] = RELEASE;
		releasedButtons[releasedButtonsSize] = button;
		releasedButtonsSize++;
	}
}

void InputSystem::setEntityComponents(std::vector<ComponentManager*> &managers, std::vector<std::vector<component_type>> required, uint64_t tags)
{
	for (ComponentManager* manager : managers) {
		operatesOn.push_back(manager->componentType);
	}
	entityListRequiredComponents = required;
	requiredTags = tags;
}

void InputSystem::initialize()
{
	if (config->platform == Windows) {
		setKeyArrayForWindows(atKey);
	}
	setMouseArray(atButton);
	glfwGetCursorPos(config->window, &cursorX, &cursorY);
}

void InputSystem::onUpdate()
{
	applyInputs();
	updateKeys();
}

void InputSystem::updateKeys()
{
	for (int i = 0; i < pressedKeysSize; i++) {
		keys[pressedKeys[i]] = DOWN;
	}
	for (int i = 0; i < releasedKeysSize; i++) {
		keys[releasedKeys[i]] = UP;
	}
	for (int i = 0; i < pressedButtonsSize; i++) {
		mouse_buttons[pressedButtons[i]] = DOWN;
	}
	for (int i = 0; i < releasedButtonsSize; i++) {
		mouse_buttons[releasedButtons[i]] = UP;
	}
	pressedKeysSize = 0;
	releasedKeysSize = 0;
	pressedButtonsSize = 0;
	releasedButtonsSize = 0;
}

InputSystem::InputSystem() {
	systemType = INPUT_SYSTEM;
}

void InputSystem::applyInputs()
{
	for (int i = 0; i < entities->size(); i++) {
		applyInputOnEntity(entities->at(i));
	}
}

//User defined functions go here 
void InputSystem::applyInputOnEntity(int entityID)
{
}