#ifndef HM_CONFIG_H
#define HM_CONFIG_H

#include "platform/platform.h"

typedef struct ConfigHandler
{
	char* language_key;
	char* game_dir;
	KeyCode keybind_confirm;
	KeyCode keybind_move_up;
	KeyCode keybind_move_down;

} ConfigHandler;

ConfigHandler* config_handler_create(void);

void config_handler_load_config(ConfigHandler* config_handler);

b8 config_handler_save_config(ConfigHandler* config_handler);

void config_handler_shutdown(ConfigHandler* config_handler);

#endif