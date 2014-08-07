#include <string>
#include <cstdlib>
#include <json/json.h>

namespace Glue
{
    bool glue_deserialize_float(Json::Value data, float &value);
    bool glue_deserialize_int(Json::Value data, int &value);
    bool glue_deserialize_bool(Json::Value data, bool &value);
    bool glue_deserialize_string(Json::Value data, std::string &value);
}
