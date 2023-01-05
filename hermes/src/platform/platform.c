#include "platform.h"

#include <string.h>

const char* platform_keycode_to_str(KeyCode keycode)
{
	switch (keycode)
	{
		case HM_KEY_UNKNOWN: return "HM_KEY_UNKNOWN";
		case HM_KEY_ARROW_LEFT: return "HM_KEY_ARROW_LEFT";
		case HM_KEY_ARROW_RIGHT: return "HM_KEY_ARROW_RIGHT";
		case HM_KEY_ARROW_UP: return "HM_KEY_ARROW_UP";
		case HM_KEY_ARROW_DOWN: return "HM_KEY_ARROW_DOWN";
		case HM_KEY_SPACE: return "HM_KEY_SPACE";
		case HM_KEY_RETURN: return "HM_KEY_RETURN";
		case HM_KEY_BACKSPACE: return "HM_KEY_BACKSPACE";
		case HM_KEY_ESCAPE: return "HM_KEY_ESCAPE";
		case HM_KEY_A: return "HM_KEY_A";
		case HM_KEY_B: return "HM_KEY_B";
		case HM_KEY_C: return "HM_KEY_C";
		case HM_KEY_D: return "HM_KEY_D";
		case HM_KEY_E: return "HM_KEY_E";
		case HM_KEY_F: return "HM_KEY_F";
		case HM_KEY_G: return "HM_KEY_G";
		case HM_KEY_H: return "HM_KEY_H";
		case HM_KEY_I: return "HM_KEY_I";
		case HM_KEY_J: return "HM_KEY_J";
		case HM_KEY_K: return "HM_KEY_K";
		case HM_KEY_L: return "HM_KEY_L";
		case HM_KEY_M: return "HM_KEY_M";
		case HM_KEY_N: return "HM_KEY_N";
		case HM_KEY_O: return "HM_KEY_O";
		case HM_KEY_P: return "HM_KEY_P";
		case HM_KEY_Q: return "HM_KEY_Q";
		case HM_KEY_R: return "HM_KEY_R";
		case HM_KEY_S: return "HM_KEY_S";
		case HM_KEY_T: return "HM_KEY_T";
		case HM_KEY_U: return "HM_KEY_U";
		case HM_KEY_V: return "HM_KEY_V";
		case HM_KEY_W: return "HM_KEY_W";
		case HM_KEY_X: return "HM_KEY_X";
		case HM_KEY_Y: return "HM_KEY_Y";
		case HM_KEY_Z: return "HM_KEY_Z";
	}

	HM_WARN("[Platform]: platform_keycode_to_str(): Invalid keycode specified! Keycode was: %d", keycode);

	return "HM_KEY_UNKNOWN";
}

KeyCode platform_str_to_keycode(const char* str)
{
	if (strcmp(str, "HM_KEY_UNKNOWN") == 0)
		return HM_KEY_UNKNOWN;

	if (strcmp(str, "HM_KEY_ARROW_LEFT") == 0)
		return HM_KEY_ARROW_LEFT;

	if (strcmp(str, "HM_KEY_ARROW_RIGHT") == 0)
		return HM_KEY_ARROW_RIGHT;

	if (strcmp(str, "HM_KEY_ARROW_UP") == 0)
		return HM_KEY_ARROW_UP;

	if (strcmp(str, "HM_KEY_ARROW_DOWN") == 0)
		return HM_KEY_ARROW_DOWN;

	if (strcmp(str, "HM_KEY_SPACE") == 0)
		return HM_KEY_SPACE;

	if (strcmp(str, "HM_KEY_RETURN") == 0)
		return HM_KEY_RETURN;

	if (strcmp(str, "HM_KEY_BACKSPACE") == 0)
		return HM_KEY_BACKSPACE;

	if (strcmp(str, "HM_KEY_ESCAPE") == 0)
		return HM_KEY_ESCAPE;

	if (strcmp(str, "HM_KEY_A") == 0)
		return HM_KEY_A;

	if (strcmp(str, "HM_KEY_B") == 0)
		return HM_KEY_B;

	if (strcmp(str, "HM_KEY_C") == 0)
		return HM_KEY_C;

	if (strcmp(str, "HM_KEY_D") == 0)
		return HM_KEY_D;

	if (strcmp(str, "HM_KEY_E") == 0)
		return HM_KEY_E;

	if (strcmp(str, "HM_KEY_F") == 0)
		return HM_KEY_F;

	if (strcmp(str, "HM_KEY_G") == 0)
		return HM_KEY_G;

	if (strcmp(str, "HM_KEY_H") == 0)
		return HM_KEY_H;

	if (strcmp(str, "HM_KEY_I") == 0)
		return HM_KEY_I;

	if (strcmp(str, "HM_KEY_J") == 0)
		return HM_KEY_J;

	if (strcmp(str, "HM_KEY_K") == 0)
		return HM_KEY_K;

	if (strcmp(str, "HM_KEY_L") == 0)
		return HM_KEY_L;

	if (strcmp(str, "HM_KEY_M") == 0)
		return HM_KEY_M;

	if (strcmp(str, "HM_KEY_N") == 0)
		return HM_KEY_N;

	if (strcmp(str, "HM_KEY_O") == 0)
		return HM_KEY_O;

	if (strcmp(str, "HM_KEY_P") == 0)
		return HM_KEY_P;

	if (strcmp(str, "HM_KEY_Q") == 0)
		return HM_KEY_Q;

	if (strcmp(str, "HM_KEY_R") == 0)
		return HM_KEY_R;

	if (strcmp(str, "HM_KEY_S") == 0)
		return HM_KEY_S;

	if (strcmp(str, "HM_KEY_T") == 0)
		return HM_KEY_T;

	if (strcmp(str, "HM_KEY_U") == 0)
		return HM_KEY_U;

	if (strcmp(str, "HM_KEY_V") == 0)
		return HM_KEY_V;

	if (strcmp(str, "HM_KEY_W") == 0)
		return HM_KEY_W;

	if (strcmp(str, "HM_KEY_X") == 0)
		return HM_KEY_X;

	if (strcmp(str, "HM_KEY_Y") == 0)
		return HM_KEY_Y;

	if (strcmp(str, "HM_KEY_Z") == 0)
		return HM_KEY_Z;

	HM_WARN("[Platform]: platform_str_to_keycode(): Invalid string specified! String was: %s", str);

	return HM_KEY_UNKNOWN;
}