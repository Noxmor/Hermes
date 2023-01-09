#include "config.h"

#include <stdio.h>
#include <string.h>

#include "core/memory_system.h"

#include "platform/platform.h"

#include "io/serialization.h"

#define CONFIG_PATH "Hermes.cfg"

ConfigHandler* config_handler_create(void)
{
	ConfigHandler* config_handler = memory_system_malloc(sizeof(ConfigHandler), HM_MEMORY_GROUP_CONFIG_HANDLER);
	return config_handler;
}

void config_handler_load_config(ConfigHandler* config_handler)
{
	config_handler->language_key = memory_system_malloc((strlen("english") + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	strcpy(config_handler->language_key, "english");

	config_handler->game_dir = memory_system_malloc((strlen("base") + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	strcpy(config_handler->game_dir, "base");

	config_handler->keybind_confirm = HM_KEY_RETURN;
	config_handler->keybind_move_up = HM_KEY_ARROW_UP;
	config_handler->keybind_move_down = HM_KEY_ARROW_DOWN; 
	
	SerializableData* config_data = serializable_data_create_from_file(CONFIG_PATH);

	if (config_data == NULL)
	{
		HM_WARN("[ConfigHandler]: Failed to open file \"Hermes.cfg\"!");
		return;
	}

	if (strcmp(config_data->key, "config") != 0 || !serializable_data_is_parent(config_data))
	{
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Expected parent node with key \"config\"!");
		serializable_data_shutdown(config_data);
		return;
	}
	
	SerializableData* language_data = serializable_data_find(config_data, "language");
	if (language_data != NULL && serializable_data_is_child(language_data))
	{
		config_handler->language_key = memory_system_realloc(config_handler->language_key, (strlen(language_data->value) + 1) * sizeof(char), (strlen(config_handler->language_key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(config_handler->language_key, language_data->value);
	}
	else
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Found no child node with key \"language\"!");

	SerializableData* game_dir_data = serializable_data_find(config_data, "game_dir");
	if (game_dir_data != NULL && serializable_data_is_child(game_dir_data))
	{
		config_handler->game_dir = memory_system_realloc(config_handler->game_dir, (strlen(game_dir_data->value) + 1) * sizeof(char), (strlen(config_handler->game_dir) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(config_handler->game_dir, game_dir_data->value);
	}
	else
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Found no node with key \"game_dir\"!");

	SerializableData* keybind_confirm_data = serializable_data_find(config_data, "keybind_confirm");
	if (keybind_confirm_data != NULL && serializable_data_is_child(keybind_confirm_data))
	{
		const KeyCode keycode = platform_str_to_keycode(keybind_confirm_data->value);
		if (keycode != HM_KEY_UNKNOWN)
			config_handler->keybind_confirm = keycode;
	}
	else
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Found no node with key \"keybind_confirm\"!");

	SerializableData* keybind_move_up_data = serializable_data_find(config_data, "keybind_move_up");
	if (keybind_move_up_data != NULL && serializable_data_is_child(keybind_move_up_data))
	{
		const KeyCode keycode = platform_str_to_keycode(keybind_move_up_data->value);
		if (keycode != HM_KEY_UNKNOWN)
			config_handler->keybind_move_up = keycode;
	}
	else
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Found no node with key \"keybind_move_up\"!");

	SerializableData* keybind_move_down_data = serializable_data_find(config_data, "keybind_move_down");
	if (keybind_move_down_data != NULL && serializable_data_is_child(keybind_move_down_data))
	{
		const KeyCode keycode = platform_str_to_keycode(keybind_move_down_data->value);
		if (keycode != HM_KEY_UNKNOWN)
			config_handler->keybind_move_down = keycode;
	}
	else
		HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg: Found no node with key \"keybind_move_down\"!");

	serializable_data_shutdown(config_data);
}

b8 config_handler_save_config(ConfigHandler* config_handler)
{
	SerializableData* config_data = serializable_data_create("config", NULL);

	SerializableData* language_data = serializable_data_create("language", config_handler->language_key);
	serializable_data_add_child(config_data, language_data);

	SerializableData* game_dir_data = serializable_data_create("game_dir", config_handler->game_dir);
	serializable_data_add_child(config_data, game_dir_data);

	SerializableData* keybind_confirm_data = serializable_data_create("keybind_confirm", platform_keycode_to_str(config_handler->keybind_confirm));
	serializable_data_add_child(config_data, keybind_confirm_data);

	SerializableData* keybind_move_up_data = serializable_data_create("keybind_move_up", platform_keycode_to_str(config_handler->keybind_move_up));
	serializable_data_add_child(config_data, keybind_move_up_data);

	SerializableData* keybind_move_down_data = serializable_data_create("keybind_move_down", platform_keycode_to_str(config_handler->keybind_move_down));
	serializable_data_add_child(config_data, keybind_move_down_data);
	
	b8 success = serializable_data_save_to_file(config_data, CONFIG_PATH);

	serializable_data_shutdown(config_data);

	return success;
}

void config_handler_shutdown(ConfigHandler* config_handler)
{
	memory_system_free(config_handler->language_key, (strlen(config_handler->language_key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	memory_system_free(config_handler->game_dir, (strlen(config_handler->game_dir) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

	memory_system_free(config_handler, sizeof(ConfigHandler), HM_MEMORY_GROUP_CONFIG_HANDLER);
}