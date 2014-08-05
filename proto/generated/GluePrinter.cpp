#include "GluePrinter.h"

namespace Glue
{
    void GluePrinter::glue_import(std::string data)
    {
    }
            
    std::string GluePrinter::glue_output_type(int index)
    {
        switch (index) {
            case INDEX_PRINT:
                return "int";
            break;
        }
    }

    void GluePrinter::glue_set_int(int index, int value)
    {
        switch (index) {
            case INDEX_PRINT:
                print(value);
                break;
        }
    }
}
