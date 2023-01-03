#include "locale.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void locale_shutdown(Locale* locale)
{
	if (locale->key != NULL)
	{
		free(locale->key);
		free(locale->value);
	}

	free(locale);
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
	LocaleHandler* locale_handler = malloc(sizeof(LocaleHandler));
	locale_handler->size = size;
	locale_handler->locales = calloc(size, sizeof(Locale));
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
		locale_handler->locales[index].key = calloc(strlen(key) + 1, sizeof(char));
		strcpy(locale_handler->locales[index].key, key);
		locale_handler->locales[index].value = calloc(strlen(value) + 1, sizeof(char));
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
		return "HM_KEY_INVALID";
	}

	if (strcmp(key, locale_handler->locales[index].key) != 0)
		HM_WARN("[LocaleHandler]: Returning locale with hashed key \"%s\", but actual key was \"%s\"!", key, locale_handler->locales[index].key);

	return locale_handler->locales[index].value;
}

b8 load_language_definitions(LocaleHandler* locale_handler)
{
	FILE* f = fopen("locale/languages.txt", "r");

	if (f == NULL)
	{
		HM_ERROR("[LocaleHandler]: Could not open file \"locale/languages.txt\"!");
		return HM_FALSE;
	}

	locale_handler->languages = NULL;

	locale_handler->language_count = 0;

	char buffer[256];

	while (fgets(buffer, 256, f) != NULL)
	{
		if (strlen(buffer) > 1 && buffer[strlen(buffer) - 1 - 1] == ':')
		{
			++locale_handler->language_count;

			buffer[strlen(buffer) - 1 - 1] = '\0';

			locale_handler->languages = realloc(locale_handler->languages, locale_handler->language_count * sizeof(Locale*));
			locale_handler->languages[locale_handler->language_count - 1] = malloc(sizeof(Locale));
			Locale* locale = locale_handler->languages[locale_handler->language_count - 1];
			locale->key = malloc((strlen(buffer) + 1) * sizeof(char));
			strcpy(locale->key, buffer);
		}
	}
	fclose(f);

	return HM_TRUE;
}

b8 locale_handler_load_languages(LocaleHandler* locale_handler, const char* current_language_key)
{
	if (!load_language_definitions(locale_handler))
		return HM_FALSE;

	FILE* f = fopen("locale/languages.txt", "r");

	if (f == NULL)
	{
		HM_ERROR("[LocaleHandler]: Could not open file \"locale/languages.txt\"!");
		return HM_FALSE;
	}

	u8 language_index = 0;
	u64 line_number = 0;

	char buffer[256];

	while (fgets(buffer, 256, f) != NULL)
	{
		++line_number;
		
		if (strlen(buffer) == 0)
			continue;

		const u64 buffer_language_len = strlen(buffer) - 1 - 1;

		if (buffer[buffer_language_len] != ':')
			continue;

		if(strncmp(buffer, current_language_key, strlen(current_language_key)) != 0)
			continue;

		while (fgets(buffer, 256, f) != NULL)
		{
			++line_number;

			if (strlen(buffer) == 0)
				continue;

			if (buffer[strlen(buffer) - 1] == '\n')
				buffer[strlen(buffer) - 1] = '\0';

			if (buffer[0] != ' ')
			{
				if (buffer[strlen(buffer) - 1] == ':')
				{
					HM_ERROR("[LocaleHandler]: Error in locale/languages.txt:%zu: Found definition of next language before loading all %zu known languages with target language \"%s\"!", line_number, locale_handler->language_count, current_language_key);
					fclose(f);
					return HM_FALSE;
				}

				HM_WARN("[LocaleHandler]: Incorrect syntax in locale/languages.txt:%zu: Line needs to be indentated with one whitespace (Skipped line)", line_number);
				continue;
			}
			
			const char* splitter = strchr(buffer, '=');
		
			if (splitter == NULL)
			{
				HM_WARN("[LocaleHandler]: Incorrect syntax in locale/languages.txt:%zu: Found no '=' (Skipped line)", line_number);
				continue;
			}
		
			const u64 splitter_index = (u64)(splitter - buffer);
		
			if (splitter_index == 1 || splitter_index == strlen(buffer) - 1)
			{
				HM_WARN("[LocaleHandler]: Incorrect syntax in locale/languages.txt:%zu: '=' can't be at the beginning or end of a line (Skipped line)", line_number);
				continue;
			}
			
			buffer[splitter_index] = '\0';

			if (strcmp(locale_handler->languages[language_index]->key, buffer + 1) != 0)
			{
				HM_ERROR("[LocaleHandler]: Error in locale/languages.txt:%zu: Invalid order of language definitions! Expected key \"%s\"", line_number, locale_handler->languages[language_index]->key);
				fclose(f);
				return HM_FALSE;
			}

			locale_handler->languages[language_index]->value = calloc(strlen(buffer + splitter_index + 1) + 1, sizeof(char));
			memcpy(locale_handler->languages[language_index]->value, buffer + splitter_index + 1, strlen(buffer + splitter_index + 1));

			HM_TRACE("[LocaleHandler]: Loaded language: Key: \"%s\", Value: \"%s\"", locale_handler->languages[language_index]->key, locale_handler->languages[language_index]->value);

			if (++language_index >= locale_handler->language_count)
				break;
		}

		while (fgets(buffer, 256, f) != NULL)
		{
			++line_number;

			if (strlen(buffer) == 0)
				continue;

			if (buffer[0] != ' ')
				break;

			HM_WARN("[LocaleHandler]: locale/languages.txt:%zu: Already read all languages! (Skipped line)", line_number);
		}

		break;
	}

	fclose(f);

	return HM_TRUE;
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

		if (buffer[0] == '\n')
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

b8 locale_handler_load_locales(LocaleHandler* locale_handler, const char* current_language_key)
{
	char path[256];

	sprintf(path, "locale/%s/commands.txt", current_language_key);
	if (!load_locale(locale_handler, path))
		return HM_FALSE;

	sprintf(path, "locale/%s/interfaces.txt", current_language_key);
	if (!load_locale(locale_handler, path))
		return HM_FALSE;

	return HM_TRUE;
}

void locale_handler_shutdown(LocaleHandler* locale_handler)
{
	for (u32 i = 0; i < locale_handler->size; ++i)
	{
		Locale* locale = locale_handler->locales + i;

		if (locale->key != NULL)
		{
			free(locale->key);
			free(locale->value);
		}
	}
	
	free(locale_handler->locales);
	
	for (u8 i = 0; i < locale_handler->language_count; ++i)
		locale_shutdown(locale_handler->languages[i]);
	
	free(locale_handler->languages);
	
	free(locale_handler);
}