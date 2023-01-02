#ifndef HM_COMMAND_H
#define HM_COMMAND_H

#include "core/core.h"

typedef enum CommandType
{
	HM_COMMAND_TYPE_NONE = 0,
	HM_COMMAND_TYPE_QUIT,
	HM_COMMAND_TYPE_OPTIONS,
	HM_COMMAND_TYPE_BACK
} CommandType;

typedef struct Command
{
	const char* name_id;
	CommandType type;
	b8 visible;
} Command;

#endif