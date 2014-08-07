#include <string>
#include <cstdlib>
#include <glue/glue.h>

namespace Glue
{
    template<>
        bool glue_deserialize(Json::Value data, float &value)
        {
            if (data.isNumeric()) {
                value = data.asFloat();
                return true;
            } else {
                return true;
            }
        }
}
