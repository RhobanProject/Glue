#ifndef _GLUE_PRINTER_H
#define _GLUE_PRINTER_H

#include <Node.h>
#include "GlueTypes.h"
#include "Printer.h"

namespace Glue
{
    class GluePrinter : public Printer, public Node,
    public Node_set_int
    {
        public:
            void glue_import(std::string data);
            std::string glue_output_type(int index);
            void glue_set_int(int index, int value);
    };
}

#endif
