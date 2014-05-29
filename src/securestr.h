/**
 * secureStrings library
 * version 0.51-alpha (2014-05-29_001)
 *
 * Copyright (C) 2010, 2014 Robert ALTNOEDER
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that
 * the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SECURESTR_H
#define _SECURESTR_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

/**
 * MAXIMUM CAPACITY
 *
 * The maximum capacity (size) of any char array allocated by
 * the secureStrings library never exceeds the maximum datatype value
 * of size_t minus 127
 *
 * The upper 127 values are reserved for status codes
 *
 * size_t is a system datatype. size_t MUST be unsigned.
 */
#ifdef SIZE_MAX
    #define SSTR_SIZE_T_MAX ((~((size_t) 0)) < SIZE_MAX ? (~((size_t) 0)) : SIZE_MAX)
#else
    #define SSTR_SIZE_T_MAX (~((size_t) 0))
#endif /* SIZE_MAX */

/**
 * SSTR_RCAP_MAX is the maximum number of char elements in an array
 */
#define SSTR_RCAP_MAX ((size_t) ((SSTR_SIZE_T_MAX - 127) < SSTR_SIZE_T_MAX ? \
    (SSTR_SIZE_T_MAX - 127) : SSTR_SIZE_T_MAX))

/**
 * SSTR_CAP_MAX is the maximum number of char elements
 * that can be used for string processing
 *
 * SSTR_CAP_MAX is (SSTR_RCAP_MAX - 1)
 */
#define SSTR_CAP_MAX ((size_t) (SSTR_RCAP_MAX - 1))

/**
 * Define pos_t (position type), which is used to identify the
 * position of a character inside a secureString
 */
typedef size_t sstr_pos;

typedef struct sstr_struct
{
    char     *chars;
    size_t   cap;
    sstr_pos len;
} sstring;

/**
 * datatype for return values of secureStrings functions
 */
typedef   unsigned int   sstr_rc;

/* valid values of the sstr_rc datatype */
#ifndef _MAC_OS_X
const sstr_rc SSTR_PASS;
const sstr_rc SSTR_FAIL;
const sstr_rc SSTR_FALSE;
const sstr_rc SSTR_TRUE;
#endif
#ifdef _MAC_OS_X
#define SSTR_PASS    ((sstr_rc) 0)
#define SSTR_FAIL  (~((sstr_rc) 0))
#define SSTR_FALSE   ((sstr_rc) 0)
#define SSTR_TRUE    ((sstr_rc) 1)
#endif

/* SSTR_NPOS is the value returned to indicate
 * an invalid position in an array */
#ifndef _MAC_OS_X
const sstr_pos SSTR_NPOS;
#endif
#ifdef _MAC_OS_X
#define SSTR_NPOS (~((sstr_pos) 0))
#endif

sstring *sstr_alloc(
    size_t
);

void sstr_dealloc(
    sstring *
);

sstr_rc sstr_cpy(
    sstring *,
    sstring *
);

sstr_rc sstr_appd(
    sstring *,
    sstring *
);

sstr_rc sstr_appdchar(
    char,
    sstring *
);

sstr_rc sstr_substr(
    sstring *,
    sstring *,
    sstr_pos,
    size_t
);

sstr_rc sstr_appdsubstr(
    sstring *,
    sstring *,
    sstr_pos,
    size_t
);

sstr_rc sstr_trunc(
    sstring *,
    size_t
);

sstr_rc sstr_cmp(
    sstring *,
    sstring *
);

sstr_rc sstr_startswith(
    sstring *,
    sstring *
);

sstr_rc sstr_endswith(
    sstring *,
    sstring *
);

sstr_pos sstr_indexof(
    sstring *,
    sstring *
);

sstr_rc sstr_getchar(
    sstring *,
    char *,
    sstr_pos
);

sstr_rc sstr_setchar(
    char,
    sstring *,
    sstr_pos
);

sstr_rc sstr_swap(
    sstring *,
    sstring *
);

sstr_rc sstr_clear(
    sstring *
);

sstr_rc sstr_wipe(
    sstring *
);

size_t sstr_len(
    sstring *
);

size_t sstr_cap(
    sstring *
);

#define sString         sstring

#define sstrVersion     sstr_version

#define sstrAlloc       sstr_alloc
#define sstrDealloc     sstr_dealloc
#define sstrCpy         sstr_cpy
#define sstrAppd        sstr_appd
#define sstrAppdChar    sstr_appdchar
#define sstrSubstr      sstr_substr
#define sstrAppdSubstr  sstr_appdsubstr
#define sstrTrunc       sstr_trunc
#define sstrCmp         sstr_cmp
#define sstrStartsWith  sstr_startswith
#define sstrEndsWith    sstr_endswith
#define sstrIndexOf     sstr_indexof
#define sstrGetChar     sstr_getchar
#define sstrSetChar     sstr_setchar
#define sstrSwap        sstr_swap
#define sstrClear       sstr_clear
#define sstrWipe        sstr_wipe
#define sstrLen         sstr_len
#define sstrCap         sstr_cap

#endif /* _SECURESTR_H */
