#include "location.h"

#include <stdio.h>
#include <string.h>

#include "core/memory_system.h"

#include "platform/platform.h"

#include "io/serialization.h"

LocationHandler* location_handler_create(void)
{
	LocationHandler* location_handler = memory_system_malloc(sizeof(LocationHandler), HM_MEMORY_GROUP_UNKNOWN);
	location_handler->location_count = 0;
	location_handler->locations = NULL;
	return location_handler;
}

//void load_location_path(Location* location, const char* path, FILE* f, u64* line_number)
//{
//	++(location->path_count);
//	
//	location->paths = memory_system_realloc(location->paths, location->path_count * sizeof(LocationPath), (location->path_count - 1) * sizeof(LocationPath), HM_MEMORY_GROUP_UNKNOWN);
//
//	LocationPath* location_path = location->paths + (location->path_count - 1);
//	location_path->locked = HM_FALSE;
//	location_path->visible = HM_TRUE;
//
//	char buffer[256];
//
//	b8 found_id = HM_FALSE;
//
//	while (fgets(buffer, 256, f) != NULL)
//	{
//		++(*line_number);
//
//		if (strlen(buffer) == 0)
//			continue;
//
//		if (buffer[0] == '\n')
//			continue;
//
//		if (buffer[0] != ' ')
//			break; //FIXME: This will lead to multiple paths not be recognized, only the first one.
//
//		if (buffer[strlen(buffer) - 1] == '\n')
//			buffer[strlen(buffer) - 1] = '\0';
//
//		const char* splitter = strchr(buffer, '=');
//
//		if (splitter == NULL)
//		{
//			HM_WARN("[LocationHandler]: Incorrect syntax in %s:%zu: Found no '=' (Skipped line)", path, line_number);
//			continue;
//		}
//
//		const u64 splitter_index = (u64)(splitter - buffer);
//
//		if (splitter_index == 1 || splitter_index == strlen(buffer) - 1)
//		{
//			HM_WARN("[LocationHandler]: Incorrect syntax in %s:%zu: '=' can't be at the beginning or end of a line (Skipped line)", path, line_number);
//			continue;
//		}
//
//		buffer[splitter_index] = '\0';
//
//		if (strcmp(buffer, " id") == 0)
//		{
//			found_id = HM_TRUE;
//			location_path->name_id = memory_system_malloc((strlen(buffer + splitter_index + 1) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
//			strcpy(location_path->name_id, buffer + splitter_index + 1);
//		}
//		else if (strcmp(buffer, " locked") == 0)
//		{
//			if (strcmp(buffer + splitter_index + 1, "yes") == 0)
//				location_path->locked = HM_TRUE;
//			else if (strcmp(buffer + splitter_index + 1, "no") == 0)
//				location_path->locked = HM_FALSE;
//			else
//				HM_ERROR("[LocationHandler]: Incorrect syntax in %s:%zu: Value must either be \"yes\" or \"no\"", path, line_number);
//		}
//		else if (strcmp(buffer, " visible") == 0)
//		{
//			if (strcmp(buffer + splitter_index + 1, "yes") == 0)
//				location_path->visible = HM_TRUE;
//			else if (strcmp(buffer + splitter_index + 1, "no") == 0)
//				location_path->visible = HM_FALSE;
//			else
//				HM_ERROR("[LocationHandler]: Incorrect syntax in %s:%zu: Value must either be \"yes\" or \"no\"", path, line_number);
//		}
//		else
//			HM_WARN("[LocationHandler]: Incorrect syntax in %s:%zu: Unknown key (Skipped Line)", path, line_number);
//	}
//
//	HM_ASSERT(found_id);
//}
//
//void load_location(Location* location, const char* path)
//{
//	location->paths = NULL;
//	location->path_count = 0;
//
//	FILE* f = fopen(path, "r");
//
//	if (f == NULL)
//	{
//		HM_ERROR("[LocationHandler]: Failed to open file \"%s\"!", path);
//		return;
//	}
//
//	u64 line_number = 0;
//
//	char buffer[256];
//
//	while (fgets(buffer, 256, f) != NULL)
//	{
//		++line_number;
//
//		if (strlen(buffer) == 0)
//			continue;
//
//		if (buffer[0] == '\n')
//			continue;
//
//		if (buffer[strlen(buffer) - 1] == '\n')
//			buffer[strlen(buffer) - 1] = '\0';
//
//		const char* splitter = strchr(buffer, '=');
//
//		if (splitter == NULL)
//		{
//			if (strcmp(buffer, "path:") == 0)
//			{
//				load_location_path(location, path, f, &line_number);
//
//				continue;
//			}
//			
//			HM_WARN("[LocationHandler]: Incorrect syntax in %s:%zu: Found no '=' (Skipped line)", path, line_number);
//			continue;
//		}
//
//		const u64 splitter_index = (u64)(splitter - buffer);
//
//		if (splitter_index == 0 || splitter_index == strlen(buffer) - 1)
//		{
//			HM_WARN("[LocaleHandler]: Incorrect syntax in locale/languages.txt:%zu: '=' can't be at the beginning or end of a line (Skipped line)", line_number);
//			continue;
//		}
//
//		buffer[splitter_index] = '\0';
//
//		if (strcmp(buffer, "id") == 0)
//		{
//			location->name_id = memory_system_malloc((strlen(buffer) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
//			strcpy(location->name_id, buffer);
//		}
//		else
//			HM_WARN("[LocationHandler]: Incorrect syntax in %s:%zu: Unknown key (Skipped line)", path, line_number);
//	}
//
//	fclose(f);
//}
//
//void location_handler_load_locations(LocationHandler* location_handler, const char* game_dir)
//{	
//	char* path = memory_system_calloc(strlen(game_dir) + strlen("/map/*.*") + 1, sizeof(char), HM_MEMORY_GROUP_STRING);
//	sprintf(path, "%s/map/*.*", game_dir);
//	
//	char** location_files = platform_get_files_in_dir(path, &location_handler->location_count);
//
//	location_handler->locations = memory_system_malloc(location_handler->location_count * sizeof(Location), HM_MEMORY_GROUP_UNKNOWN);
//
//	for (u64 i = 0; i < location_handler->location_count; ++i)
//	{
//		path = memory_system_realloc(path, (strlen(game_dir) + strlen("/map/") + strlen(location_files[i]) + 1) * sizeof(char), (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
//		sprintf(path, "%s/map/%s", game_dir, location_files[i]);
//		load_location(location_handler->locations + i, path);
//	}
//
//	for (u64 i = 0; i < location_handler->location_count; ++i)
//		memory_system_free(location_files[i], (strlen(location_files[i]) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
//
//	memory_system_free(location_files, location_handler->location_count * sizeof(char*), HM_MEMORY_GROUP_STRING);
//
//	memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
//}

b8 load_location_from_file(Location* location, const char* path)
{
	location->name_id = NULL;
	location->paths = NULL;
	location->path_count = 0;

	SerializableData* location_data = serializable_data_create_from_file(path);

	if (strcmp(location_data->key, "location") != 0)
	{
		HM_ERROR("[LocationHandler]: Incorrect syntax in %s: Expected key \"location\" at the root node!");
		serializable_data_shutdown(location_data);
		return HM_FALSE;
	}

	SerializableData* location_id_data = serializable_data_find(location_data, "id");
	if (location_id_data == NULL)
	{
		HM_ERROR("[LocationHandler]: Incorrect syntax in %s: Node with key \"id\" was not found!");
		serializable_data_shutdown(location_data);
		return HM_FALSE;
	}

	location->name_id = memory_system_malloc((strlen(location_id_data->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	strcpy(location->name_id, location_id_data->value);

	for (u64 i = 0; i < location_data->children_count; ++i)
	{
		SerializableData* child = location_data->children[i];

		if (strcmp(child->key, "path") != 0)
			continue;

		SerializableData* path_data = child;

		SerializableData* path_id_data = serializable_data_find(path_data, "id");
		if (path_id_data == NULL)
		{
			HM_ERROR("[LocationHandler]: Incorrect syntax in %s: Path node has no child with key \"id\"! (Skipped path)");
			continue;
		}

		++(location->path_count);
		location->paths = memory_system_realloc(location->paths, location->path_count * sizeof(LocationPath), (location->path_count - 1) * sizeof(LocationPath), HM_MEMORY_GROUP_UNKNOWN);
		LocationPath* location_path = &location->paths[location->path_count - 1];
		location_path->locked = HM_FALSE;
		location_path->visible = HM_TRUE;
		location_path->name_id = memory_system_malloc((strlen(path_id_data->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(location_path->name_id, path_id_data->value);

		SerializableData* path_locked_data = serializable_data_find(path_data, "locked");
		if (path_locked_data != NULL)
		{
			if (strcmp(path_locked_data->value, "yes") == 0)
				location_path->locked = HM_TRUE;
			else
			{
				if(strcmp(path_locked_data->value, "no") != 0)
					HM_WARN("[LocationHandler]: Incorrect syntax in %s: Path child node with key \"locked\" has invalid value!");
			}
		}

		SerializableData* path_visible_data = serializable_data_find(path_data, "visible");
		if (path_visible_data != NULL)
		{
			if (strcmp(path_visible_data->value, "no") == 0)
			location_path->visible = HM_FALSE;
			else
			{
				if (strcmp(path_visible_data->value, "yes") != 0)
					HM_WARN("[LocationHandler]: Incorrect syntax in %s: Path child node with key \"visible\" has invalid value!");
			}
		}
	}

	serializable_data_shutdown(location_data);

	return HM_TRUE;
}

void location_handler_load_locations(LocationHandler* location_handler, const char* game_dir)
{	
	char* path = memory_system_calloc(strlen(game_dir) + strlen("/map/*.txt") + 1, sizeof(char), HM_MEMORY_GROUP_STRING);
	sprintf(path, "%s/map/*.txt", game_dir);
	
	char** location_files = platform_get_files_in_dir(path, &location_handler->location_count);

	location_handler->locations = memory_system_malloc(location_handler->location_count * sizeof(Location), HM_MEMORY_GROUP_UNKNOWN);

	for (u64 i = 0; i < location_handler->location_count; ++i)
	{
		path = memory_system_realloc(path, (strlen(game_dir) + strlen("/map/") + strlen(location_files[i]) + 1) * sizeof(char), (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		sprintf(path, "%s/map/%s", game_dir, location_files[i]); 
		
		if (!load_location_from_file(location_handler->locations + i, path))
		{
			--(location_handler->location_count);
			location_handler->locations = memory_system_realloc(location_handler->locations, location_handler->location_count * sizeof(Location), (location_handler->location_count + 1) * sizeof(Location), HM_MEMORY_GROUP_UNKNOWN);
		}
	}

	for (u64 i = 0; i < location_handler->location_count; ++i)
		memory_system_free(location_files[i], (strlen(location_files[i]) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

	memory_system_free(location_files, location_handler->location_count * sizeof(char*), HM_MEMORY_GROUP_STRING);

	memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
}

void location_handler_shutdown(LocationHandler* location_handler)
{
	for (u64 i = 0; i < location_handler->location_count; ++i)
	{
		Location* location = location_handler->locations + i;

		for (u64 j = 0; j < location->path_count; ++j)
		{
			LocationPath* location_path = location->paths + j;

			memory_system_free(location_path->name_id, (strlen(location_path->name_id) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		}

		memory_system_free(location->paths, location->path_count * sizeof(LocationPath), HM_MEMORY_GROUP_UNKNOWN);
		memory_system_free(location->name_id, (strlen(location->name_id) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	}
	
	memory_system_free(location_handler->locations, location_handler->location_count * sizeof(Location), HM_MEMORY_GROUP_UNKNOWN);
	memory_system_free(location_handler, sizeof(LocationHandler), HM_MEMORY_GROUP_UNKNOWN);
}