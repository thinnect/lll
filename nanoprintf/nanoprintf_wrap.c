/**
 * nanoprintf wrapper for newlib-nano.
 *
 * Provides linker wrap functions to redirect sprintf/snprintf calls to nanoprintf,
 * giving proper C99 format specifier support (PRIu8, PRIx64, %hhu, etc.) that
 * newlib-nano lacks.
 *
 * Enable with:
 *   CFLAGS  += -DUSE_NANOPRINTF
 *   LDFLAGS += -Wl,--wrap=sprintf -Wl,--wrap=snprintf
 *   LDFLAGS += -Wl,--wrap=vsprintf -Wl,--wrap=vsnprintf
 *
 * Copyright Thinnect Inc. 2025
 * @license MIT
 */

#include <stdarg.h>
#include <stddef.h>

// Configure nanoprintf features
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1  // For PRIx64 etc.
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 1  // For PRIu8, PRIu16 (hh, h)
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_ALT_FORM_FLAG 1
#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

/**
 * Linker wrap functions to redirect all sprintf/snprintf calls to nanoprintf.
 * These are enabled by -Wl,--wrap=snprintf etc. in LDFLAGS.
 * Any call to snprintf() becomes __wrap_snprintf(), which we implement here.
 */

int __wrap_sprintf(char *buffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = npf_vsnprintf(buffer, (size_t)-1, format, args);
    va_end(args);
    return ret;
}

int __wrap_snprintf(char *buffer, size_t bufsz, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = npf_vsnprintf(buffer, bufsz, format, args);
    va_end(args);
    return ret;
}

int __wrap_vsprintf(char *buffer, const char *format, va_list args)
{
    return npf_vsnprintf(buffer, (size_t)-1, format, args);
}

int __wrap_vsnprintf(char *buffer, size_t bufsz, const char *format, va_list args)
{
    return npf_vsnprintf(buffer, bufsz, format, args);
}
