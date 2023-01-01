#include "core/platform_detection.h"

#ifdef HM_PLATFORM_WINDOWS

#include "platform.h"

#include <windows.h>

typedef struct Platform
{
	HANDLE console;
} Platform;

static Platform platform;

void platform_init()
{
	platform.console = GetStdHandle(STD_OUTPUT_HANDLE);
}

void platform_set_title(const char* title)
{
	SetConsoleTitle(title);
}

void platform_set_text_color(Color color)
{
	WORD windows_color = 0;

	if (color & HM_COLOR_RED)
		windows_color |= FOREGROUND_RED;

	if (color & HM_COLOR_GREEN)
		windows_color |= FOREGROUND_GREEN;

	if (color & HM_COLOR_BLUE)
		windows_color |= FOREGROUND_BLUE;

	SetConsoleTextAttribute(platform.console, windows_color);
}

void platform_shutdown()
{
	platform_set_text_color(HM_COLOR_RED | HM_COLOR_GREEN | HM_COLOR_BLUE);
}

#endif