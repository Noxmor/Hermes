#ifndef HM_LOG_H
#define HM_LOG_H

#include "core/types.h"

typedef enum LogLevel
{
	HM_LOG_LEVEL_FATAL = 0,
	HM_LOG_LEVEL_ERROR,
	HM_LOG_LEVEL_WARN,
	HM_LOG_LEVEL_INFO,
	HM_LOG_LEVEL_TRACE,

	HM_LOG_LEVEL_SIZE
} LogLevel;

b8 logger_init(void);

void logger_log(LogLevel level, const char* message, ...);

void logger_shutdown(void);

#define HM_FATAL(...) logger_log(HM_LOG_LEVEL_FATAL, __VA_ARGS__)
#define HM_ERROR(...) logger_log(HM_LOG_LEVEL_ERROR, __VA_ARGS__)
#define HM_WARN(...) logger_log(HM_LOG_LEVEL_WARN, __VA_ARGS__)
#define HM_INFO(...) logger_log(HM_LOG_LEVEL_INFO, __VA_ARGS__)
#define HM_TRACE(...) logger_log(HM_LOG_LEVEL_TRACE, __VA_ARGS__)

#endif