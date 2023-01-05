#ifndef HM_PLATFORM_H
#define HM_PLATFORM_H

#include "core/core.h"

#define BIT(x) (1 << x)

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
	HM_KEY_ESCAPE
} KeyCode;

const char* platform_keycode_to_str(KeyCode keycode);

void platform_init(void);

void platform_create_dir(const char* path);

void platform_set_title(const char* title);

u64 platform_get_next_key(void);

void platform_draw_text(u64 x, u64 y, const char* text);

void platform_set_cursor_pos(u64 x, u64 y);

void platform_flush(void);

void platform_clear_screen(void);

void platform_shutdown(void);

#endif