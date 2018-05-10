#include <iostream>
#include <sstream>

#include <snappy.h>

std::string slurp()
{
    std::ostringstream ss;
    ss << std::cin.rdbuf();
    return ss.str();
}

int main(int argc, char **argv)
try
{
    std::string input, output;
    input = slurp();
    if (argc == 2 && std::string(argv[1]) == std::string("-z"))
        snappy::Compress(input.data(), input.size(), &output);
    else
        snappy::Uncompress(input.data(), input.size(), &output);
    std::cout << output;
    return 0;
}
catch (std::bad_alloc)
{
    return 1;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
