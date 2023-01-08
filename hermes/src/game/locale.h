#ifndef HM_LOCALE_H
#define HM_LOCALE_H

#include "core/core.h"

typedef struct Locale
{
	char* key;
	char* value;
} Locale;

void locale_shutdown(Locale* locale);

typedef struct LocaleHandler
{
	Locale* locales;
	u32 size;
	Locale** languages;
	u8 language_count;
} LocaleHandler;

LocaleHandler* locale_handler_create(u32 size);

void locale_handler_insert(LocaleHandler* locale_handler, const char* key, const char* value);

const char* locale_handler_get(LocaleHandler* locale_handler, const char* key);

void locale_handler_load_languages(LocaleHandler* locale_handler, const char* game_dir, const char* language_key);

void locale_handler_load_locales(LocaleHandler* locale_handler, const char* game_dir, const char* language_key);

void locale_handler_shutdown(LocaleHandler* locale_handler);

#endif