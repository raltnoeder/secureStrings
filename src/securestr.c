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

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <securestr.h>

#define sstr_version_cstr "0.54-beta (2014-10-25_001)"

const sstring sstr_version_struct =
{
    // chars = string buffer
    sstr_version_cstr,
    // cap   = capacity of the string
    // length of (sstr_version_cstr + '\0') minus 1
    (sizeof (sstr_version_cstr) - 1),
    // len   = length of the string: (len <= cap)
    // length of (sstr_version_cstr + '\0') minus 1
    (sizeof (sstr_version_cstr) - 1)
};
const sstring *sstr_version = &sstr_version_struct;

//
// valid values of the sstr_rc_t datatype
//
const sstr_rc SSTR_PASS  =    (sstr_rc) 0;
const sstr_rc SSTR_FAIL  = (~((sstr_rc) 0));
const sstr_rc SSTR_FALSE =    (sstr_rc) 0;
const sstr_rc SSTR_TRUE  =    (sstr_rc) 1;

// SSTR_SIZE_FAIL is the value returned to indicate an invalid size
const size_t  SSTR_SIZE_FAIL = (~((size_t) 0));

//
// SSTR_NPOS is the value returned to indicate an invalid position in an array
//
const sstr_pos SSTR_NPOS = (~((sstr_pos) 0));


#ifndef _SSTR_NO_DYNMEM
/**
 * Allocate a secureString
 *
 * sstr_cap is the number of char elements that will
 * be available for string processing
 */
sstring *sstr_alloc(
    size_t sstr_cap
)
{
    sstring *dst_str = NULL;

    if (sstr_cap <= SSTR_CAP_MAX)
    {
        char *sstr_chars = malloc(sstr_cap + 1);
        if (sstr_chars != NULL)
        {
            dst_str = malloc(sizeof (sstring));
            if (dst_str != NULL)
            {
                dst_str->chars    = sstr_chars;
                dst_str->cap      = sstr_cap;
                dst_str->len      = 0;
                dst_str->chars[0] = '\0';
            }
            else
            {
                free(sstr_chars);
            }
        }
    }

    return dst_str;
}
#endif /* not _SSTR_NO_DYNMEM */


#ifndef _SSTR_NO_DYNMEM
/**
 * Deallocate a secureString
 */
void sstr_dealloc(
    sstring *dst_str
)
{
    if (dst_str != NULL)
    {
        free(dst_str->chars);
        free(dst_str);
    }
}
#endif /* not _SSTR_NO_DYNMEM */


/**
 * Copy a string to another string (overwrite)
 */
sstr_rc sstr_cpy(
    sstring *src_str,
    sstring *dst_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && dst_str != NULL)
    {
        // check whether the destination secureString has enough
        // capacity to store the contents of the source secureString
        if (dst_str->cap >= src_str->len)
        {
            // copy secureString contents
            for (sstr_pos sstr_idx = 0; sstr_idx < src_str->len; ++sstr_idx)
            {
                dst_str->chars[sstr_idx] = src_str->chars[sstr_idx];
            }

            // update destination secureString length
            dst_str->len = src_str->len;
            // terminate destination secureString with a null-character
            dst_str->chars[dst_str->len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Append a string to another string
 */
sstr_rc sstr_appd(
    sstring *src_str,
    sstring *dst_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && dst_str != NULL)
    {
        // check whether the destination secureString has enough
        // capacity to get appended the contents of the source
        // secureString
        if ((dst_str->cap - dst_str->len) >= src_str->len)
        {
            {
                sstr_pos src_idx = 0;
                sstr_pos dst_idx = dst_str->len;
                while (src_idx < src_str->len)
                {
                    dst_str->chars[dst_idx] = src_str->chars[src_idx];
                    ++src_idx;
                    ++dst_idx;
                }
            }

            // update destination secureString length
            dst_str->len += src_str->len;
            // terminate destination secureString with a null-character
            dst_str->chars[dst_str->len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Append a char to a string
 */
sstr_rc sstr_appdchar(
    char    src_char,
    sstring *dst_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (dst_str != NULL)
    {
        // check whether the destination secureString has enough
        // capacity for an additional character
        if (dst_str->len < dst_str->cap)
        {
            dst_str->chars[dst_str->len] = src_char;

            // update destination secureString length
            ++(dst_str->len);
            // terminate destination secureString with a null-character
            dst_str->chars[dst_str->len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Query the length of a string
 */
size_t sstr_len(
    const sstring *src_str
)
{
    size_t sstr_result = SSTR_SIZE_FAIL;

    if (src_str != NULL)
    {
        sstr_result = src_str->len;
    }

    return sstr_result;
}


/**
 * Query the capacity of a string
 */
size_t sstr_cap(
    const sstring *src_str
)
{
    size_t sstr_result = SSTR_SIZE_FAIL;

    if (src_str != NULL)
    {
        sstr_result = src_str->cap;
    }

    return sstr_result;
}


/**
 * Truncate a string to the specified length
 */
sstr_rc sstr_trunc(
    sstring *dst_str,
    size_t  sstr_len
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (dst_str != NULL)
    {
        if (sstr_len <= dst_str->len)
        {
            dst_str->len = sstr_len;
            dst_str->chars[sstr_len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Clear a string (reset its length to zero)
 */
sstr_rc sstr_clear(
    sstring *dst_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (dst_str != NULL)
    {
        dst_str->len = 0;
        dst_str->chars[0] = '\0';

        sstr_status = SSTR_PASS;
    }

    return sstr_status;
}


/**
 * Clear a string by overwriting it with null characters
 */
sstr_rc sstr_wipe(
    sstring *dst_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (dst_str != NULL)
    {
        // the real capacity of a secureString is cap + 1
        // (one character reserved for a trailing null character)
        // so the 'sstr_idx <= dst_str->cap' is correct and safe
        for (sstr_pos sstr_idx = 0; sstr_idx <= dst_str->cap; ++sstr_idx)
        {
            dst_str->chars[sstr_idx] = '\0';
        }
        dst_str->len = 0;

        sstr_status = SSTR_PASS;
    }

    return sstr_status;
}


/**
 * Swap two strings
 */
sstr_rc sstr_swap(
    sstring *swap1st,
    sstring *swap2nd
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (swap1st != NULL && swap2nd != NULL)
    {
        sstring swaptmp;
        swaptmp.cap    = swap1st->cap;
        swaptmp.len    = swap1st->len;
        swaptmp.chars  = swap1st->chars;

        swap1st->cap   = swap2nd->cap;
        swap1st->len   = swap2nd->len;
        swap1st->chars = swap2nd->chars;

        swap2nd->cap   = swaptmp.cap;
        swap2nd->len   = swaptmp.len;
        swap2nd->chars = swaptmp.chars;

        sstr_status = SSTR_PASS;
    }

    return sstr_status;
}


/**
 * Get a character from a specified position in the string
 */
sstr_rc sstr_getchar(
    const sstring *src_str,
    char          *dst_char,
    sstr_pos      sstr_idx
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && dst_char != NULL)
    {
        if (sstr_idx < src_str->len)
        {
            (*dst_char) = src_str->chars[sstr_idx];
            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Set a character at a specified position in a string
 */
sstr_rc sstr_setchar(
    char     src_char,
    sstring  *dst_str,
    sstr_pos sstr_idx
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (dst_str != NULL)
    {
        if (sstr_idx < dst_str->len)
        {
            dst_str->chars[sstr_idx] = src_char;
            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Compare two strings
 */
sstr_rc sstr_cmp(
    const sstring *src_str,
    const sstring *pat_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && pat_str != NULL)
    {
        if (src_str->len == pat_str->len)
        {
            sstr_pos sstr_idx = 0;
            while (sstr_idx < src_str->len &&
                   src_str->chars[sstr_idx] == pat_str->chars[sstr_idx])
            {
                ++sstr_idx;
            }

            if (sstr_idx == src_str->len)
            {
                sstr_status = SSTR_TRUE;
            }
            else
            {
                sstr_status = SSTR_FALSE;
            }
        }
        else
        {
            sstr_status = SSTR_FALSE;
        }
    }

    return sstr_status;
}


/**
 * Compare the head part of two strings
 */
sstr_rc sstr_startswith(
    const sstring *src_str,
    const sstring *pat_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && pat_str != NULL)
    {
        if (src_str->len >= pat_str->len)
        {
            sstr_pos sstr_idx = 0;
            while (sstr_idx < pat_str->len &&
                   src_str->chars[sstr_idx] == pat_str->chars[sstr_idx])
            {
                ++sstr_idx;
            }

            if (sstr_idx == pat_str->len)
            {
                sstr_status = SSTR_TRUE;
            }
            else
            {
                sstr_status = SSTR_FALSE;
            }
        }
        else
        {
            sstr_status = SSTR_FALSE;
        }
    }

    return sstr_status;
}


/**
 * Compare the tail part of two strings
 */
sstr_rc sstr_endswith(
    const sstring *src_str,
    const sstring *pat_str
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && pat_str != NULL)
    {
        if (src_str->len >= pat_str->len)
        {
            sstr_pos src_idx = src_str->len - pat_str->len;
            sstr_pos pat_idx = 0;
            while (pat_idx < pat_str->len &&
                   src_str->chars[src_idx] == pat_str->chars[pat_idx])
            {
                ++src_idx;
                ++pat_idx;
            }

            if (pat_idx == pat_str->len)
            {
                sstr_status = SSTR_TRUE;
            }
            else
            {
                sstr_status = SSTR_FALSE;
            }
        }
        else
        {
            sstr_status = SSTR_FALSE;
        }
    }

    return sstr_status;
}


/**
 * Extract a substring from a string
 */
sstr_rc sstr_substr(
    sstring  *src_str,
    sstring  *dst_str,
    sstr_pos start_pos,
    size_t   substr_len
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && dst_str != NULL)
    {
        if (src_str->len >= start_pos &&
            (src_str->len - start_pos) >= substr_len &&
            dst_str->cap >= substr_len)
        {
            {
                sstr_pos src_idx = start_pos;
                sstr_pos dst_idx = 0;
                while (dst_idx < substr_len)
                {
                    dst_str->chars[dst_idx] = src_str->chars[src_idx];
                    ++src_idx;
                    ++dst_idx;
                }
            }

            // update destination secureString length
            dst_str->len =  substr_len;
            // terminate destination secureString with a null-character
            dst_str->chars[dst_str->len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Extract a substring from a string and append it to another string
 */
sstr_rc sstr_appdsubstr(
    sstring  *src_str,
    sstring  *dst_str,
    sstr_pos start_pos,
    size_t   substr_len
)
{
    sstr_rc sstr_status = SSTR_FAIL;

    if (src_str != NULL && dst_str != NULL)
    {
        if (src_str->len >= start_pos &&
            (src_str->len - start_pos) >= substr_len &&
            dst_str->cap - dst_str->len >= substr_len)
        {
            sstr_pos final_len = dst_str->len + substr_len;

            {
                sstr_pos src_idx = start_pos;
                sstr_pos dst_idx = dst_str->len;
                while (dst_idx < final_len)
                {
                    dst_str->chars[dst_idx] = src_str->chars[src_idx];
                    ++src_idx;
                    ++dst_idx;
                }
            }

            // update destination secureString length
            dst_str->len = final_len;
            // terminate destination secureString with a null-character
            dst_str->chars[dst_str->len] = '\0';

            sstr_status = SSTR_PASS;
        }
    }

    return sstr_status;
}


/**
 * Find a substring in another string
 */
sstr_pos sstr_indexof(
    const sstring *src_str,
    const sstring *pat_str
)
{
    sstr_pos sstr_index = SSTR_NPOS;

    if (src_str != NULL && pat_str != NULL)
    {
        if (src_str->len >= pat_str->len)
        {
            // the first byte of a string with a length of zero is not valid
            // the empty string always matches at position zero of the
            // source string
            if (pat_str->len > 0)
            {
                sstr_pos search_len = src_str->len - pat_str->len;
                for (sstr_pos src_idx = 0;
                     src_idx <= search_len && sstr_index == SSTR_NPOS;
                     ++src_idx)
                {
                    // compare first character of both strings
                    if (src_str->chars[src_idx] == pat_str->chars[0])
                    {
                        /* first character matches, compare strings */
                        sstr_pos src_off = src_idx + 1;
                        sstr_pos pat_idx = 1;

                        while (pat_idx < pat_str->len &&
                               src_str->chars[src_off] == pat_str->chars[pat_idx])
                        {
                            ++src_off;
                            ++pat_idx;
                        }
                        if (pat_idx == pat_str->len)
                        {
                            sstr_index = src_idx;
                        }
                    }
                }
            }
            else
            {
                sstr_index = 0;
            }
        }
    }

    return sstr_index;
}