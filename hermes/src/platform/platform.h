#ifndef HM_PLATFORM_H
#define HM_PLATFORM_H

#include "core/core.h"

#define BIT(x) (1 << x)

typedef enum Color
{
	HM_COLOR_LAST = 0,

	HM_COLOR_FOREGROUND_RED = BIT(0),
	HM_COLOR_FOREGROUND_GREEN = BIT(1),
	HM_COLOR_FOREGROUND_BLUE = BIT(2),
	HM_COLOR_FOREGROUND_INTENSITY = BIT(3),

	HM_COLOR_BACKGROUND_RED = BIT(4),
	HM_COLOR_BACKGROUND_GREEN = BIT(5),
	HM_COLOR_BACKGROUND_BLUE = BIT(6),
	HM_COLOR_BACKGROUND_INTENSITY = BIT(7)
} Color;

void platform_init(void);

void platform_set_title(const char* title);

void platform_draw_text(u64 x, u64 y, const char* text, Color color);

void platform_set_cursor_pos(u64 x, u64 y);

void platform_flush(void);

void platform_clear_screen(void);

void platform_shutdown(void);

#endif