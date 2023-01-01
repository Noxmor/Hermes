#include "application.h"

#include <stdlib.h>

#include "platform/platform.h"

void application_init(Application* app);

void application_shutdown(Application* app);

Application* application_create(const char* title)
{
	Application* app = malloc(sizeof(Application));
	app->title = title;
	return app;
}

void application_run(Application* app)
{
	application_init(app);
	
	app->running = HM_TRUE;

	while (app->running)
	{
		application_close(app);
	}

	application_shutdown(app);
}

void application_close(Application* app)
{
	app->running = HM_FALSE;
}

void application_init(Application* app)
{
	platform_set_title(app->title);
}

void application_shutdown(Application* app)
{
	free(app);
}