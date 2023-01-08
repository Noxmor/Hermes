#ifndef HM_SERIALIZATION_H
#define HM_SERIALIZATION_H

#include "core/core.h"

typedef struct SerializableData
{
	char* key;
	char* value;
	struct SerializableData** children;
	u64 children_count;
} SerializableData;

SerializableData* serializable_data_create(const char* key, const char* value);

SerializableData* serializable_data_create_from_file(const char* path);

b8 serializable_data_save_to_file(SerializableData* root, const char* path);

void serializable_data_add_child(SerializableData* parent, SerializableData* child);

SerializableData* serializable_data_find(SerializableData* parent, const char* key);

void serializable_data_log(SerializableData* root);

void serializable_data_shutdown(SerializableData* root);

#endif