/**
 * secureStrings library
 * version 0.52-beta (2014-07-02_001)
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


#ifndef _SECURESTR_CONV_H
#define _SECURESTR_CONV_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <securestr.h>

/**
 * sizeof(text) is the length of a text including its
 * trailing null byte
 */
#define SSTRING(text) ((sstring *) &((sstring) { text, \
        (sizeof(text) - 1), \
        (sizeof(text) - 1) }))

sstr_rc sstr_cpycstr(
    const char *,
    sstring *,
    size_t
);

sstr_rc sstr_appdcstr(
    const char *,
    sstring *,
    size_t
);

sstr_rc sstr_cmpcstr(
    sstring *,
    const char *,
    size_t
);

#define sstrCpyCstr     sstr_cpycstr
#define sstrAppdCstr    sstr_appdcstr
#define sstrCmpCstr     sstr_cmpcstr

#endif /* _SECURESTR_CONV_H */
