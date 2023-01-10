#include "memory_system.h"

#include <stdlib.h>

typedef struct MemorySystem
{
	u64 total_allocated;
	u64 groups_allocated[HM_MEMORY_GROUP_SIZE];
	u64 total_allocations;
	u64 total_deallocations;
} MemorySystem;

static MemorySystem memory_system;

void memory_system_alloc(u64 size, MemoryGroup group)
{
	HM_ASSERT(group < HM_MEMORY_GROUP_SIZE);
	
	HM_TRACE("[MemorySystem]: Allocated %zu bytes", size);

	if (group == HM_MEMORY_GROUP_UNKNOWN)
		HM_WARN("[MemorySystem]: Allocated memory with group HM_MEMORY_GROUP_UNKNOWN!");

	memory_system.total_allocated += size;
	memory_system.groups_allocated[group] += size;
}

void memory_system_dealloc(u64 size, MemoryGroup group)
{
	HM_ASSERT(group < HM_MEMORY_GROUP_SIZE); 

	HM_ASSERT(memory_system.total_allocated >= size);

	HM_ASSERT(memory_system.groups_allocated[group] >= size);

	HM_TRACE("[MemorySystem]: Freed %zu bytes", size);

	if (group == HM_MEMORY_GROUP_UNKNOWN)
		HM_WARN("[MemorySystem]: Freed memory with group HM_MEMORY_GROUP_UNKNOWN!");

	memory_system.total_allocated -= size;
	memory_system.groups_allocated[group] -= size;
}

void* memory_system_malloc(u64 size, MemoryGroup group)
{
	memory_system_alloc(size, group);

	return malloc(size);
}

void* memory_system_calloc(u64 count, u64 size, MemoryGroup group)
{
	memory_system_alloc(count * size, group);

	return calloc(count, size);
}

void* memory_system_realloc(void* ptr, u64 new_size, u64 current_size, MemoryGroup group)
{
	if (ptr == NULL)
	{
		memory_system_alloc(new_size, group);

		return realloc(ptr, new_size);
	}

	if (new_size > current_size)
	{
		memory_system_alloc(new_size - current_size, group);

		return realloc(ptr, new_size);
	}

	if (new_size < current_size)
	{
		memory_system_dealloc(current_size - new_size, group);

		return realloc(ptr, new_size);
	}

	HM_WARN("[MemorySystem]: Ignored reallocation: Size did not change!");

	return ptr;
}

void memory_system_free(void* ptr, u64 size, MemoryGroup group)
{
	memory_system_dealloc(size, group);

	free(ptr);
}

const char* memory_group_to_str(MemoryGroup memory_group)
{
	HM_ASSERT(memory_group < HM_MEMORY_GROUP_SIZE);

	switch (memory_group)
	{
		case HM_MEMORY_GROUP_UNKNOWN: return "Unknown";
		case HM_MEMORY_GROUP_STRING: return "String";
		case HM_MEMORY_GROUP_APPLICATION: return "Application";
		case HM_MEMORY_GROUP_PLATFORM: return "Platform";
		case HM_MEMORY_GROUP_SERIALIZABLE_DATA: return "Serializable Data";
		case HM_MEMORY_GROUP_CONFIG_HANDLER: return "Config Handler";
		case HM_MEMORY_GROUP_SAVEFILE: return "Savefile";
		case HM_MEMORY_GROUP_LOCALE: return "Locale";
		case HM_MEMORY_GROUP_LOCALE_HANDLER: return "Locale Handler";
		case HM_MEMORY_GROUP_LOCATION_PATH: return "Location Path";
		case HM_MEMORY_GROUP_LOCATION: return "Location";
		case HM_MEMORY_GROUP_LOCATION_HANDLER: return "Location Handler";

		default:
			break;
	}

	HM_ASSERT(HM_FALSE);

	return "Unknown";
}

void memory_system_log(void)
{
	HM_INFO("[MemorySystem]: Total allocated: %zu", memory_system.total_allocated);

	for(MemoryGroup memory_group = HM_MEMORY_GROUP_UNKNOWN; memory_group < HM_MEMORY_GROUP_SIZE; ++memory_group)
		HM_INFO("[MemorySystem]: MemoryGroup \"%s\": %zu", memory_group_to_str(memory_group), memory_system.groups_allocated[memory_group]);
}