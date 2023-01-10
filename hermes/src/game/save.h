#ifndef HM_SAVE_H
#define HM_SAVE_H

#include "core/core.h"

typedef struct Savefile
{
	char* path;

} Savefile;

Savefile* savefile_create(const char* path);

Savefile* savefile_create_from_file(const char* path);

b8 savefile_save(Savefile* savefile);

void savefile_shutdown(Savefile* savefile);

#endif