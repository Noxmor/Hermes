#include "save.h"

#include <string.h>

#include "core/memory_system.h"

#include "io/serialization.h"

Savefile* savefile_create(const char* path)
{
	Savefile* savefile = memory_system_malloc(sizeof(Savefile), HM_MEMORY_GROUP_SAVEFILE);
	savefile->path = memory_system_malloc((strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	strcpy(savefile->path, path);

	return savefile;
}

Savefile* savefile_create_from_file(const char* path)
{
	Savefile* savefile = savefile_create(path);

	//TODO: Implement
	HM_ASSERT(HM_FALSE);

	return savefile;
}

b8 savefile_save(Savefile* savefile)
{
	SerializableData* savefile_data = serializable_data_create("save", NULL);

	serializable_data_add_child(savefile_data, serializable_data_create("location", savefile->current_location->name_id));

	const b8 success = serializable_data_save_to_file(savefile_data, savefile->path);

	serializable_data_shutdown(savefile_data);

	return success;
}

void savefile_shutdown(Savefile* savefile)
{
	memory_system_free(savefile->path, (strlen(savefile->path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

	memory_system_free(savefile, sizeof(Savefile), HM_MEMORY_GROUP_SAVEFILE);
}