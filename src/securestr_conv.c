/**
 * secureStrings library
 * version 0.53-beta (2014-08-27_001)
 *
 * secureStrings conversion extensions
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
#include <securestr.h>
#include <securestr_conv.h>


/**
 * Copy a C string to a secureString (overwrite)
 */
sstr_rc sstr_cpycstr(
    const char *src_cstr,
    sstring    *dst_str,
    size_t     cstr_len
)
{
    register sstr_pos sstr_idx;

    if (src_cstr != NULL && dst_str != NULL)
    {
        /* check whether the destination secureString has enough
         * capacity to store the contents of the source C string */
        if (dst_str->cap >= cstr_len)
        {
            for (sstr_idx = 0; sstr_idx < cstr_len; ++sstr_idx)
            {
                dst_str->chars[sstr_idx] = (char) src_cstr[sstr_idx];
            }

            /* update destination secureString length */
            dst_str->len = cstr_len;
            /* terminate destination secureString with a null-character */
            dst_str->chars[dst_str->len] = '\0';

            return SSTR_PASS;
        }
    }

    return SSTR_FAIL;
}


/**
 * Append a C string to a secureString
 */
sstr_rc sstr_appdcstr(
    const char *src_cstr,
    sstring    *dst_str,
    size_t     cstr_len
)
{
    register sstr_pos src_idx;
    register sstr_pos dst_idx;

    if (src_cstr != NULL && dst_str != NULL)
    {
        /* check whether the destination secureString has enough
         * capacity to get appended the contents of the source
         * secureString */
        if ( (dst_str->cap - dst_str->len) >= cstr_len )
        {
            for (src_idx = 0, dst_idx = dst_str->len; src_idx < cstr_len; ++src_idx, ++dst_idx)
            {
                dst_str->chars[dst_idx] = (char) src_cstr[src_idx];
            }

            /* update destination secureString length */
            dst_str->len += cstr_len;
            /* terminate destination secureString with a null-character */
            dst_str->chars[dst_str->len] = '\0';

            return SSTR_PASS;
        }
    }

    return SSTR_FAIL;
}


/**
 * Compare a secureString with a C string
 */
sstr_rc sstr_cmpcstr(
    sstring    *src_str,
    const char *pat_str,
    size_t     cstr_len
)
{
    register sstr_pos sstr_idx;

    if (src_str != NULL && pat_str != NULL)
    {
        if (cstr_len == src_str->len)
        {
            for (sstr_idx = 0; sstr_idx < cstr_len; ++sstr_idx)
            {
                if (src_str->chars[sstr_idx] != (char) pat_str[sstr_idx])
                {
                    return SSTR_FALSE;
                }
            }

            return SSTR_TRUE;
        }
    }

    return SSTR_FAIL;
}