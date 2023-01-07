#ifndef HM_LOCATION_H
#define HM_LOCATION_H

#include "core/core.h"

typedef struct LocationPath
{
	char* name_id;
	b8 locked;
	b8 visible;
} LocationPath;

typedef struct Location
{
	char* name_id;
	LocationPath* paths;
	u64 path_count;
} Location;

typedef struct LocationHandler
{
	Location* locations;
	u64 location_count;
} LocationHandler;

LocationHandler* location_handler_create(void);

void location_handler_load_locations(LocationHandler* location_handler, const char* game_dir);

void location_handler_shutdown(LocationHandler* location_handler);

#endif