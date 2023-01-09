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

	memory_system_free(locale, sizeof(Locale), HM_MEMORY_GROUP_LOCALE);
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
	LocaleHandler* locale_handler = memory_system_malloc(sizeof(LocaleHandler), HM_MEMORY_GROUP_LOCALE_HANDLER);
	locale_handler->size = size;
	locale_handler->locales = memory_system_calloc(size, sizeof(Locale), HM_MEMORY_GROUP_LOCALE);
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

void locale_handler_load_languages(LocaleHandler* locale_handler, const char* game_dir, const char* language_key)
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

	if (strcmp(language_data->key, "languages") != 0 || !serializable_data_is_parent(language_data))
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

		if (!serializable_data_is_parent(language_definition_data))
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Expected parent node, but got child node with key \"%s\" and value \"%s\"!", path, language_definition_data->key, language_definition_data->value);
			continue;
		}

		++(locale_handler->language_count);

		locale_handler->languages = memory_system_realloc(locale_handler->languages, locale_handler->language_count * sizeof(Locale*), (locale_handler->language_count - 1) * sizeof(Locale*), HM_MEMORY_GROUP_LOCALE);
		locale_handler->languages[locale_handler->language_count - 1] = memory_system_malloc(sizeof(Locale), HM_MEMORY_GROUP_LOCALE);
		Locale* locale = locale_handler->languages[locale_handler->language_count - 1];
		locale->key = memory_system_malloc((strlen(language_definition_data->key) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
		strcpy(locale->key, language_definition_data->key);
	}

	SerializableData* target_language_data = serializable_data_find(language_data, language_key);

	if (target_language_data == NULL)
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Found no definition of language \"%s\"!", path, language_key);
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

	if (!serializable_data_is_parent(target_language_data))
	{
		HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Expected node with key \"%s\" to be a parent node, not a child node!", path, language_key); 
		
		for (u64 i = 0; i < locale_handler->language_count; ++i)
		{
			Locale* locale = locale_handler->languages[i];
			locale->value = memory_system_malloc((strlen(INVALID_LOCALE) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(locale->value, INVALID_LOCALE);
		}
	}

	for (u64 i = 0; i < target_language_data->children_count; ++i)
	{
		SerializableData* child = target_language_data->children[i];

		if (!serializable_data_is_child(child))
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Expected node with key \"%s\" to be a child node, not a parent node!", path, child->key);

			Locale* locale = locale_handler->languages[i];

			locale->value = memory_system_malloc((strlen(INVALID_LOCALE) + 1) * sizeof(char), HM_MEMORY_GROUP_STRING);
			strcpy(locale->value, INVALID_LOCALE);

			continue;
		}

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

void load_locale(LocaleHandler* locale_handler, const char* language_key, const char* path)
{
	SerializableData* locale_data = serializable_data_create_from_file(path);

	if (locale_data == NULL)
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Empty file!", path);
		return;
	}

	if (strcmp(locale_data->key, language_key) != 0 || !serializable_data_is_parent(locale_data))
	{
		HM_ERROR("[LocaleHandler]: Incorrect syntax in %s: Expected parent node with key \"%s\"!", path, language_key);
		serializable_data_shutdown(locale_data);
		return;
	}

	for (u64 i = 0; i < locale_data->children_count; ++i)
	{
		SerializableData* child = locale_data->children[i];

		if (!serializable_data_is_child(child))
		{
			HM_WARN("[LocaleHandler]: Incorrect syntax in %s: Expected child node, but got parent node with key \"%s\"!", path, child->key);
			continue;
		}

		locale_handler_insert(locale_handler, child->key, child->value);
	}

	serializable_data_shutdown(locale_data);
}

void locale_handler_load_locales(LocaleHandler* locale_handler, const char* game_dir, const char* language_key)
{
	char path[256];

#define FILES_TO_LOAD_COUNT 4
	static const char* files_to_load[FILES_TO_LOAD_COUNT] = { "commands", "interfaces", "controls", "locations" };

	for (u64 i = 0; i < FILES_TO_LOAD_COUNT; ++i)
	{
		sprintf(path, "%s/locale/%s/%s.txt", game_dir, language_key, files_to_load[i]);
		load_locale(locale_handler, language_key, path);
	}
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
	
	memory_system_free(locale_handler->locales, locale_handler->size * sizeof(Locale), HM_MEMORY_GROUP_LOCALE);
	
	for (u8 i = 0; i < locale_handler->language_count; ++i)
		locale_shutdown(locale_handler->languages[i]);
	
	memory_system_free(locale_handler->languages, locale_handler->language_count * sizeof(Locale*), HM_MEMORY_GROUP_LOCALE);
	
	memory_system_free(locale_handler, sizeof(LocaleHandler), HM_MEMORY_GROUP_LOCALE_HANDLER);
}