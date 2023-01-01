#include "application.h"

#include <stdlib.h>

Application* application_create(const char* title)
{
	Application* app = malloc(sizeof(Application));
	app->title = title;
	return app;
}

void application_run(Application* app)
{
	app->running = HM_TRUE;

	while (app->running)
	{

	}
}

void application_close(Application* app)
{
	app->running = HM_FALSE;
}