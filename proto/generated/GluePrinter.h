#ifndef _GLUE_PRINTER_H
#define _GLUE_PRINTER_H

#include "GlueTypes.h"
#include "../Node.h"
#include "../Printer.h"

namespace Glue
{
    class GluePrinter : public Printer, public Node,
    public Node_set_int
    {
        public:
            void glue_import(std::string data);
            void set_int(int index, int value);
    };
}

#endif
