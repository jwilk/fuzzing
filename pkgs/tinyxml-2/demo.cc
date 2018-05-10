#include <stdio.h>

#include <tinyxml2.h>

using namespace tinyxml2;

int main(int argc, char ** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    XMLDocument* doc = new XMLDocument();
    if (doc->LoadFile(argv[1]) != XML_NO_ERROR) {
        doc->PrintError();
        return 1;
    }
    XMLPrinter printer;
    doc->Print(&printer);
    puts(printer.CStr());
    return 0;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
