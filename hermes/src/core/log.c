#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "core/core.h"

typedef struct Logger
{
	FILE* log;
	FILE* error_log;
} Logger;

static Logger logger;

b8 logger_init(void)
{
	logger.log = fopen("Hermes.log", "wb");

	if (logger.log == NULL)
		return HM_FALSE;

	logger.error_log = fopen("error.log", "wb");

	if (logger.error_log == NULL)
		return HM_FALSE;

	return HM_TRUE;
}

void logger_log(LogLevel level, const char* message, ...)
{
	__builtin_va_list args = 0;
	va_start(args, message);

	const u64 len = vsnprintf(NULL, 0, message, args);
	char* formatted_message = malloc((len + 1) * sizeof(char));
	vsprintf(formatted_message, message, args);
	va_end(args);

	time_t now = time(0);
	struct tm* time_info = localtime(&now);

	static const char* level_str[HM_LOG_LEVEL_SIZE] = { "FATAL", "ERROR", "WARN", "INFO", "TRACE" };

	if (logger.log != NULL)
	{
		fprintf(logger.log, "[%02d:%02d:%02d] (%s): %s\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, level_str[level], formatted_message);

		if (ferror(logger.log))
		{
			fclose(logger.log);
			logger.log = NULL;
		}
	}

	if (level < HM_LOG_LEVEL_INFO && logger.error_log != NULL)
	{
		fprintf(logger.error_log, "[%02d:%02d:%02d] (%s): %s\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, level_str[level], formatted_message);

		if (ferror(logger.error_log))
		{
			fclose(logger.error_log);
			logger.error_log = NULL;
		}
		else
		{
			fflush(logger.log);
			fflush(logger.error_log);
		}
	}

	free(formatted_message);
}

void logger_shutdown(void)
{
	if (logger.log != NULL)
		fclose(logger.log);

	if (logger.error_log != NULL)
	{
		fseek(logger.error_log, 0, SEEK_END);
		const b8 empty = ftell(logger.error_log) == 0;

		fclose(logger.error_log);

		if (empty)
			remove("error.log");
	}
}