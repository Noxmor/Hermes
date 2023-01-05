#include "platform.h"

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
	}

	HM_WARN("[Platform]: platform_keycode_to_str(): Invalid keycode specified! Keycode was: %d", keycode);

	return "HM_KEY_UNKNOWN";
}