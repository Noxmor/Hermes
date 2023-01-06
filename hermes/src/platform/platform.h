#ifndef HM_PLATFORM_H
#define HM_PLATFORM_H

#include "core/core.h"

typedef enum KeyCode
{
	HM_KEY_UNKNOWN = 0,
	HM_KEY_ARROW_LEFT,
	HM_KEY_ARROW_RIGHT,
	HM_KEY_ARROW_UP,
	HM_KEY_ARROW_DOWN,
	HM_KEY_SPACE,
	HM_KEY_RETURN,
	HM_KEY_BACKSPACE,
	HM_KEY_ESCAPE,
	HM_KEY_A,
	HM_KEY_B,
	HM_KEY_C,
	HM_KEY_D,
	HM_KEY_E,
	HM_KEY_F,
	HM_KEY_G,
	HM_KEY_H,
	HM_KEY_I,
	HM_KEY_J,
	HM_KEY_K,
	HM_KEY_L,
	HM_KEY_M,
	HM_KEY_N,
	HM_KEY_O,
	HM_KEY_P,
	HM_KEY_Q,
	HM_KEY_R,
	HM_KEY_S,
	HM_KEY_T,
	HM_KEY_U,
	HM_KEY_V,
	HM_KEY_W,
	HM_KEY_X,
	HM_KEY_Y,
	HM_KEY_Z
} KeyCode;

const char* platform_keycode_to_str(KeyCode keycode);

KeyCode platform_str_to_keycode(const char* str);

void platform_init(void);

void platform_create_dir(const char* path);

char** platform_get_files_in_dir(const char* dir_path, u64* file_count);

void platform_set_title(const char* title);

u64 platform_get_next_key(void);

void platform_draw_text(u64 x, u64 y, const char* text);

void platform_set_cursor_pos(u64 x, u64 y);

void platform_flush(void);

void platform_clear_screen(void);

void platform_shutdown(void);

#endif