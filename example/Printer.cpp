#include <cstdio>
#include "Printer.h"

void Printer::print(std::string value)
{
    printf("Printer: %s\n", value.c_str());
}
