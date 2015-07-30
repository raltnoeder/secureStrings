/**
 * secureStrings library
 * version 0.54-beta (2014-10-25_001)
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


// MAXIMUM CAPACITY
//
// The maximum capacity (size) of any char array allocated by
// the secureStrings library never exceeds the maximum datatype value
// of size_t minus 127
//
// The upper 127 values are reserved for status codes
//
// size_t is a system datatype. size_t MUST be unsigned.
#ifdef SIZE_MAX
    #define SSTR_SIZE_T_MAX ((~((size_t) 0)) < SIZE_MAX ? (~((size_t) 0)) : SIZE_MAX)
#else
    #define SSTR_SIZE_T_MAX (~((size_t) 0))
#endif /* SIZE_MAX */

// SSTR_RCAP_MAX is the maximum number of char elements in an array
#define SSTR_RCAP_MAX ((size_t) ((SSTR_SIZE_T_MAX - 127) < SSTR_SIZE_T_MAX ? \
        (SSTR_SIZE_T_MAX - 127) : SSTR_SIZE_T_MAX))


// SSTR_CAP_MAX is the maximum number of char elements
// that can be used for string processing
//
// SSTR_CAP_MAX is (SSTR_RCAP_MAX - 1)
#define SSTR_CAP_MAX ((size_t) (SSTR_RCAP_MAX - 1))

// Define sstr_pos (position type), which is used to identify the
// position of a character inside a secureString
typedef size_t sstr_pos;

typedef struct sstr_struct
{
    char     *chars;
    size_t   cap;
    sstr_pos len;
}
sstring;

// datatype for return values of secureStrings functions
typedef unsigned int sstr_rc;

// valid values of the sstr_rc datatype
extern const sstr_rc SSTR_PASS;
extern const sstr_rc SSTR_FAIL;
extern const sstr_rc SSTR_FALSE;
extern const sstr_rc SSTR_TRUE;

// SSTR_NPOS is the value returned to indicate
// an invalid position in an array
extern const sstr_pos SSTR_NPOS;

// SSTR_SIZE_FAIL is the value returned to indicate an invalid size
extern const size_t   SSTR_SIZE_FAIL;


#ifndef _SSTR_NO_DYNMEM
/**
 * Allocate a secureString
 *
 * sstr_cap is the number of char elements that will
 * be available for string processing
 */
sstring *sstr_alloc(
    size_t sstr_cap
);
#endif /* not _SSTR_NO_DYNMEM */


#ifndef _SSTR_NO_DYNMEM
/**
 * Deallocate a secureString
 */
void sstr_dealloc(
    sstring *dst_str
);
#endif /* not _SSTR_NO_DYNMEM */


/**
 * Copy a string to another string (overwrite)
 */
sstr_rc sstr_cpy(
    sstring *src_str,
    sstring *dst_str
);


/**
 * Append a string to another string
 */
sstr_rc sstr_appd(
    sstring *src_str,
    sstring *dst_str
);


/**
 * Append a char to a string
 */
sstr_rc sstr_appdchar(
    char    src_char,
    sstring *dst_str
);


/**
 * Query the length of a string
 */
size_t sstr_len(
    const sstring *src_str
);


/**
 * Query the capacity of a string
 */
size_t sstr_cap(
    const sstring *src_str
);


/**
 * Truncate a string to the specified length
 */
sstr_rc sstr_trunc(
    sstring *dst_str,
    size_t  sstr_len
);


/**
 * Clear a string (reset its length to zero)
 */
sstr_rc sstr_clear(
    sstring *dst_str
);


/**
 * Clear a string by overwriting it with null characters
 */
sstr_rc sstr_wipe(
    sstring *dst_str
);


/**
 * Swap two strings
 */
sstr_rc sstr_swap(
    sstring *swap1st,
    sstring *swap2nd
);


/**
 * Get a character from a specified position in the string
 */
sstr_rc sstr_getchar(
    const sstring *src_str,
    char          *dst_char,
    sstr_pos      sstr_idx
);


/**
 * Set a character at a specified position in a string
 */
sstr_rc sstr_setchar(
    char     src_char,
    sstring  *dst_str,
    sstr_pos sstr_idx
);


/**
 * Compare two strings
 */
sstr_rc sstr_cmp(
    const sstring *src_str,
    const sstring *pat_str
);


/**
 * Compare the head part of two strings
 */
sstr_rc sstr_startswith(
    const sstring *src_str,
    const sstring *pat_str
);


/**
 * Compare the tail part of two strings
 */
sstr_rc sstr_endswith(
    const sstring *src_str,
    const sstring *pat_str
);


/**
 * Extract a substring from a string
 */
sstr_rc sstr_substr(
    sstring  *src_str,
    sstring  *dst_str,
    sstr_pos start_pos,
    size_t   substr_len
);


/**
 * Extract a substring from a string and append it to another string
 */
sstr_rc sstr_appdsubstr(
    sstring  *src_str,
    sstring  *dst_str,
    sstr_pos start_pos,
    size_t   substr_len
);


/**
 * Find a substring in another string
 */
sstr_pos sstr_indexof(
    const sstring *src_str,
    const sstring *pat_str
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