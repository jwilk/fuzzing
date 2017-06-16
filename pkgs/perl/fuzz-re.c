#include <EXTERN.h>
#include <perl.h>

static PerlInterpreter *my_perl;

static const char *make_code(uint16_t iflags)
{
    static char buffer[99];
    const char all_flags[] = "msixpaludn";
    char re_flags[sizeof(all_flags)] = "";
    unsigned int i;
    for (i = 0; i < (sizeof all_flags) - 1; i++) {
        uint16_t bitmask = (1U << i);
        if (iflags & bitmask) {
            char buf[2];
            buf[0] = all_flags[i];
            buf[1] = '\0';
            strcat(re_flags, buf);
        }
    }
    sprintf(buffer, "$text =~ qr/$pat/%s", re_flags);
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
        "our ($text, $pat);" \
            "sub v($) { ($text, $pat) = @_; };" \
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
        int rc = read(STDIN_FILENO, buf, (sizeof buf) - 3);
        if (rc < 0)
            abort();
        uint16_t iflags = *((uint16_t*)buf);
        const char *code = make_code(iflags);
        char *text = buf + 2;
        char *pat = text + strlen(text) + 1;
        char *args[] = { text, pat, NULL };
        if (verbose) {
            printf("$text = ");
            print_perl_string(text);
            printf(";\n$pat = ");
            print_perl_string(pat);
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
