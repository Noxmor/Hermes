#ifndef HM_PLATFORM_H
#define HM_PLATFORM_H

#define BIT(x) (1 << x)

typedef enum Color
{
	HM_COLOR_RED = BIT(0),
	HM_COLOR_GREEN = BIT(1),
	HM_COLOR_BLUE = BIT(2),
	HM_COLOR_INTENSITY = BIT(3)
} Color;

void platform_init(void);

void platform_set_title(const char* title);

void platform_set_text_color(Color color);

void platform_clear_screen(void);

void platform_shutdown(void);

#endif