#include "math.h"
#include "Glue.h"

namespace Glue {
    template<>
    int glue_convert<float, int>(float value) {
        return round(value);
    }
}
