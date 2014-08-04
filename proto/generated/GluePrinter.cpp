#include "GluePrinter.h"

namespace Glue
{
    void GluePrinter::set_int(int index, int value)
    {
        switch (index) {
            case INDEX_PRINT:
                print(value);
                break;
        }
    }
}
