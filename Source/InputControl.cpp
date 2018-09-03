#include "InputControl.h"

//TODO fill this all in
InputControl::InputControl(GLFWwindow * inputWindow)
{
}

InputControl::~InputControl()
{
}

void InputControl::setFlags()
{
	int state = glfwGetKey(inputWindow, GLFW_KEY_UP);
	if (state == GLFW_PRESS) {
		up_key.state = DOWN;
	}
	else if (state == GLFW_REPEAT) {
		up_key.state = DOWN;
	}
	else if (state == GLFW_RELEASE) {
		up_key.state = UP;
	}
}

void InputControl::setFlag(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			up_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			up_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			up_key.state = UP;
		}
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			down_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			down_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			down_key.state = UP;
		}
	}
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			left_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			left_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			left_key.state = UP;
		}
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			right_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			right_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			right_key.state = UP;
		}
	}
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			w_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			w_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			w_key.state = UP;
		}
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			s_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			s_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			s_key.state = UP;
		}
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			a_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			a_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			a_key.state = UP;
		}
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			d_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			d_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			d_key.state = UP;
		}
	}
	if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) {
			q_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			q_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			q_key.state = UP;
		}
	}
	if (key == GLFW_KEY_P) {
		if (action == GLFW_PRESS) {
			p_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			p_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			p_key.state = UP;
		}
	}
	if (key == GLFW_KEY_O) {
		if (action == GLFW_PRESS) {
			o_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			o_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			o_key.state = UP;
		}
	}
	if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) {
			e_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			e_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			e_key.state = UP;
		}
	}
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			space_key.state = DOWN;
		}
		else if (action == GLFW_REPEAT) {
			space_key.state = DOWN;
		}
		else if (action == GLFW_RELEASE) {
			space_key.state = UP;
		}
	}
}

bool InputControl::isDown(key_name name)
{
	if (name == UP_ARROW_KEY) {
		if (up_key.state == DOWN) {
			return true;
		}
	}
	else if (name == DOWN_ARROW_KEY) {
		if (down_key.state == DOWN) {
			return true;
		}
	}
	else if (name == RIGHT_ARROW_KEY) {
		if (right_key.state == DOWN) {
			return true;
		}
	}
	else if (name == LEFT_ARROW_KEY) {
		if (left_key.state == DOWN) {
			return true;
		}
	}
	else if (name == W_KEY) {
		if (w_key.state == DOWN) {
			return true;
		}
	}
	else if (name == S_KEY) {
		if (s_key.state == DOWN) {
			return true;
		}
	}
	else if (name == A_KEY) {
		if (a_key.state == DOWN) {
			return true;
		}
	}
	else if (name == D_KEY) {
		if (d_key.state == DOWN) {
			return true;
		}
	}
	else if (name == Q_KEY) {
		if (q_key.state == DOWN) {
			return true;
		}
	}
	else if (name == E_KEY) {
		if (e_key.state == DOWN) {
			return true;
		}
	}
	else if (name == P_KEY) {
		if (p_key.state == DOWN) {
			return true;
		}
	}
	else if (name == O_KEY) {
		if (o_key.state == DOWN) {
			return true;
		}
	}
	else if (name == SPACE_KEY) {
		if (space_key.state == DOWN) {
			return true;
		}
	}
	return false;
}

bool InputControl::isUp(key_name name)
{
	return false;
}

bool InputControl::isFirstPress(key_name name)
{
	return false;
}