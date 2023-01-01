#include "core/platform_detection.h"

#ifdef HM_PLATFORM_WINDOWS

#include "core/core.h"
#include "platform.h"

#include <stdio.h>
#include <windows.h>

typedef struct Platform
{
	HANDLE console;
	u64 screen_width;
	u64 screen_height;
	DWORD* next_screen;
	WORD last_color;
} Platform;

static Platform platform;

void platform_init(void)
{
	HM_ASSERT(platform.console == NULL);
	platform.console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(platform.console, &csbi);

	platform.screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	platform.screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	platform.next_screen = malloc(platform.screen_width * platform.screen_height * sizeof(DWORD));

	platform_clear_screen();
	platform_flush();
}

void platform_set_title(const char* title)
{
	HM_ASSERT(platform.console != NULL); 
	
	SetConsoleTitle(title);
}

WORD hermes_to_windows_color(Color color)
{
	HM_ASSERT(platform.console != NULL); 
	
	if (color == HM_COLOR_LAST)
		return platform.last_color;

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

	return windows_color;
}

void platform_draw_text(u64 x, u64 y, const char* text, Color color)
{
	HM_ASSERT(platform.console != NULL);

	WORD windows_color = hermes_to_windows_color(color);

	const u64 start_index = y * platform.screen_width + x;

	for (u64 i = 0; i < strlen(text); ++i)
	{
		DWORD element = text[i] | (windows_color << 8);
		platform.next_screen[start_index + i] = element;
	}

	platform.last_color = windows_color;
}

void platform_set_cursor_pos(u64 x, u64 y)
{
	HM_ASSERT(platform.console != NULL);
	
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(platform.console, coord);
}

void platform_flush(void)
{
	HM_ASSERT(platform.console != NULL);
	
	DWORD chars_written = 0;
	DWORD attr_written = 0;

	for (u64 y = 0; y < platform.screen_height; ++y)
	{
		for (u64 x = 0; x < platform.screen_width; ++x)
		{
			const COORD pos = { x, y };
			FillConsoleOutputCharacter(platform.console, platform.next_screen[y * platform.screen_width + x], 1, pos, &chars_written);
			FillConsoleOutputAttribute(platform.console, (platform.next_screen[y * platform.screen_width + x]) >> 8, 1, pos, &attr_written);
		}
	}
}

void platform_clear_screen(void)
{
	HM_ASSERT(platform.console != NULL);
	
	memset(platform.next_screen, 0, platform.screen_width * platform.screen_height * sizeof(DWORD));
}

void platform_shutdown(void)
{
	platform_clear_screen();
	platform_flush();

	platform_set_cursor_pos(0, 0);

	free(platform.next_screen);
}

#endif