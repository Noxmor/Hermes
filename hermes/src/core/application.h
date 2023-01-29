#ifndef HM_APPLICATION_H
#define HM_APPLICATION_H

#include "core/core.h"

#include "game/config.h"
#include "game/save.h"
#include "game/locale.h"
#include "game/interface.h"
#include "game/location.h"

typedef enum GameState
{
	HM_GAMESTATE_MAIN_MENU = 0,
	HM_GAMESTATE_PLAY_MENU,
	HM_GAMESTATE_LOCATION_MENU,
	HM_GAMESTATE_TRAVEL_MENU,
	HM_GAMESTATE_OPTIONS_MENU,
	HM_GAMESTATE_LANGUAGE_MENU,
	HM_GAMESTATE_CONTROLS_MENU,
	HM_GAMESTATE_BATTLE_MENU,
	HM_GAMESTATE_DIALOGUE_MENU,
	HM_GAMESTATE_INVENTORY_MENU,
	HM_GAMESTATE_QUEST_MENU,

	HM_GAMESTATE_SIZE
} GameState;

typedef struct Application
{
	const char* title;
	b8 running;

	ConfigHandler* config_handler;
	Savefile* savefile;
	LocaleHandler* locale_handler;
	LocationHandler* location_handler;

	void (*update_func[HM_GAMESTATE_SIZE])(struct Application*);

	Interface main_menu;
	Interface play_menu;
	Interface location_menu;
	Interface travel_menu;
	Interface options_menu;
	Interface language_menu;
	Interface controls_menu;
	Interface yes_no_menu;

	CommandType current_command_type;
} Application;

Application* application_create(const char* title);

void application_run(Application* app);

void application_close(Application* app);

void application_shutdown(Application* app);

#endif