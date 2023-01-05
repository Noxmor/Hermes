#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "core/memory_system.h"

ConfigHandler* config_handler_create(void)
{
	ConfigHandler* config_handler = memory_system_malloc(sizeof(ConfigHandler), HM_MEMORY_GROUP_UNKNOWN);
	return config_handler;
}

void config_handler_load_config(ConfigHandler* config_handler)
{
	FILE* f = fopen("Hermes.cfg", "r");

	if (f == NULL)
	{
		HM_ERROR("[ConfigHandler]: Failed to open file \"Hermes.cfg\"!");

		config_handler->language_key = memory_system_malloc((strlen("english") + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(config_handler->language_key, "english");

		config_handler->game_dir = memory_system_malloc((strlen("base") + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(config_handler->game_dir, "base");

		config_handler->keybind_confirm = HM_KEY_RETURN;
		config_handler->keybind_move_up = HM_KEY_ARROW_UP;
		config_handler->keybind_move_down = HM_KEY_ARROW_DOWN;

		return;
	}

	u64 line_number = 0;

	char buffer[256];

	while (fgets(buffer, 256, f) != NULL)
	{
		++line_number;
		
		if (strlen(buffer) == 0)
			continue;

		if (buffer[0] == '\n')
			continue;

		const char* splitter = strchr(buffer, '=');

		if (splitter == NULL)
		{
			HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg:%zu: Found no '=' (Skipped line)", line_number);
			continue;
		}

		const u64 splitter_index = (u64)(splitter - buffer);
		
		if (splitter_index == 0 || splitter_index == strlen(buffer) - 1)
		{
			HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg:%zu: '=' can't be at the beginning or end of a line (Skipped line)", line_number);
			continue;
		}

		buffer[strlen(buffer) - 1] = '\0';
		buffer[splitter_index] = '\0';

		if (strcmp(buffer, "LANGUAGE") == 0)
		{
			config_handler->language_key = memory_system_malloc((strlen(buffer + splitter_index + 1) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(config_handler->language_key, buffer + splitter_index + 1);
		}
		else if (strcmp(buffer, "GAME_DIR") == 0)
		{
			config_handler->game_dir = memory_system_malloc((strlen(buffer + splitter_index + 1) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(config_handler->game_dir, buffer + splitter_index + 1);
		}
		else if (strcmp(buffer, "KEYBIND_CONFIRM") == 0)
			config_handler->keybind_confirm = atoi(buffer + splitter_index + 1);
		else if (strcmp(buffer, "KEYBIND_MOVE_UP") == 0)
			config_handler->keybind_move_up = atoi(buffer + splitter_index + 1);
		else if (strcmp(buffer, "KEYBIND_MOVE_DOWN") == 0)
			config_handler->keybind_move_down = atoi(buffer + splitter_index + 1);
		else
			HM_WARN("[ConfigHandler]: Incorrect syntax in Hermes.cfg:%zu: Unknown key \"%s\" (Skipped line)", buffer, line_number);
	}

	fclose(f);
}

void config_handler_save_config(ConfigHandler* config_handler)
{
	FILE* f = fopen("Hermes.cfg", "w");

	if (f == NULL)
	{
		HM_ERROR("[ConfigHandler]: Failed to create config file!");
		return;
	}

	HM_INFO("KEYBIND: %d", config_handler->keybind_confirm);
	HM_INFO("KEYBIND: %d", config_handler->keybind_move_up);
	HM_INFO("KEYBIND: %d", config_handler->keybind_move_down);

	fwrite("LANGUAGE=", sizeof(char), strlen("LANGUAGE="), f);
	fwrite(config_handler->language_key, sizeof(char), strlen(config_handler->language_key), f);
	fwrite("\n", sizeof(char), 1, f);

	fwrite("GAME_DIR=", sizeof(char), strlen("GAME_DIR="), f);
	fwrite(config_handler->game_dir, sizeof(char), strlen(config_handler->game_dir), f);
	fwrite("\n", sizeof(char), 1, f);

	char keybind_buffer[8];

	fwrite("KEYBIND_CONFIRM=", sizeof(char), strlen("KEYBIND_CONFIRM="), f);
	sprintf(keybind_buffer, "%d", config_handler->keybind_confirm);
	fwrite(keybind_buffer, sizeof(char), strlen(keybind_buffer), f);
	fwrite("\n", sizeof(char), 1, f);

	fwrite("KEYBIND_MOVE_UP=", sizeof(char), strlen("KEYBIND_MOVE_UP="), f);
	sprintf(keybind_buffer, "%d", config_handler->keybind_move_up); 
	fwrite(keybind_buffer, sizeof(char), strlen(keybind_buffer), f);
	fwrite("\n", sizeof(char), 1, f);

	fwrite("KEYBIND_MOVE_DOWN=", sizeof(char), strlen("KEYBIND_MOVE_DOWN="), f);
	sprintf(keybind_buffer, "%d", config_handler->keybind_move_down);
	fwrite(keybind_buffer, sizeof(char), strlen(keybind_buffer), f);

	fclose(f);
}

void config_handler_shutdown(ConfigHandler* config_handler)
{
	memory_system_free(config_handler->language_key, (strlen(config_handler->language_key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	memory_system_free(config_handler->game_dir, (strlen(config_handler->game_dir) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

	memory_system_free(config_handler, sizeof(ConfigHandler), HM_MEMORY_GROUP_UNKNOWN);
}