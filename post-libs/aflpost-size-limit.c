/* Copyright © 2017 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long limit = ULONG_MAX;

__attribute__((constructor)) void aflpost_size_limit_init(void)
{
    const char *limit_str = getenv("AFL_SIZE_LIMIT");
    if (limit_str) {
        char *ends;
        limit = strtoul(limit_str, &ends, 10);
        if (*ends != '\0')
            limit = ULONG_MAX;
    }
    if (limit == ULONG_MAX) {
        fprintf(stderr, "afl-file-size-limit.so: invalid $AFL_SIZE_LIMIT\n");
        abort();
    }
}

const unsigned char* afl_postprocess(const unsigned char* in_buf, unsigned int* len)
{
    if (*len > limit)
        return NULL;
    return in_buf;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
