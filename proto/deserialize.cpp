#include <string>
#include <cstdlib>
#include "Glue.h"

namespace Glue
{
    template<>
        bool glue_deserialize(std::string data, float &value)
        {
            value = atof(data.c_str());
            return true;
        }
}
