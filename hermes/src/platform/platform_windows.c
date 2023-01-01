#include "core/platform_detection.h"

#ifdef HM_PLATFORM_WINDOWS

#include "core/core.h"
#include "platform.h"

#include <stdio.h>
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

void platform_set_draw_color(Color color)
{
	HM_ASSERT(platform.console != NULL); 
	
	if (color == HM_COLOR_LAST)
		return;

	WORD windows_color = 0;

	if (color & HM_COLOR_FOREGROUND_RED)
		windows_color |= FOREGROUND_RED;

	if (color & HM_COLOR_FOREGROUND_GREEN)
		windows_color |= FOREGROUND_GREEN;

	if (color & HM_COLOR_FOREGROUND_BLUE)
		windows_color |= FOREGROUND_BLUE;

	if (color & HM_COLOR_FOREGROUND_INTENSITY)
		windows_color |= FOREGROUND_INTENSITY;

	if (color & HM_COLOR_BACKGROUND_RED)
		windows_color |= BACKGROUND_RED;

	if (color & HM_COLOR_BACKGROUND_GREEN)
		windows_color |= BACKGROUND_GREEN;

	if (color & HM_COLOR_BACKGROUND_BLUE)
		windows_color |= BACKGROUND_BLUE;

	if (color & HM_COLOR_BACKGROUND_INTENSITY)
		windows_color |= BACKGROUND_INTENSITY;

	SetConsoleTextAttribute(platform.console, windows_color);
}

void platform_draw_text(const char* text, Color color)
{
	HM_ASSERT(platform.console != NULL);

	platform_set_draw_color(color);

	printf(text);
}

void platform_set_cursor_pos(u64 x, u64 y)
{
	HM_ASSERT(platform.console != NULL);
	
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(platform.console, coord);
}

void platform_clear_screen(void)
{
	HM_ASSERT(platform.console != NULL);
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	GetConsoleScreenBufferInfo(platform.console, &csbi);
	DWORD chars_written = 0;
	FillConsoleOutputCharacter(platform.console, ' ', csbi.dwSize.X * csbi.dwSize.Y, startCoords, &chars_written);
	DWORD attr_written = 0;
	FillConsoleOutputAttribute(platform.console, HM_COLOR_FOREGROUND_RED | HM_COLOR_FOREGROUND_GREEN | HM_COLOR_FOREGROUND_BLUE, csbi.dwSize.X * csbi.dwSize.Y, startCoords, &attr_written);

	platform_set_cursor_pos(0, 0);
}

void platform_shutdown(void)
{
	platform_set_draw_color(HM_COLOR_FOREGROUND_RED | HM_COLOR_FOREGROUND_GREEN | HM_COLOR_FOREGROUND_BLUE);
	platform_clear_screen();
}

#endif