#pragma once

#include "Core/Main_Headers.h"
#include "Core/ManagersFactories.h"
#include "Core/EntitySystem.h"

class InputSystem : public EntitySystem {
public:
	static key_state* keys;
	static key_state* mouse_buttons;
	static int* pressedKeys;
	static int pressedKeysSize;
	static int* releasedKeys;
	static int releasedKeysSize;
	static int* pressedButtons;
	static int pressedButtonsSize;
	static int* releasedButtons;
	static int releasedButtonsSize;
	static int* atKey; //Array of scancodes, with index equal to key_code and value equal to scancode
	static int* atButton;
	static double cursorX;
	static double cursorY;
	
	static void showCursor(GLFWwindow* window);
	static void setKey(int key, int scancode, int action, int mods);
	static void setCursor(double x, double y) { cursorX = x; cursorY = y; };
	static void setMouseButton(int button, int action, int mods);
	
	void setEntityComponents(std::vector<ComponentManager*> &managers, std::vector<std::vector<component_type>> required = { {TAGS_COMPONENT} }, uint64_t tags = 0);
	void initialize();
	void start() {};
	void onUpdate();
	void updateKeys();
	InputSystem();
	~InputSystem() {};

	void applyInputs();
	void applyInputOnEntity(int entityID);
};