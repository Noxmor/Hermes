#include "core/platform_detection.h"

#ifdef HM_PLATFORM_WINDOWS

#include "core/core.h"
#include "core/memory_system.h"
#include "platform.h"

#include <windows.h>
#include <conio.h>

typedef struct Platform
{
	HANDLE console;
	u64 screen_width;
	u64 screen_height;
	DWORD* next_screen;
} Platform;

static Platform platform;

void platform_init(void)
{
	HM_ASSERT(platform.console == NULL);
	platform.console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(platform.console, &csbi);

	platform.screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	platform.screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	platform.next_screen = memory_system_malloc(platform.screen_width * platform.screen_height * sizeof(DWORD), HM_MEMORY_GROUP_PLATFORM);

	platform_clear_screen();
	platform_flush();
}

void platform_create_dir(const char* path)
{
	CreateDirectoryA(path, NULL);
}

char** platform_get_files_in_dir(const char* dir_path, u64* file_count)
{
	char** files = NULL;
	
	u64 files_found = 0;

	WIN32_FIND_DATA fd;
	HANDLE find = FindFirstFile(dir_path, &fd);

	if (find != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				++files_found;
				files = memory_system_realloc(files, files_found * sizeof(char*), (files_found - 1) * sizeof(char*), HM_MEMORY_GROUP_STRING);
				files[files_found - 1] = memory_system_malloc((strlen(fd.cFileName) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
				strcpy(files[files_found - 1], fd.cFileName);
			}
		} while (FindNextFile(find, &fd));
		FindClose(find);
	}

	if (file_count != NULL)
		*file_count = files_found;

	return files;
}

void platform_set_title(const char* title)
{
	HM_ASSERT(platform.console != NULL); 
	
	SetConsoleTitle(title);
}

u64 platform_get_next_key(void)
{
	i32 key_code = getch();
	
	if (key_code == 0 || key_code == 0xE0)
		key_code = getch();

	switch (key_code)
	{
		case 27: return HM_KEY_ESCAPE;
		case 72: return HM_KEY_ARROW_UP;
		case 80: return HM_KEY_ARROW_DOWN;
		case 75: return HM_KEY_ARROW_LEFT;
		case 77: return HM_KEY_ARROW_RIGHT;
		case 32: return HM_KEY_SPACE;
		case 8: return HM_KEY_BACKSPACE;
		case 13: return HM_KEY_RETURN;
		case 'a': return HM_KEY_A;
		case 'b': return HM_KEY_B;
		case 'c': return HM_KEY_C;
		case 'd': return HM_KEY_D;
		case 'e': return HM_KEY_E;
		case 'f': return HM_KEY_F;
		case 'g': return HM_KEY_G;
		case 'h': return HM_KEY_H;
		case 'i': return HM_KEY_I;
		case 'j': return HM_KEY_J;
		case 'k': return HM_KEY_K;
		case 'l': return HM_KEY_L;
		case 'm': return HM_KEY_M;
		case 'n': return HM_KEY_N;
		case 'o': return HM_KEY_O;
		case 'p': return HM_KEY_P;
		case 'q': return HM_KEY_Q;
		case 'r': return HM_KEY_R;
		case 's': return HM_KEY_S;
		case 't': return HM_KEY_T;
		case 'u': return HM_KEY_U;
		case 'v': return HM_KEY_V;
		case 'w': return HM_KEY_W;
		case 'x': return HM_KEY_X;
		case 'y': return HM_KEY_Y;
		case 'z': return HM_KEY_Z;
	}

	HM_WARN("Unknown key '%d' was pressed!", key_code);

	return HM_KEY_UNKNOWN;
}

WORD hermes_color_code_to_windows_color(char color_code)
{
	switch (color_code)
	{
		case '0': return 0;
		case '1': return FOREGROUND_BLUE;
		case '2': return FOREGROUND_GREEN;
		case '3': return FOREGROUND_GREEN | FOREGROUND_BLUE;
		case '4': return FOREGROUND_RED;
		case '5': return FOREGROUND_RED | FOREGROUND_BLUE;
		case '6': return FOREGROUND_RED | FOREGROUND_GREEN;
		case '7': return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		case '8': return 0 | FOREGROUND_INTENSITY;
		case '9': return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case 'a': return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		case 'b': return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case 'c': return FOREGROUND_RED | FOREGROUND_INTENSITY;
		case 'd': return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case 'e': return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		case 'f': return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}

	return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
}

void platform_draw_text(u64 x, u64 y, const char* text)
{
	HM_ASSERT(platform.console != NULL);

	WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	u64 start_index = y * platform.screen_width + x;

	for (u64 i = 0; i < strlen(text); ++i)
	{
		const char c = text[i];

		if (c < 0 && text[i + 1] < 0)
			continue;

		if (c == '?')
		{
			color = hermes_color_code_to_windows_color(text[++i]);

			if (text[i] != '7' && color == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) && i < strlen(text))
				HM_WARN("[Platform]: Unknown color code '%c' in text \"%s\"!", text[i], text);

			continue;
		}

		if (c == '\n')
		{
			start_index = (y + 1) * platform.screen_width + 0;
			continue;
		}

		if (c == '\\' && text[i + 1] == 'n')
		{
			++i;
			start_index = (y + 1) * platform.screen_width + 0;
			continue;
		}

		const DWORD element = (u8)c | (color << 16);
		platform.next_screen[start_index++] = element;
	}
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
			FillConsoleOutputAttribute(platform.console, platform.next_screen[y * platform.screen_width + x] >> 16, 1, pos, &attr_written);
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

	memory_system_free(platform.next_screen, platform.screen_width * platform.screen_height * sizeof(DWORD), HM_MEMORY_GROUP_PLATFORM);
}

#endif