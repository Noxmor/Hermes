#include "core/core.h"

#include "core/memory_system.h"
#include "core/application.h"
#include "platform/platform.h"

int main(void)
{
	if (!logger_init())
		return 1; 
	
	HM_INFO("Initialized Log!");

	platform_init();
	HM_INFO("Initialized platform!");

	Application* app = application_create("Hermes");
	HM_INFO("Created application!");

	HM_INFO("Running application...");
	application_run(app);
	HM_INFO("Shut down application!");

	platform_shutdown();
	HM_INFO("Shut down platform!");

	memory_system_log();

	HM_INFO("Shutting down log...");
	logger_shutdown();
}