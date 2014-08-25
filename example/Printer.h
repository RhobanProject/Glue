#ifndef _PRINTER_H
#define _PRINTER_H

#include <string>

/**
 * Glue:Block(family=Output)
 */
class Printer
{
    public:
        /**
         * Glue:Input()
         */
        void print(std::string value);
};

#endif
