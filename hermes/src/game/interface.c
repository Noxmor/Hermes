#include "interface.h"

#include <string.h>

#include "core/memory_system.h"

#include "platform/platform.h"

void interface_render_commands(Interface* inf, u64 x, u64 y, LocaleHandler* locale_handler)
{
	for (u8 i = 0; i < inf->command_count; ++i)
	{
		if (i == inf->current_command_index)
		{
			platform_set_cursor_pos(x + 2 + strlen(locale_handler_get(locale_handler, inf->commands[i].name_id)), y + 5 + i);
			platform_draw_text(x, y + 5 + i, "§e> ");

			char* colored_message = memory_system_malloc((strlen(locale_handler_get(locale_handler, inf->commands[i].name_id)) + 2 + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(colored_message, "§e");
			strcpy(colored_message + 2, locale_handler_get(locale_handler, inf->commands[i].name_id));

			platform_draw_text(x + 2, y + 5 + i, colored_message);

			memory_system_free(colored_message, (strlen(colored_message) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		}
		else
			platform_draw_text(x, y + 5 + i, locale_handler_get(locale_handler, inf->commands[i].name_id));
	}
}

void interface_move_up(Interface* inf)
{
	if (inf->current_command_index > 0)
		--(inf->current_command_index);
}

void interface_move_down(Interface* inf)
{
	if (inf->current_command_index < inf->command_count - 1)
		++(inf->current_command_index);
}