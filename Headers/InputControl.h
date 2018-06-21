/*
	Class to control the input into this application
*/

#include "Headers.h"

//Names of the unique keys
enum key_name {
	NO_KEY_NAME = 0, UNIQUE_KEY_1,
	Q_KEY, E_KEY, R_KEY, T_KEY, Y_KEY, U_KEY, I_KEY, O_KEY, P_KEY,

	LEFT_ARROW_KEY, RIGHT_ARROW_KEY, UP_ARROW_KEY, DOWN_ARROW_KEY,
	W_KEY, S_KEY, A_KEY, D_KEY,
	BRACKET_OPEN_KEY, BRACKET_CLOSED_KEY,
};

//Enumeration of the different state a key could be in
enum key_state {
	UP = 0, DOWN = 1, REPEAT = 2
};

//Structure to refer to a key
struct key {
	key_name name = NO_KEY_NAME;
	key_state state = UP;
};

class InputControl
{
private:
	//List of different keys
	//TODO turn this into an array filled with all necessary keys
	key up_key;
	key down_key;
	key left_key;
	key right_key;
	key w_key;
	key s_key;
	key a_key;
	key d_key;
	key q_key;
	key e_key;
public:
	//Reference to a window to capture the input from
	GLFWwindow * inputWindow;
	//Constructor and destructor
	InputControl(GLFWwindow* inputWindowArg);
	~InputControl();

	//Functions to set the state of different keys
	void setFlags();
	void setFlag(int key, int scancode, int action, int mods);

	//Get information from a key by it's name
	bool isDown(key_name name);
	bool isUp(key_name name);
	bool isFirstPress(key_name name);
};
