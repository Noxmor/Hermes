#include "core/core.h"

#include "core/application.h"
#include "platform/platform.h"

int main(void)
{
	platform_init();

	if (!logger_init())
		return 1;

	HM_INFO("Initialized Log!");

	Application* app = application_create("Hermes");

	application_run(app);

	logger_shutdown();

	platform_shutdown();
}