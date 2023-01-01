#ifndef HM_APPLICATION_H
#define HM_APPLICATION_H

#include "core/core.h"

typedef struct Application
{
	const char* title;
	b8 running;
} Application;

Application* application_create(const char* title);

void application_run(Application* app);

void application_close(Application* app);

#endif