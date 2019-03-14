#pragma once

#include "Core/Main_Headers.h"

//Sets the key array for windows platforms
static void setKeyArrayForWindows(int* keyArray) {
	keyArray[KEY_A] = 30;
	keyArray[KEY_B] = 48;
	keyArray[KEY_C] = 46;
	keyArray[KEY_D] = 32;
	keyArray[KEY_E] = 18;
	keyArray[KEY_F] = 33;
	keyArray[KEY_G] = 34;
	keyArray[KEY_H] = 35;
	keyArray[KEY_I] = 23;
	keyArray[KEY_J] = 36;
	keyArray[KEY_K] = 37;
	keyArray[KEY_L] = 38;
	keyArray[KEY_M] = 50;
	keyArray[KEY_N] = 49;
	keyArray[KEY_O] = 24;
	keyArray[KEY_P] = 25;
	keyArray[KEY_Q] = 16;
	keyArray[KEY_R] = 19;
	keyArray[KEY_S] = 31;
	keyArray[KEY_T] = 20;
	keyArray[KEY_U] = 22;
	keyArray[KEY_V] = 47;
	keyArray[KEY_W] = 17;
	keyArray[KEY_X] = 45;
	keyArray[KEY_Y] = 21;
	keyArray[KEY_Z] = 44;

	keyArray[KEY_1] = 2;
	keyArray[KEY_2] = 3;
	keyArray[KEY_3] = 4;
	keyArray[KEY_4] = 5;
	keyArray[KEY_5] = 6;
	keyArray[KEY_6] = 7;
	keyArray[KEY_7] = 8;
	keyArray[KEY_8] = 9;
	keyArray[KEY_9] = 10;
	keyArray[KEY_0] = 11;

	keyArray[KEY_PAD_1] = 79;
	keyArray[KEY_PAD_2] = 80;
	keyArray[KEY_PAD_3] = 81;
	keyArray[KEY_PAD_4] = 75;
	keyArray[KEY_PAD_5] = 76;
	keyArray[KEY_PAD_6] = 77;
	keyArray[KEY_PAD_7] = 71;
	keyArray[KEY_PAD_8] = 72;
	keyArray[KEY_PAD_9] = 73;
	keyArray[KEY_PAD_0] = 82;

	keyArray[KEY_PAD_SLASH] = 309;
	keyArray[KEY_PAD_MUL] = 55;
	keyArray[KEY_PAD_MINUS] = 74;
	keyArray[KEY_PAD_PLUS] = 78;

	keyArray[KEY_ESC] = 1;
	keyArray[KEY_TILDA] = 41;
	keyArray[KEY_TAB] = 15;
	keyArray[KEY_CAPS] = 58;
	keyArray[KEY_LEFT_SHIFT] = 42;
	keyArray[KEY_LEFT_CTL] = 29;
	keyArray[KEY_LEFT_ALT] = 56;
	keyArray[KEY_RIGHT_SHIFT] = 54;
	keyArray[KEY_RIGHT_CTL] = 285;
	keyArray[KEY_RIGHT_ALT] = 312;
	keyArray[KEY_BACKSPACE] = 14;
	keyArray[KEY_DELETE] = 339;
	keyArray[KEY_ENTER] = 28;
	keyArray[KEY_SPACE] = 57;
	keyArray[KEY_BRACKET_OPEN] = 26;
	keyArray[KEY_BRACKET_CLOSE] = 27;
	keyArray[KEY_COLON] = 39;
	keyArray[KEY_QUOTE] = 40;
	keyArray[KEY_WINDOWS] = 347;

	keyArray[KEY_FORWARD_SLASH] = 53;
	keyArray[KEY_BACK_SLASH] = 43;
	keyArray[KEY_GREATER] = 52;
	keyArray[KEY_LESS] = 51;

	keyArray[KEY_F1] = 59;
	keyArray[KEY_F2] = 60;
	keyArray[KEY_F3] = 61;
	keyArray[KEY_F4] = 62;
	keyArray[KEY_F5] = 63;
	keyArray[KEY_F6] = 64;
	keyArray[KEY_F7] = 65;
	keyArray[KEY_F8] = 66;
	keyArray[KEY_F9] = 67;
	keyArray[KEY_F10] = 68;
	keyArray[KEY_F11] = 87;

	keyArray[KEY_MUTE] = 288;
	keyArray[KEY_VOL_MINUS] = 74;
	keyArray[KEY_VOL_PLUS] = 78;
	keyArray[KEY_REFRESH] = 63;

	keyArray[KEY_UP] = 328;
	keyArray[KEY_DOWN] = 336;
	keyArray[KEY_LEFT] = 331;
	keyArray[KEY_RIGHT] = 333;
}

//Sets the mouse array to the correct codes
static void setMouseArray(int* mouseArray) {
	mouseArray[MOUSE_LEFT] = 0;
	mouseArray[MOUSE_RIGHT] = 1;
	mouseArray[MOUSE_SCROLL] = 2;
	
	mouseArray[MOUSE_SIDE_1] = 4;
	mouseArray[MOUSE_SIDE_2] = 5;
	mouseArray[MOUSE_SIDE_3] = 6;
	mouseArray[MOUSE_SIDE_4] = 7;
}