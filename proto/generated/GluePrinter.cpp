#include "GluePrinter.h"

namespace Glue
{
    void GluePrinter::glue_import(Json::Value data)
    {
    }
            
    std::string GluePrinter::glue_input_type(int index)
    {
        switch (index) {
            case INDEX_PRINT:
                return "int";
            break;
        }

        return "";
    }
            
    std::string GluePrinter::glue_output_type(int index)
    {
        return "";
    }

    void GluePrinter::glue_set_int(int index, int subindex, int value)
    {
        switch (index) {
            case INDEX_PRINT:
                print(value);
                break;
        }
    }
}
