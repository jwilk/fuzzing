#include <stdio.h>

#define TIXML_USE_STL 1
#include <tinyxml.h>

int main(int argc, char ** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    TiXmlDocument doc(argv[1]);
    if (!doc.LoadFile()) {
        fprintf(stderr, "invalid XML\n");
        return 1;
    }
    TiXmlPrinter printer;
    doc.Accept(&printer);
    puts(printer.CStr());
    return 0;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
