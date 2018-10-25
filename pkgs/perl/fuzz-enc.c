/*
 * Copyright © 2017-2018 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <EXTERN.h>
#include <perl.h>

static PerlInterpreter *my_perl;

static char * encodings[] = {
    "7bit-jis",
    "AdobeStandardEncoding",
    "AdobeSymbol",
    "AdobeZdingbat",
    "ascii",
    "ascii-ctrl",
    "big5-eten",
    "big5-hkscs",
    "cp1006",
    "cp1026",
    "cp1047",
    "cp1250",
    "cp1251",
    "cp1252",
    "cp1253",
    "cp1254",
    "cp1255",
    "cp1256",
    "cp1257",
    "cp1258",
    "cp37",
    "cp424",
    "cp437",
    "cp500",
    "cp737",
    "cp775",
    "cp850",
    "cp852",
    "cp855",
    "cp856",
    "cp857",
    "cp858",
    "cp860",
    "cp861",
    "cp862",
    "cp863",
    "cp864",
    "cp865",
    "cp866",
    "cp869",
    "cp874",
    "cp875",
    "cp932",
    "cp936",
    "cp949",
    "cp950",
    "dingbats",
    "euc-cn",
    "euc-jp",
    "euc-kr",
    "gb12345-raw",
    "gb2312-raw",
    "gsm0338",
    "hp-roman8",
    "hz",
    "iso-2022-jp",
    "iso-2022-jp-1",
    "iso-2022-kr",
    "iso-8859-1",
    "iso-8859-10",
    "iso-8859-11",
    "iso-8859-13",
    "iso-8859-14",
    "iso-8859-15",
    "iso-8859-16",
    "iso-8859-2",
    "iso-8859-3",
    "iso-8859-4",
    "iso-8859-5",
    "iso-8859-6",
    "iso-8859-7",
    "iso-8859-8",
    "iso-8859-9",
    "iso-ir-165",
    "jis0201-raw",
    "jis0208-raw",
    "jis0212-raw",
    "johab",
    "koi8-f",
    "koi8-r",
    "koi8-u",
    "ksc5601-raw",
    "MacArabic",
    "MacCentralEurRoman",
    "MacChineseSimp",
    "MacChineseTrad",
    "MacCroatian",
    "MacCyrillic",
    "MacDingbats",
    "MacFarsi",
    "MacGreek",
    "MacHebrew",
    "MacIcelandic",
    "MacJapanese",
    "MacKorean",
    "MacRoman",
    "MacRomanian",
    "MacRumanian",
    "MacSami",
    "MacSymbol",
    "MacThai",
    "MacTurkish",
    "MacUkrainian",
    "MIME-B",
    "MIME-Header",
    "MIME-Header-ISO_2022_JP",
    "MIME-Q",
    "nextstep",
    "null",
    "posix-bc",
    "shiftjis",
    "symbol",
    "UCS-2BE",
    "UCS-2LE",
    "UTF-16",
    "UTF-16BE",
    "UTF-16LE",
    "UTF-32",
    "UTF-32BE",
    "UTF-32LE",
    "UTF-7",
    "utf-8-strict",
    "utf8",
    "viscii"
};

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

EXTERN_C void boot_DynaLoader(pTHX_ CV* cv);

EXTERN_C void xs_init(pTHX)
{
    char *file = __FILE__;
    newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}

int main(int argc, char **argv, char **env)
{
    char *fuzzy_argv[] = {
        "",
        "-E", "use Encode;",
        NULL,
    };
    int verbose = isatty(STDOUT_FILENO);
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    perl_parse(my_perl, xs_init, 3, fuzzy_argv, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    {
        size_t i;
        for (i = 0; i < sizeof(encodings) / sizeof(encodings[0]); i++) {
            char *encoding = encodings[i];
            char *args[] = { encoding, "", NULL };
            call_argv("decode", G_DISCARD, args);
        }
    }
#ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
#endif
    {
        char buf[99999];
        memset(buf, 0, sizeof buf);
        int rc = read(STDIN_FILENO, buf, (sizeof buf) - 3);
        if (rc < 0)
            abort();
        unsigned char encoding_no = buf[0];
        if (encoding_no < sizeof(encodings) / sizeof(encodings[0])) {
            char *encoding = encodings[encoding_no];
            char *text = buf + 1;
            char *args[] = { encoding, text, NULL };
            if (verbose) {
                printf("%s\ndecode(", fuzzy_argv[2]);
                print_perl_string(encoding);
                printf(", ");
                print_perl_string(text);
                printf(");\n");
            }
            call_argv("decode", G_DISCARD, args);
        }
    }
#ifdef __AFL_HAVE_MANUAL_CONTROL
    _exit(0);
#endif
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
}

/* vim:set ts=4 sts=4 sw=4 et:*/
