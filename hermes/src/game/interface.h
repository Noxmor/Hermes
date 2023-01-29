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

void interface_render_commands(Interface* inf, u64 x, u64 y, LocaleHandler* locale_handler);

void interface_move_up(Interface* inf);

void interface_move_down(Interface* inf);

#endif