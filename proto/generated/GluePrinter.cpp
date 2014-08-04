#include "GluePrinter.h"

namespace Glue
{
    void GluePrinter::glue_import(std::string data)
    {
    }

    void GluePrinter::set_int(int index, int value)
    {
        switch (index) {
            case INDEX_PRINT:
                print(value);
                break;
        }
    }
}
