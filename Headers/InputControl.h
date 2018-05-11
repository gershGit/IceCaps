#include "Headers.h"

enum key_name {
		NO_KEY_NAME = 0, UNIQUE_KEY_1,
		Q_KEY, E_KEY, R_KEY, T_KEY, Y_KEY, U_KEY, I_KEY, O_KEY, P_KEY,

		LEFT_ARROW_KEY, RIGHT_ARROW_KEY, UP_ARROW_KEY, DOWN_ARROW_KEY, 
		W_KEY, S_KEY, A_KEY, D_KEY,
		BRACKET_OPEN_KEY, BRACKET_CLOSED_KEY, };

enum key_state {
	UP = 0, DOWN =1 , REPEAT = 2
};

struct key {
	key_name name = NO_KEY_NAME;
	key_state state = UP;
};

class InputControl
{
private:
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
	GLFWwindow * inputWindow;
	InputControl(GLFWwindow* inputWindowArg);
	~InputControl();

	void setFlags();
	void setFlag(int key, int scancode, int action, int mods);
	bool isDown(key_name name);
	bool isUp(key_name name);
	bool isFirstPress(key_name name);
};
