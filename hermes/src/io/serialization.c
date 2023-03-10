#include "serialization.h"

#include <stdio.h>
#include <string.h>

#include "core/memory_system.h"

SerializableData* serializable_data_create(const char* key, const char* value)
{
	SerializableData* serializable_data = memory_system_malloc(sizeof(SerializableData), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
	serializable_data->key = memory_system_malloc((strlen(key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	strcpy(serializable_data->key, key);

	if (value != NULL)
	{
		serializable_data->value = memory_system_malloc((strlen(value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(serializable_data->value, value);
	}
	else
		serializable_data->value = NULL;

	serializable_data->children = NULL;
	serializable_data->children_count = 0;

	return serializable_data;
}

SerializableData* serializable_data_create_from_file(const char* path)
{
	FILE* f = fopen(path, "r");

	if (f == NULL)
	{
		HM_ERROR("[SerializableData]: Failed to open file \"%s\"!", path);

		return NULL;
	}

	SerializableData* root = NULL;
	SerializableData* parent = NULL;

	u64 indentation_level = 0;

	u64 line_number = 0;

	char buffer[256];

	while (fgets(buffer, 256, f) != NULL)
	{
		++line_number;
		
		if (strlen(buffer) == 0)
			continue;

		if (buffer[0] == '\n')
			continue;

		if (buffer[0] == '#')
			continue;

		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';

		if (buffer[strlen(buffer) - 1] != ':')
		{
			HM_ERROR("[SerializableData]: Incorrect syntax in %s:%zu: Expected parent node declaration!", path, line_number);
			fclose(f);
			return NULL;
		}

		buffer[strlen(buffer) - 1] = '\0';
		root = serializable_data_create(buffer, NULL);
		parent = root;

		++indentation_level;

		break;
	}

	u64 parent_node_count = 1;
	SerializableData** parent_nodes = memory_system_malloc(parent_node_count * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
	parent_nodes[0] = parent;

	while (fgets(buffer, 256, f) != NULL)
	{
		++line_number;

		if (strlen(buffer) == 0)
			continue;

		if (buffer[0] == '\n')
			continue;

		if (buffer[0] == '#')
			continue;

		u64 whitespaces = 0;
		for (u64 i = 0; i < strlen(buffer); ++i)
		{
			if (buffer[i] == ' ')
				++whitespaces;
			else
				break;
		}

		if (whitespaces > indentation_level)
		{
			HM_WARN("[SerializableData]: Incorrect syntax in %s:%zu: Expected indentation level of %zu, but got %zu (Skipped line)", path, line_number, indentation_level, whitespaces);
			continue;
		}
		else if (whitespaces == indentation_level - 1)
		{
			if (parent->children_count == 0)
				HM_WARN("[SerializableData]: Incorrect syntax in %s:%zu: Parent node \"%s\" has no children!", path, line_number, parent->key);
			
			--parent_node_count;
			parent_nodes = memory_system_realloc(parent_nodes, parent_node_count * sizeof(SerializableData*), (parent_node_count + 1) * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
			parent = parent_nodes[parent_node_count - 1];

			--indentation_level;
		}
		else if (whitespaces < indentation_level)
		{
			HM_WARN("[SerializableData]: Incorrect syntax in %s:%zu: Expected indentation level of %zu, but got %zu (Skipped line)", path, line_number, indentation_level, whitespaces);
			continue;
		}

		if(buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';

		const b8 line_contains_delimiter = strchr(buffer, '=') != NULL;
		if(!line_contains_delimiter)
		{
			buffer[strlen(buffer) - 1] = '\0';

			++(parent->children_count);

			parent->children = memory_system_realloc(parent->children, parent->children_count * sizeof(SerializableData*), (parent->children_count - 1) * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);

			parent->children[parent->children_count - 1] = serializable_data_create(buffer + indentation_level, NULL);

			parent = parent->children[parent->children_count - 1];

			++parent_node_count;
			parent_nodes = memory_system_realloc(parent_nodes, parent_node_count * sizeof(SerializableData*), (parent_node_count - 1) * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
			parent_nodes[parent_node_count - 1] = parent;

			++indentation_level;

			continue;
		}
		
		const char* splitter = strchr(buffer, '=');

		if (splitter == NULL)
		{
			HM_WARN("[SerializableData]: Incorrect syntax in %s:%zu: Expected key-value pair, but found no '=' symbol! (Skipped line)", path, line_number);
			continue;
		}

		const u64 splitter_index = (u64)(splitter - buffer);

		if (splitter_index == indentation_level || splitter_index == strlen(buffer) - 1)
		{
			HM_WARN("[SerializableData]: Incorrect syntax in %s:%zu: '=' symbol can't be at the beginning or end of a key-value pair definition! (Skipped line)", path, line_number);
			continue;
		}

		buffer[splitter_index] = '\0';

		++(parent->children_count);
		parent->children = memory_system_realloc(parent->children, parent->children_count * sizeof(SerializableData*), (parent->children_count - 1) * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);

		parent->children[parent->children_count - 1] = serializable_data_create(buffer + indentation_level, buffer + splitter_index + 1);
	}

	fclose(f);

	memory_system_free(parent_nodes, parent_node_count * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);

	if(root == NULL)
		HM_ERROR("[SerializableData]: Error in %s:%zu: Empty file!", path, line_number);

	return root;
}

b8 save_to_file(SerializableData* data, u64 whitespace_count, FILE* f)
{
	if (fwrite(" ", sizeof(char), whitespace_count, f) != whitespace_count)
		return HM_FALSE;

	if (data->children_count == 0 && data->value != NULL)
	{
		if (fwrite(data->key, sizeof(char), strlen(data->key), f) != strlen(data->key))
			return HM_FALSE;
		
		if (fwrite("=", sizeof(char), 1, f) != 1)
			return HM_FALSE;

		if (fwrite(data->value, sizeof(char), strlen(data->value), f) != strlen(data->value))
			return HM_FALSE;

		if (fwrite("\n", sizeof(char), 1, f) != 1)
			return HM_FALSE;
	}
	else
	{
		if (fwrite(data->key, sizeof(char), strlen(data->key), f) != strlen(data->key))
			return HM_FALSE;

		if (fwrite(":", sizeof(char), 1, f) != 1)
			return HM_FALSE;

		if (fwrite("\n", sizeof(char), 1, f) != 1)
			return HM_FALSE;

		for (u64 i = 0; i < data->children_count; ++i)
		{
			if (!save_to_file(data->children[i], whitespace_count + 1, f))
				return HM_FALSE;
		}
	}

	return HM_TRUE;
}

b8 serializable_data_save_to_file(SerializableData* root, const char* path)
{
	FILE* f = fopen(path, "w");

	if (f == NULL)
	{
		HM_ERROR("[SerializableData]: Failed to open file \"%s\"!", path);
		return HM_FALSE;
	}

	b8 success = save_to_file(root, 0, f);

	fclose(f);

	return success;
}

void serializable_data_add_child(SerializableData* parent, SerializableData* child)
{
	++(parent->children_count);
	parent->children = memory_system_realloc(parent->children, parent->children_count * sizeof(SerializableData*), (parent->children_count - 1) * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
	parent->children[parent->children_count - 1] = child;
}

SerializableData* serializable_data_find(SerializableData* parent, const char* key)
{
	for (u64 i = 0; i < parent->children_count; ++i)
	{
		if (strcmp(parent->children[i]->key, key) == 0)
			return parent->children[i];
	}

	return NULL;
}

b8 serializable_data_is_parent(SerializableData* serializable_data)
{
	return serializable_data->children_count > 0 && serializable_data->value == NULL;
}

b8 serializable_data_is_child(SerializableData* serializable_data)
{
	return serializable_data->children_count == 0 && serializable_data->value != NULL;
}

void serializable_data_print(SerializableData* root, u64 offset)
{
	char whitespaces[offset + 1];
	memset(whitespaces, ' ', offset);
	whitespaces[offset] = '\0';

	if (root->value != NULL)
	{
		HM_TRACE("[SerializableData]: %s%s=%s", whitespaces, root->key, root->value);
	}
	else
	{
		HM_TRACE("[SerializableData]: %s%s:", whitespaces, root->key);

		for (u64 i = 0; i < root->children_count; ++i)
			serializable_data_print(root->children[i], offset + 1);
	}
}

void serializable_data_log(SerializableData* root)
{
	serializable_data_print(root, 0);
}

void serializable_data_shutdown(SerializableData* root)
{
	memory_system_free(root->key, (strlen(root->key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	
	if(root->value != NULL)
		memory_system_free(root->value, (strlen(root->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	else
	{
		for (u64 i = 0; i < root->children_count; ++i)
			serializable_data_shutdown(root->children[i]);

		memory_system_free(root->children, root->children_count * sizeof(SerializableData*), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
	}

	memory_system_free(root, sizeof(SerializableData), HM_MEMORY_GROUP_SERIALIZABLE_DATA);
}