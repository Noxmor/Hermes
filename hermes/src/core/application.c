#include "application.h"

#include <stdlib.h>

#include "platform/platform.h"

#include <conio.h>

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

	u8 current_command_index = 0;
	const char* commands[3] = {
		"CMD_START",
		"CMD_OPTIONS",
		"CMD_QUIT"
	};

	while (app->running)
	{
		//Test code
		platform_draw_text(0, 0, "This is the main menu of Hermes.\n", HM_COLOR_FOREGROUND_RED | HM_COLOR_FOREGROUND_GREEN | HM_COLOR_FOREGROUND_BLUE);
		platform_draw_text(0, 1, "Please select any of the commands below to proceed\n", HM_COLOR_LAST);

		for (u64 i = 0; i < 3; ++i)
		{
			Color color = HM_COLOR_FOREGROUND_RED | HM_COLOR_FOREGROUND_GREEN | HM_COLOR_FOREGROUND_BLUE;

			if (i == current_command_index)
				color = HM_COLOR_FOREGROUND_RED;
			
			platform_draw_text(0, 3 + i, commands[i], color);
		}

		platform_flush();

		switch ((char)getch())
		{
			case ' ': application_close(app); break;
			case 'w':
			case 'W': if (current_command_index > 0) current_command_index--; break;
			case 's':
			case 'S': if (current_command_index < 3 - 1) current_command_index++; break;
		}
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