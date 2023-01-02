#ifndef HM_INTERFACE_H
#define HM_INTERFACE_H

#include "game/command.h"
#include "game/locale.h"

typedef struct Interface
{
	Command* commands;
	u8 command_count;
	u8 current_command_index;
} Interface;

void interface_render_commands(Interface* inf, LocaleHandler* locale_handler);

#endif