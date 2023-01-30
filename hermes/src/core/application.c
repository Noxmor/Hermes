#include "application.h"

#include <string.h>
#include <stdio.h>

#include "core/memory_system.h"

#include "platform/platform.h"

static void update_main_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_MAIN_MENU"));

	interface_render_commands(&app->main_menu, 0, 0, app->locale_handler);

	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->main_menu.commands[app->main_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->main_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->main_menu);
}

static void update_play_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_PLAY_MENU"));

	interface_render_commands(&app->play_menu, 0, 0, app->locale_handler);
				
	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->play_menu.commands[app->play_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->play_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->play_menu);
}

static void update_location_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_LOCATION_MENU"));
	platform_draw_text(0, 1, locale_handler_get(app->locale_handler, app->savefile->current_location->name_id));

	interface_render_commands(&app->location_menu, 0, 0, app->locale_handler);

	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->location_menu.commands[app->location_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->location_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->location_menu);
}

static void update_travel_menu_commands(Interface* inf, Savefile* savefile)
{
	inf->current_command_index = 0;
	inf->commands = memory_system_realloc(inf->commands, sizeof(Command) * (savefile->current_location->path_count + 1), sizeof(Command) * inf->command_count, HM_MEMORY_GROUP_UNKNOWN);
	inf->command_count = savefile->current_location->path_count + 1;

	for(u8 i = 0; i < inf->command_count - 1; ++i)
	{
		inf->commands[i].name_id = savefile->current_location->paths[i].name_id;
		inf->commands[i].type = HM_COMMAND_TYPE_TRAVEL_BIT | HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	}

	inf->commands[inf->command_count - 1].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	inf->commands[inf->command_count - 1].name_id = "CMD_BACK";
}

static void update_travel_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_TRAVEL_MENU"));
	interface_render_commands(&app->travel_menu, 0, 0, app->locale_handler);
				
	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->travel_menu.commands[app->travel_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->travel_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->travel_menu);
}

static void update_options_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_OPTIONS_MENU"));

	interface_render_commands(&app->options_menu, 0, 0, app->locale_handler);
				
	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->options_menu.commands[app->options_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->options_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->options_menu);
}

static void update_language_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_LANGUAGE_MENU"));

	interface_render_commands(&app->language_menu, 0, 0, app->locale_handler);
				
	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->language_menu.commands[app->language_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->language_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->language_menu);	
}

static void update_controls_menu(Application* app)
{
	platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "INF_CONTROLS_MENU"));

	interface_render_commands(&app->controls_menu, 0, 0, app->locale_handler);
				
	platform_flush();
	platform_clear_screen();

	const KeyCode input_keycode = platform_get_next_key();

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	if (input_keycode == HM_KEY_ESCAPE)
		app->current_command_type = HM_COMMAND_TYPE_QUIT_BIT;

	if (input_keycode == app->config_handler->keybind_confirm)
		app->current_command_type = app->controls_menu.commands[app->controls_menu.current_command_index].type;

	if (input_keycode == app->config_handler->keybind_move_up)
		interface_move_up(&app->controls_menu);

	if (input_keycode == app->config_handler->keybind_move_down)
		interface_move_down(&app->controls_menu);
}

static void update_battle_menu(Application* app)
{
	//TODO: Implement
}

static void update_dialogue_menu(Application* app)
{
	//TODO: Implement
}

static void update_inventory_menu(Application* app)
{
	//TODO: Implement
}

static void update_quest_menu(Application* app)
{
	//TODO: Implement
}

Application* application_create(const char* title)
{
	Application* app = memory_system_malloc(sizeof(Application), HM_MEMORY_GROUP_APPLICATION);
	app->title = title;

	platform_set_title(app->title);

	app->config_handler = config_handler_create();
	config_handler_load_config(app->config_handler);

	app->savefile = NULL;

	app->locale_handler = locale_handler_create(1024);
	locale_handler_load_languages(app->locale_handler, app->config_handler->game_dir, app->config_handler->language_key);
	locale_handler_load_locales(app->locale_handler, app->config_handler->game_dir, app->config_handler->language_key);

	app->location_handler = location_handler_create();
	location_handler_load_locations(app->location_handler, app->config_handler->game_dir);

	memset(app->update_func, 0, HM_GAMESTATE_SIZE * sizeof(void (*)(Application*)));
	app->update_func[HM_GAMESTATE_MAIN_MENU] = update_main_menu;
	app->update_func[HM_GAMESTATE_PLAY_MENU] = update_play_menu;
	app->update_func[HM_GAMESTATE_LOCATION_MENU] = update_location_menu;
	app->update_func[HM_GAMESTATE_TRAVEL_MENU] = update_travel_menu;
	app->update_func[HM_GAMESTATE_OPTIONS_MENU] = update_options_menu;
	app->update_func[HM_GAMESTATE_LANGUAGE_MENU] = update_language_menu;
	app->update_func[HM_GAMESTATE_CONTROLS_MENU] = update_controls_menu;
	app->update_func[HM_GAMESTATE_BATTLE_MENU] = update_battle_menu;
	app->update_func[HM_GAMESTATE_DIALOGUE_MENU] = update_dialogue_menu;
	app->update_func[HM_GAMESTATE_INVENTORY_MENU] = update_inventory_menu;
	app->update_func[HM_GAMESTATE_QUEST_MENU] = update_quest_menu;

	for(u64 i = 0; i < HM_GAMESTATE_SIZE; ++i)
		HM_ASSERT(app->update_func[i] != NULL);

	app->main_menu.command_count = 3;
	app->main_menu.commands = memory_system_malloc(sizeof(Command) * app->main_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->main_menu.commands[0].name_id = "CMD_PLAY";
	app->main_menu.commands[0].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_PLAY_BIT;
	app->main_menu.commands[1].name_id = "CMD_OPTIONS";
	app->main_menu.commands[1].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_OPTIONS_BIT;
	app->main_menu.commands[2].name_id = "CMD_QUIT";
	app->main_menu.commands[2].type = HM_COMMAND_TYPE_QUIT_BIT;
	app->main_menu.current_command_index = 0;

	app->play_menu.command_count = 3;
	app->play_menu.commands = memory_system_malloc(sizeof(Command) * app->play_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->play_menu.commands[0].name_id = "CMD_NEW_GAME";
	app->play_menu.commands[0].type = HM_COMMAND_TYPE_CREATE_NEW_GAME_BIT;
	app->play_menu.commands[1].name_id = "CMD_LOAD_GAME";
	app->play_menu.commands[1].type = HM_COMMAND_TYPE_LOAD_GAME_BIT;
	app->play_menu.commands[2].name_id = "CMD_BACK";
	app->play_menu.commands[2].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	app->play_menu.current_command_index = 0;

	app->location_menu.command_count = 7;
	app->location_menu.commands = memory_system_malloc(sizeof(Command) * app->location_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->location_menu.commands[0].name_id = "CMD_TRAVEL";
	app->location_menu.commands[0].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_TRAVEL_BIT;
	app->location_menu.commands[1].name_id = "CMD_TALK";
	app->location_menu.commands[1].type = HM_COMMAND_TYPE_NONE;
	app->location_menu.commands[2].name_id = "CMD_SEARCH";
	app->location_menu.commands[2].type = HM_COMMAND_TYPE_NONE;
	app->location_menu.commands[3].name_id = "CMD_OPEN_INVENTORY";
	app->location_menu.commands[3].type = HM_COMMAND_TYPE_NONE;
	app->location_menu.commands[4].name_id = "CMD_QUESTS";
	app->location_menu.commands[4].type = HM_COMMAND_TYPE_NONE;
	app->location_menu.commands[5].name_id = "CMD_OPTIONS";
	app->location_menu.commands[5].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_OPTIONS_BIT;
	app->location_menu.commands[6].name_id = "CMD_QUIT";
	app->location_menu.commands[6].type = HM_COMMAND_TYPE_QUIT_BIT;
	app->location_menu.current_command_index = 0;

	app->travel_menu.command_count = 1;
	app->travel_menu.commands = memory_system_malloc(sizeof(Command) * app->travel_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->travel_menu.commands[0].name_id = "CMD_BACK";
	app->travel_menu.commands[0].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	app->travel_menu.current_command_index = 0;

	app->options_menu.command_count = 3;
	app->options_menu.commands = memory_system_malloc(sizeof(Command) * app->options_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->options_menu.commands[0].name_id = "CMD_LANGUAGE";
	app->options_menu.commands[0].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_LANGUAGE_BIT;
	app->options_menu.commands[1].name_id = "CMD_CONTROLS";
	app->options_menu.commands[1].type = HM_COMMAND_TYPE_PUSH_GAMESTATE_CONTROLS_BIT;
	app->options_menu.commands[2].name_id = "CMD_BACK";
	app->options_menu.commands[2].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	app->options_menu.current_command_index = 0;

	app->language_menu.command_count = app->locale_handler->language_count + 1;
	app->language_menu.commands = memory_system_malloc(sizeof(Command) * app->language_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	for (u8 i = 0; i < app->locale_handler->language_count; ++i)
	{
		app->language_menu.commands[i].name_id = app->locale_handler->languages[i]->value;
		app->language_menu.commands[i].type = HM_COMMAND_TYPE_CHANGE_LANGUAGE_BIT;
	}
	app->language_menu.commands[app->language_menu.command_count - 1].name_id = "CMD_BACK";
	app->language_menu.commands[app->language_menu.command_count - 1].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	app->language_menu.current_command_index = 0;

	app->controls_menu.command_count = 4;
	app->controls_menu.commands = memory_system_malloc(sizeof(Command) * app->controls_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->controls_menu.commands[0].name_id = "KEYBIND_CONFIRM";
	app->controls_menu.commands[0].type = HM_COMMAND_TYPE_SET_KEYBIND_BIT;
	app->controls_menu.commands[1].name_id = "KEYBIND_MOVE_UP";
	app->controls_menu.commands[1].type = HM_COMMAND_TYPE_SET_KEYBIND_BIT;
	app->controls_menu.commands[2].name_id = "KEYBIND_MOVE_DOWN";
	app->controls_menu.commands[2].type = HM_COMMAND_TYPE_SET_KEYBIND_BIT;
	app->controls_menu.commands[3].name_id = "CMD_BACK";
	app->controls_menu.commands[3].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
	app->controls_menu.current_command_index = 0;

	app->yes_no_menu.command_count = 2;
	app->yes_no_menu.commands = memory_system_malloc(sizeof(Command) * app->yes_no_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	app->yes_no_menu.commands[0].name_id = "CMD_YES";
	app->yes_no_menu.commands[0].type = HM_COMMAND_TYPE_NONE;
	app->yes_no_menu.commands[1].name_id = "CMD_NO";
	app->yes_no_menu.commands[1].type = HM_COMMAND_TYPE_NONE;

	app->current_command_type = HM_COMMAND_TYPE_NONE;

	return app;
}

void application_run(Application* app)
{
	app->running = HM_TRUE;

	/*
	* TODO:
	* Current design flow does not allow to first pop a state, then push a state and finally push another state.
	* Maybe make the command struct "abstract" and only store a enum type and a void* to the specific type, whereas
	* each specific command gets its own struct with the data needed. Maybe store this as an array for allowing multiple commands
	* to run at once, so first pop a state, push a state, push another state and so on
	*/

#define STATE_STACK_SIZE 64
	GameState state_stack[STATE_STACK_SIZE];
	state_stack[0] = HM_GAMESTATE_MAIN_MENU;
	GameState* current_state = state_stack;

	while (app->running)
	{
		if(app->current_command_type & HM_COMMAND_TYPE_QUIT_BIT)
		{
			application_close(app);
			continue;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_PLAY_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_PLAY_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_CREATE_NEW_GAME_BIT)
		{
			platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "MISC_NEW_SAVEFILE"));
			platform_draw_text(0, 2, "> ");
			platform_set_cursor_pos(2, 2);

			platform_flush();
			platform_clear_screen();

			char buffer[256];

			//TODO: Handle invalid input for savefile names
			while (fgets(buffer, 256 - (strlen("saves/.save")), stdin) == NULL || (strlen(buffer) == 1 && buffer[0] == '\n'));

			if (buffer[strlen(buffer) - 1] == '\n')
				buffer[strlen(buffer) - 1] = '\0';

			char* path = memory_system_malloc((strlen("saves/.save") + strlen(buffer) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			sprintf(path, "saves/%s.save", buffer);

			b8 create_savefile = HM_TRUE;

			FILE* f = fopen(path, "r");
			if (f != NULL)
			{
				fclose(f);

				app->yes_no_menu.current_command_index = 1;

				while(HM_TRUE)
				{
					platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "MISC_OVERWRITE_SAVEFILE_POPUP"));
					interface_render_commands(&app->yes_no_menu, 0, 0, app->locale_handler);

					platform_flush();
					platform_clear_screen();
					
					const KeyCode input_keycode = platform_get_next_key();

					if (input_keycode == app->config_handler->keybind_confirm)
					{
						if(app->yes_no_menu.current_command_index == 1)
							create_savefile = HM_FALSE;
						
						break;
					}

					if (input_keycode == app->config_handler->keybind_move_up)
						interface_move_up(&app->yes_no_menu);

					if (input_keycode == app->config_handler->keybind_move_down)
						interface_move_down(&app->yes_no_menu);
				}
			}

			if(create_savefile)
			{
				if (app->savefile != NULL)
				{
					platform_create_dir("saves");

					if (!savefile_save(app->savefile))
						HM_ERROR("[Application]: Failed to save savefile \"%s\"!", app->savefile->path);

					savefile_shutdown(app->savefile);
				}

				app->savefile = savefile_create(path);
				//TODO: Remove hard coded location id
				//TODO: Load a "new.txt" file which contains all starting info for a new savefile.
				app->savefile->current_location = location_handler_get_location_by_name_id(app->location_handler, "LOC_START");

				update_travel_menu_commands(&app->travel_menu, app->savefile);

				++current_state;
				*current_state = HM_GAMESTATE_LOCATION_MENU;
			}

			memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

			platform_flush();
			platform_clear_screen();
		}

		if(app->current_command_type & HM_COMMAND_TYPE_LOAD_GAME_BIT)
		{
			//TODO: Implement
		}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_LOCATION_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_LOCATION_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_TRAVEL_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_TRAVEL_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_TRAVEL_BIT)
		{
			app->savefile->current_location = location_handler_get_location_by_name_id(app->location_handler, app->travel_menu.commands[app->travel_menu.current_command_index].name_id);
			update_travel_menu_commands(&app->travel_menu, app->savefile);
		}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_OPTIONS_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_OPTIONS_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_LANGUAGE_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_LANGUAGE_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_CHANGE_LANGUAGE_BIT)
		{
				b8 language_changed = HM_TRUE;

				for (u8 i = 0; i < app->locale_handler->language_count; ++i)
				{
					if (strcmp(app->language_menu.commands[app->language_menu.current_command_index].name_id, app->locale_handler->languages[i]->value) == 0)
					{
						if (strcmp(app->locale_handler->languages[i]->key, app->config_handler->language_key) == 0)
						{
							HM_WARN("Tried to change language from \"%s\" to \"%s\"! (Ignored)", app->config_handler->language_key, app->locale_handler->languages[i]->key);
							language_changed = HM_FALSE;
							break;
						}

						app->config_handler->language_key = memory_system_realloc(app->config_handler->language_key, (strlen(app->locale_handler->languages[i]->key) + 1) * sizeof(char), (strlen(app->config_handler->language_key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
						strcpy(app->config_handler->language_key, app->locale_handler->languages[i]->key);
						HM_TRACE("Changed current language to: \"%s\"", app->config_handler->language_key);
						break;
					}
				}

				if (language_changed)
				{
					LocaleHandler* locale_handler_new = locale_handler_create(app->locale_handler->size);

					locale_handler_load_languages(locale_handler_new, app->config_handler->game_dir, app->config_handler->language_key);
					locale_handler_load_locales(locale_handler_new, app->config_handler->game_dir, app->config_handler->language_key);

					locale_handler_shutdown(app->locale_handler);
					app->locale_handler = locale_handler_new;

					memory_system_free(app->language_menu.commands, app->language_menu.command_count * sizeof(Command), HM_MEMORY_GROUP_UNKNOWN);

					app->language_menu.command_count = app->locale_handler->language_count + 1;

					app->language_menu.commands = memory_system_malloc(app->language_menu.command_count * sizeof(Command), HM_MEMORY_GROUP_UNKNOWN);

					for (u8 i = 0; i < app->locale_handler->language_count; ++i)
					{
						app->language_menu.commands[i].name_id = app->locale_handler->languages[i]->value;
						app->language_menu.commands[i].type = HM_COMMAND_TYPE_CHANGE_LANGUAGE_BIT;
					}

					app->language_menu.commands[app->language_menu.command_count - 1].name_id = "CMD_BACK";
					app->language_menu.commands[app->language_menu.command_count - 1].type = HM_COMMAND_TYPE_POP_GAMESTATE_BIT;
				}				
			}

		if(app->current_command_type & HM_COMMAND_TYPE_PUSH_GAMESTATE_CONTROLS_BIT)
		{
			HM_ASSERT(current_state != &state_stack[STATE_STACK_SIZE - 1]);
			
			++current_state;
			*current_state = HM_GAMESTATE_CONTROLS_MENU;
		}

		if(app->current_command_type & HM_COMMAND_TYPE_SET_KEYBIND_BIT)
		{
			const char* keybind_to_change = app->controls_menu.commands[app->controls_menu.current_command_index].name_id;

			platform_draw_text(0, 0, locale_handler_get(app->locale_handler, "MISC_SET_KEYBIND"));
			platform_draw_text(strlen(locale_handler_get(app->locale_handler, "MISC_SET_KEYBIND")), 0, locale_handler_get(app->locale_handler, keybind_to_change));
				
			platform_set_cursor_pos(0, 2);
				
			platform_flush();
			platform_clear_screen();
				
			const KeyCode keybind = platform_get_next_key();
				
			if (strcmp(keybind_to_change, "KEYBIND_CONFIRM") == 0)
				app->config_handler->keybind_confirm = keybind;
			else if (strcmp(keybind_to_change, "KEYBIND_MOVE_UP") == 0)
				app->config_handler->keybind_move_up = keybind;
			else if (strcmp(keybind_to_change, "KEYBIND_MOVE_DOWN") == 0)
				app->config_handler->keybind_move_down = keybind;
			else
				HM_WARN("[Keybind Menu]: Invalid keybind specified!");
		}

		if(app->current_command_type & HM_COMMAND_TYPE_POP_GAMESTATE_BIT)
		{
			HM_ASSERT(current_state != &state_stack[0]);

			--current_state;
		}

		app->update_func[*current_state](app);
	}
}

void application_close(Application* app)
{
	app->running = HM_FALSE;
}

void application_shutdown(Application* app)
{
	location_handler_shutdown(app->location_handler);

	if (app->savefile != NULL)
	{
		platform_create_dir("saves");
		
		if (!savefile_save(app->savefile))
			HM_ERROR("[Application]: Failed to save savefile \"%s\"!", app->savefile->path);

		savefile_shutdown(app->savefile);
	}

	locale_handler_shutdown(app->locale_handler);

	if (!config_handler_save_config(app->config_handler))
		HM_ERROR("[Application]: Failed to save config file!");

	config_handler_shutdown(app->config_handler);

	memory_system_free(app->main_menu.commands, sizeof(Command) * app->main_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->play_menu.commands, sizeof(Command) * app->play_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->location_menu.commands, sizeof(Command) * app->location_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->travel_menu.commands, sizeof(Command) * app->travel_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->options_menu.commands, sizeof(Command) * app->options_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->language_menu.commands, sizeof(Command) * app->language_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->controls_menu.commands, sizeof(Command) * app->controls_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(app->yes_no_menu.commands, sizeof(Command) * app->yes_no_menu.command_count, HM_MEMORY_GROUP_UNKNOWN);

	memory_system_free(app, sizeof(Application), HM_MEMORY_GROUP_APPLICATION);
}