#include "locale.h"

#include <stdio.h>
#include <string.h>

#include "core/memory_system.h"

#include "io/serialization.h"

#define INVALID_LOCALE "INVALID_LOCALE"

void locale_shutdown(Locale* locale)
{
	if (locale->key != NULL)
	{
		memory_system_free(locale->key, (strlen(locale->key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		memory_system_free(locale->value, (strlen(locale->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
	}

	memory_system_free(locale, sizeof(Locale), HM_MEMORY_GROUP_UNKNOWN);
}

// Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
u32 hash(const char* str)
{
	u32 seed = 0;

	for (; *str; ++str)
	{
		seed ^= *str;
		seed *= 0x5bd1e995;
		seed ^= seed >> 15;
	}

	return seed;
}

LocaleHandler* locale_handler_create(u32 size)
{
	LocaleHandler* locale_handler = memory_system_malloc(sizeof(LocaleHandler), HM_MEMORY_GROUP_UNKNOWN);
	locale_handler->size = size;
	locale_handler->locales = memory_system_calloc(size, sizeof(Locale), HM_MEMORY_GROUP_UNKNOWN);
	return locale_handler;
}

void locale_handler_insert(LocaleHandler* locale_handler, const char* key, const char* value)
{
	if (key == NULL)
	{
		HM_ERROR("[LocaleHandler]: Tried to insert locale, but key was NULL! Value was: \"%s\"", value);
		return;
	}
	
	if (value == NULL)
	{
		HM_ERROR("[LocaleHandler]: Tried to insert locale, but value was NULL! Key was: \"%s\"", key);
		return;
	}

	const u32 index = hash(key) % locale_handler->size;

	if (locale_handler->locales[index].key == NULL)
	{
		locale_handler->locales[index].key = memory_system_calloc(strlen(key) + 1, sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(locale_handler->locales[index].key, key);
		locale_handler->locales[index].value = memory_system_calloc(strlen(value) + 1, sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(locale_handler->locales[index].value, value);
	}
	else
		HM_ERROR("[LocaleHandler]: Hash collision occured! Tried to insert locale with key \"%s\", but there was already locale with key \"%s\"!", key, locale_handler->locales[index].key);
}

const char* locale_handler_get(LocaleHandler* locale_handler, const char* key)
{
	if (key == NULL)
	{
		HM_WARN("[LocaleHandler]: Called locale_handler_get() with invalid key! Key was NULL!");
		return "HM_KEY_INVALID";
	}

	const u32 index = hash(key) % locale_handler->size;

	if (locale_handler->locales[index].key == NULL)
	{
		HM_WARN("[LocaleHandler]: Called locale_handler_get(), but locale with specified key does not exist! Key was: \"%s\"", key);
		return key;
	}

	if (strcmp(key, locale_handler->locales[index].key) != 0)
		HM_WARN("[LocaleHandler]: Returning locale with hashed key \"%s\", but actual key was \"%s\"!", key, locale_handler->locales[index].key);

	return locale_handler->locales[index].value;
}

void locale_handler_load_languages(LocaleHandler* locale_handler, const char* game_dir, const char* current_language_key)
{
	char* path = memory_system_calloc(strlen(game_dir) + strlen("/locale/languages.txt") + 1, sizeof(char), HM_MEMORY_GROUP_STRING);
	sprintf(path, "%s/locale/languages.txt", game_dir);

	SerializableData* language_data = serializable_data_create_from_file(path);

	if (language_data == NULL)
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Empty file!", path);
		memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		return;
	}

	if (strcmp(language_data->key, "languages") != 0)
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Expected parent node with key \"languages\"!", path);
		serializable_data_shutdown(language_data);
		memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		return;
	}

	locale_handler->languages = NULL;
	locale_handler->language_count = 0;

	for (u64 i = 0; i < language_data->children_count; ++i)
	{
		SerializableData* language_definition_data = language_data->children[i];

		++(locale_handler->language_count);

		locale_handler->languages = memory_system_realloc(locale_handler->languages, locale_handler->language_count * sizeof(Locale*), (locale_handler->language_count - 1) * sizeof(Locale*), HM_MEMORY_GROUP_UNKNOWN);
		locale_handler->languages[locale_handler->language_count - 1] = memory_system_malloc(sizeof(Locale), HM_MEMORY_GROUP_UNKNOWN);
		Locale* locale = locale_handler->languages[locale_handler->language_count - 1];
		locale->key = memory_system_malloc((strlen(language_definition_data->key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(locale->key, language_definition_data->key);
	}

	SerializableData* target_language_data = serializable_data_find(language_data, current_language_key);

	if (target_language_data == NULL)
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Found no definition of language \"%s\"!", path, current_language_key);
		serializable_data_shutdown(language_data);
		memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);

		for (u64 i = 0; i < locale_handler->language_count; ++i)
		{
			Locale* locale = locale_handler->languages[i];
			locale->value = memory_system_malloc((strlen(INVALID_LOCALE) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(locale->value, INVALID_LOCALE);
		}

		return;
	}

	for (u64 i = 0; i < target_language_data->children_count; ++i)
	{
		SerializableData* child = target_language_data->children[i];

		if (i >= locale_handler->language_count)
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Too many language definitions in language node \"%s\"! (Skipping excess definitions)", path, target_language_data->key);
			break;
		}

		Locale* locale = locale_handler->languages[i];

		if (strcmp(child->key, locale->key) != 0)
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Wrong order of language definitions: Expected \"%s\", but got \"%s\"! (Skipped)", path, locale->key, child->key);
			locale->value = memory_system_malloc((strlen(INVALID_LOCALE) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(locale->value, INVALID_LOCALE);
			continue;
		}

		locale->value = memory_system_malloc((strlen(child->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(locale->value, child->value);
	}

	serializable_data_shutdown(language_data);
	memory_system_free(path, (strlen(path) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
}

b8 load_locale(LocaleHandler* locale_handler, const char* path)
{
	FILE* f = fopen(path, "r");

	if (f == NULL)
	{
		HM_ERROR("[LocaleHandler]: Failed to open file \"%s\"!", path);
		return HM_FALSE;
	}

	u64 line_number = 0;

	char buffer[256];

	while (fgets(buffer, 256, f) != NULL)
	{
		++line_number;
		
		if (strlen(buffer) == 0)
			continue;

		if (buffer[0] == '\n' || buffer[0] == '\r')
			continue;

		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';

		const char* splitter = strchr(buffer, '=');

		if (splitter == NULL)
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s:%zu: Found no '=' (Skipped line)", path, line_number);
			continue;
		}

		const u64 splitter_index = (u64)(splitter - buffer);

		if (splitter_index == 0 || splitter_index == strlen(buffer) - 1)
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s:%zu: '=' can't be at the beginning or end of a line (Skipped line)", path, line_number);
			continue;
		}

		buffer[splitter_index] = '\0';
		locale_handler_insert(locale_handler, buffer, buffer + splitter_index + 1);
		HM_TRACE("[LocaleHandler]: Loaded locale: Key: \"%s\", Value: \"%s\"", buffer, buffer + splitter_index + 1);
	}

	fclose(f);

	return HM_TRUE;
}

b8 locale_handler_load_locales(LocaleHandler* locale_handler, const char* game_dir, const char* current_language_key)
{
	char path[256];

#define FILES_TO_LOAD_COUNT 4
	static const char* files_to_load[FILES_TO_LOAD_COUNT] = { "commands", "interfaces", "controls", "locations" };

	for (u64 i = 0; i < FILES_TO_LOAD_COUNT; ++i)
	{
		sprintf(path, "%s/locale/%s/%s.txt", game_dir, current_language_key, files_to_load[i]);
		if (!load_locale(locale_handler, path))
			return HM_FALSE;
	}

	return HM_TRUE;
}

void locale_handler_shutdown(LocaleHandler* locale_handler)
{
	for (u32 i = 0; i < locale_handler->size; ++i)
	{
		Locale* locale = locale_handler->locales + i;

		if (locale->key != NULL)
		{
			memory_system_free(locale->key, (strlen(locale->key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			memory_system_free(locale->value, (strlen(locale->value) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		}
	}
	
	memory_system_free(locale_handler->locales, locale_handler->size * sizeof(Locale), HM_MEMORY_GROUP_UNKNOWN);
	
	for (u8 i = 0; i < locale_handler->language_count; ++i)
		locale_shutdown(locale_handler->languages[i]);
	
	memory_system_free(locale_handler->languages, locale_handler->language_count * sizeof(Locale*), HM_MEMORY_GROUP_UNKNOWN);
	
	memory_system_free(locale_handler, sizeof(LocaleHandler), HM_MEMORY_GROUP_UNKNOWN);
}