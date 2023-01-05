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

	HM_WARN("[Platform]: platform_str_to_keycode(): Invalid string specified! String was: %s", str);

	return HM_KEY_UNKNOWN;
}