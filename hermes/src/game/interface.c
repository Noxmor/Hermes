#include "interface.h"

#include <string.h>

#include "platform/platform.h"

void interface_render_commands(Interface* inf, LocaleHandler* locale_handler)
{
	for (u8 i = 0; i < inf->command_count; ++i)
	{
		Color color = HM_COLOR_FOREGROUND_RED | HM_COLOR_FOREGROUND_GREEN | HM_COLOR_FOREGROUND_BLUE;

		if (i == inf->current_command_index)
		{
			color = HM_COLOR_FOREGROUND_RED;
			platform_set_cursor_pos(2 + strlen(locale_handler_get(locale_handler, inf->commands[i].name_id)), 5 + i);
			platform_draw_text(0, 5 + i, "> ", color);
			platform_draw_text(2, 5 + i, locale_handler_get(locale_handler, inf->commands[i].name_id), color);
		}
		else
			platform_draw_text(0, 5 + i, locale_handler_get(locale_handler, inf->commands[i].name_id), color);
	}
}