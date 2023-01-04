#include "memory_system.h"

#include <stdlib.h>

typedef struct MemorySystem
{
	u64 total_allocated;
	u64 groups_allocated[HM_MEMORY_GROUP_SIZE];
} MemorySystem;

static MemorySystem memory_system;

void memory_system_alloc(u64 size, MemoryGroup group)
{
	HM_ASSERT(group < HM_MEMORY_GROUP_SIZE);
	
	if (group == HM_MEMORY_GROUP_UNKNOWN)
		HM_WARN("[MemorySystem]: Allocated memory with group HM_MEMORY_GROUP_UNKNOWN!");

	memory_system.total_allocated += size;
	memory_system.groups_allocated[group] += size;
}

void memory_system_dealloc(u64 size, MemoryGroup group)
{
	HM_ASSERT(group < HM_MEMORY_GROUP_SIZE); 
	
	if (group == HM_MEMORY_GROUP_UNKNOWN)
		HM_WARN("[MemorySystem]: Freed memory with group HM_MEMORY_GROUP_UNKNOWN!");

	HM_ASSERT(memory_system.total_allocated >= size);

	HM_ASSERT(memory_system.groups_allocated[group] >= size);

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

	HM_WARN("[MemorySystem]: Ignored reallocation: new_size and current_size were equal!");

	return ptr;
}

void memory_system_free(void* ptr, u64 size, MemoryGroup group)
{
	memory_system_dealloc(size, group);

	free(ptr);
}

void memory_system_log(void)
{
	HM_INFO("[MemorySystem]: Total allocated: %zu", memory_system.total_allocated);
	HM_INFO("[MemorySystem]: MemoryGroup \"Unknown\": %zu", memory_system.groups_allocated[HM_MEMORY_GROUP_UNKNOWN]);
	HM_INFO("[MemorySystem]: MemoryGroup \"String\": %zu", memory_system.groups_allocated[HM_MEMORY_GROUP_STRING]);
}