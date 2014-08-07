#include <glue/glue.h>
#include <math.h>

namespace Glue {
    int glue_convert_float_int(float value) {
        return round(value);
    }
}
