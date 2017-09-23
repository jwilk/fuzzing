/*
 * Copyright © 2017 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <EXTERN.h>
#include <perl.h>

static PerlInterpreter *my_perl;

#define asizeof(a) (sizeof a / sizeof a[0])

static const char *make_code(uint16_t iflags)
{
    const char* all_flags[] = {
        "m",
        "s",
        "i",
        "x",
        "p",
        "a",
        "l",
        "u",
        "d",
        "n",
        "xx",
        "aa",
        "r"
    };
    char re_flags[1 + 2 * asizeof(all_flags)] = "";
    static char buffer[23 + 2 * asizeof(all_flags)];
    unsigned int i;
    for (i = 0; i < asizeof(all_flags); i++) {
        uint16_t bitmask = (1U << i);
        if (iflags & bitmask)
            strcat(re_flags, all_flags[i]);
    }
    sprintf(buffer, "$text =~ s/$pat/$repl/%s", re_flags);
    return buffer;
}

static void print_perl_string(const char *s)
{
    putchar('"');
    for (; *s; s++) {
        unsigned char c = *s;
        if (c == '"' || c == '\\')
            printf("\\%c", c);
        else if (c < ' ' || c >= 0x7FU)
            printf("\\x%02x", c);
        else
            putchar(c);
    }
    putchar('"');
}

int main(int argc, char **argv, char **env)
{
    char *fuzzy_argv[] = {
        "",
        "-E",
        "use strict; use warnings;" \
        "our ($text, $pat, $repl);" \
            "sub v($) { ($text, $pat, $repl) = @_; };" \
            "0;",
        NULL,
    };
    int verbose = isatty(STDOUT_FILENO);
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    perl_parse(my_perl, NULL, 3, fuzzy_argv, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
#ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
#endif
    {
        char buf[99999];
        memset(buf, 0, sizeof buf);
        int rc = read(STDIN_FILENO, buf, (sizeof buf) - 4);
        if (rc < 0)
            abort();
        uint16_t iflags = *((uint16_t*)buf);
        const char *code = make_code(iflags);
        char *text = buf + 2;
        char *pat = text + strlen(text) + 1;
        char *repl = pat + strlen(pat) + 1;
        char *args[] = { text, pat, repl, NULL };
        if (verbose) {
            printf("$text = ");
            print_perl_string(text);
            printf(";\n$pat = ");
            print_perl_string(pat);
            printf(";\n$repl = ");
            print_perl_string(repl);
            printf(";\n%s;\n", code);
        }
        call_argv("v", G_DISCARD, args);
        SV* v = eval_pv(code, FALSE);
        SvIV(v);
    }
#ifdef __AFL_HAVE_MANUAL_CONTROL
    _exit(0);
#endif
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
}

/* vim:set ts=4 sts=4 sw=4 et:*/
