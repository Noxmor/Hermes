#ifndef HM_APPLICATION_H
#define HM_APPLICATION_H

#include "core/core.h"

#include "game/config.h"
#include "game/save.h"
#include "game/locale.h"
#include "game/location.h"

typedef struct Application
{
	const char* title;
	b8 running;
	ConfigHandler* config_handler;
	Savefile* savefile;
	LocaleHandler* locale_handler;
	LocationHandler* location_handler;
} Application;

Application* application_create(const char* title);

void application_run(Application* app);

void application_close(Application* app);

void application_shutdown(Application* app);

#endif