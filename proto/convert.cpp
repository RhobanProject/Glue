#include "math.h"
#include "glue.h"

namespace Glue {
    template<>
    int glue_convert<float, int>(float value) {
        return round(value);
    }
}
