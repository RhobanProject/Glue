#include <string>
#include <cstdlib>
#include <json/json.h>

namespace Glue
{
    bool glue_deserialize_float(Json::Value data, float &value)
    {
        if (data.isNumeric()) {
            value = data.asFloat();
            return true;
        } else if (data.isString()) {
            value = atof(data.asString().c_str());
            return true;
        } else {
            return false;
        }
    }

    bool glue_deserialize_int(Json::Value data, int &value)
    {
        if (data.isNumeric()) {
            value = data.asInt();
            return true;
        } else if (data.isString()) {
            value = atoi(data.asString().c_str());
            return true;
        } else {
            return false;
        }
    } 

    bool glue_deserialize_bool(Json::Value data, bool &value)
    {
        if (data.isBool()) {
            value = data.asBool();
            return true;
        } else if (data.isString()) {
            std::string str = data.asString();
            value = !(str == "0" || str == "false" || str == "");
            return true;
        } else {
            return false;
        }
    } 

    bool glue_deserialize_string(Json::Value data, std::string &value)
    {
        if (data.isString()) {
            value = data.asString();
            return true;
        } else {
            return false;
        }
    }
}
