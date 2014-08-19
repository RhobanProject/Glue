#include <sstream>

namespace Glue
{
    int glue_convert_float_int(float value)
    {
        return (int)value;
    }

    std::string glue_convert_int_string(int value)
    {
        printf("Converting %d to string...\n", value);
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
}
