// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#ifdef __cplusplus
extern "C" {
#endif

// We don't really have locale support.
// These are only Windows-specific stubs and placeholders, providing just the bare minimum for libc++.

typedef void *_locale_t;

static _locale_t _create_locale (int category, const char *locale)
{
    return NULL;
}

static void _free_locale (_locale_t locale) {}

#define _ENABLE_PER_THREAD_LOCALE 1
#define _DISABLE_PER_THREAD_LOCALE 2
static int _configthreadlocale (int per_thread_locale_type)
{
    // We don't support per-thread locales (yet), so we return with a failure code
    return -1;
}

#ifdef __cplusplus
}
#endif
