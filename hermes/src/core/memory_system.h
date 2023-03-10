#ifndef HM_MEMORY_SYSTEM_H
#define HM_MEMORY_SYSTEM_H

#include "core/core.h"

typedef enum MemoryGroup
{
	HM_MEMORY_GROUP_UNKNOWN = 0,
	HM_MEMORY_GROUP_STRING,
	HM_MEMORY_GROUP_APPLICATION,
	HM_MEMORY_GROUP_PLATFORM,
	HM_MEMORY_GROUP_SERIALIZABLE_DATA,
	HM_MEMORY_GROUP_CONFIG_HANDLER,
	HM_MEMORY_GROUP_SAVEFILE,
	HM_MEMORY_GROUP_LOCALE,
	HM_MEMORY_GROUP_LOCALE_HANDLER,
	HM_MEMORY_GROUP_LOCATION_PATH,
	HM_MEMORY_GROUP_LOCATION,
	HM_MEMORY_GROUP_LOCATION_HANDLER,

	HM_MEMORY_GROUP_SIZE
} MemoryGroup;

void* memory_system_malloc(u64 size, MemoryGroup group);

void* memory_system_calloc(u64 count, u64 size, MemoryGroup group);

void* memory_system_realloc(void* ptr, u64 new_size, u64 current_size, MemoryGroup group);

void memory_system_free(void* ptr, u64 size, MemoryGroup group);

void memory_system_log(void);

#endif