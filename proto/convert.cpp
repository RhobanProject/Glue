#include <glue/glue.h>
#include <math.h>

namespace Glue {
    template<>
    int glue_convert<float, int>(float value) {
        return round(value);
    }
}
