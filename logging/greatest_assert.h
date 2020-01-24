/*
 * Copyright (c) 2011 Scott Vokes <vokes.s@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef GREATEST_H
#define GREATEST_H

#define GREATEST_VERSION_MAJOR 0
#define GREATEST_VERSION_MINOR 9
#define GREATEST_VERSION_PATCH 3

/* A unit testing system for C, contained in 1 file.
 * It doesn't use dynamic allocation or depend on anything
 * beyond ANSI C89. */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Look here for examples
 * https://github.com/silentbicycle/greatest/blob/master/example.c
 */

/* Message-less forms. */
#define ASSERT(COND) ASSERTm(#COND, COND)
#define ASSERT_EQUAL(EXP, GOT) ASSERT_EQUALm(#EXP " != " #GOT, EXP, GOT)
#define ASSERT_NOT_EQUAL(EXP, GOT) ASSERT_NOT_EQUALm(#EXP " == " #GOT, EXP, GOT)
#define ASSERT_GREATER(P1, P2) ASSERT_GREATERm(#P1 " not > " #P2, P1, P2)
#define ASSERT_GREATER_EQUAL(P1, P2) ASSERT_GREATER_EQUALm(#P1 " not >= " #P2, P1, P2)
#define ASSERT_LESS(P1, P2) ASSERT_LESSm(#P1 " not < " #P2, P1, P2)
#define ASSERT_LESS_EQUAL(P1, P2) ASSERT_LESS_EQUALm(#P1 " not <= " #P2, P1, P2)
#define ASSERT_STR_EQUAL(EXP, GOT) ASSERT_STR_EQUALm(#EXP " != " #GOT, EXP, GOT)
#define ASSERT_MEM_EQUAL(EXP, GOT, LEN) ASSERT_MEM_EQUALm(#EXP " != " #GOT, EXP, GOT, LEN)

#if defined(__LOG_LEVEL__) && (__LOG_LEVEL__ & LOG_LEVEL_ASSERT)
    /* The following forms take an additional message argument first,
     * to be displayed by the test runner. */

    /* Fail if a condition is not true, with message. */
    #define ASSERTm(MSG, COND)                                                                                  \
        do {                                                                                                    \
            if (!(COND)) {                                                                                      \
                logger(LOG_LEVEL_ASSERT, "ASSERT %s", MSG);                                                     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_EQUALm(MSG, EXP, GOT)                                                                        \
        do {                                                                                                    \
            if ((EXP) != (GOT)) {                                                                               \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(EXP), MSG, (uint32_t)(GOT));   \
            }                                                                                                   \
        } while (0)

    #define ASSERT_NOT_EQUALm(MSG, EXP, GOT)                                                                    \
        do {                                                                                                    \
            if ((EXP) == (GOT)) {                                                                               \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(EXP), MSG, (uint32_t)(GOT));   \
            }                                                                                                   \
        } while (0)

    #define ASSERT_GREATERm(MSG, P1, P2)                                                                        \
        do {                                                                                                    \
            if ((P1) <= (P2)) {                                                                                 \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(P1), MSG, (uint32_t)(P2));     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_GREATER_EQUALm(MSG, P1, P2)                                                                  \
        do {                                                                                                    \
            if ((P1) < (P2)) {                                                                                  \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(P1), MSG, (uint32_t)(P2));     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_LESSm(MSG, P1, P2)                                                                           \
        do {                                                                                                    \
            if ((P1) >= (P2)) {                                                                                 \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(P1), MSG, (uint32_t)(P2));     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_LESS_EQUALm(MSG, P1, P2)                                                                     \
        do {                                                                                                    \
            if ((P1) > (P2)) {                                                                                  \
                logger(LOG_LEVEL_ASSERT, "ASSERT [[%lu]] %s [[%lu]]", (uint32_t)(P1), MSG, (uint32_t)(P2));     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_STR_EQUALm(MSG, EXP, GOT)                                                                    \
        do {                                                                                                    \
            const char* exp_s = (EXP);                                                                          \
            const char* got_s = (GOT);                                                                          \
            if (0 != strcmp(exp_s, got_s)) {                                                                    \
                logger(LOG_LEVEL_ASSERT, "ASSERT %s", MSG);                                                     \
            }                                                                                                   \
        } while (0)

    #define ASSERT_MEM_EQUALm(MSG, EXP, GOT, LEN)                                                               \
        do {                                                                                                    \
            const char* exp_s = (EXP);                                                                          \
            const char* got_s = (GOT);                                                                          \
            if (0 != memcmp(exp_s, got_s, LEN)) {                                                               \
                loggerb2( LOG_LEVEL_ASSERT, "ASSERT %s |", (void*)exp_s, LEN, " !=", (void*)got_s, LEN, MSG);   \
            }                                                                                                   \
        } while (0)

#else
    #define ASSERTm(MSG, COND)
    #define ASSERT_EQUALm(MSG, EXP, GOT)
    #define ASSERT_NOT_EQUALm(MSG, EXP, GOT)
    #define ASSERT_GREATERm(MSG, P1, P2)
    #define ASSERT_GREATER_EQUALm(MSG, P1, P2)
    #define ASSERT_LESSm(MSG, P1, P2)
    #define ASSERT_LESS_EQUALm(MSG, P1, P2)
    #define ASSERT_STR_EQUALm(MSG, EXP, GOT)
    #define ASSERT_MEM_EQUALm(MSG, EXP, GOT, LEN)
#endif

#endif
