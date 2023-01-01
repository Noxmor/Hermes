#include "core/platform_detection.h"

#ifdef HM_PLATFORM_WINDOWS

#include "core/core.h"
#include "platform.h"

#include <windows.h>

typedef struct Platform
{
	HANDLE console;
} Platform;

static Platform platform;

void platform_init(void)
{
	HM_ASSERT(platform.console == NULL);
	platform.console = GetStdHandle(STD_OUTPUT_HANDLE);
}

void platform_set_title(const char* title)
{
	HM_ASSERT(platform.console != NULL); 
	
	SetConsoleTitle(title);
}

void platform_set_text_color(Color color)
{
	HM_ASSERT(platform.console != NULL); 
	
	WORD windows_color = 0;

	if (color & HM_COLOR_RED)
		windows_color |= FOREGROUND_RED;

	if (color & HM_COLOR_GREEN)
		windows_color |= FOREGROUND_GREEN;

	if (color & HM_COLOR_BLUE)
		windows_color |= FOREGROUND_BLUE;

	SetConsoleTextAttribute(platform.console, windows_color);
}

void platform_shutdown(void)
{
	platform_set_text_color(HM_COLOR_RED | HM_COLOR_GREEN | HM_COLOR_BLUE);
}

void platform_clear_screen(void)
{
	HM_ASSERT(platform.console != NULL);
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	GetConsoleScreenBufferInfo(platform.console, &csbi);
	DWORD chars_written = 0;
	FillConsoleOutputCharacter(platform.console, ' ', csbi.dwSize.X * csbi.dwSize.Y, startCoords, &chars_written);
}

#endif