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
    
    template<>
        bool glue_deserialize(Json::Value data, int &value)
        {
            if (data.isNumeric()) {
                value = data.asInt();
                return true;
            } else {
                return true;
            }
        } 
    
    template<>
        bool glue_deserialize(Json::Value data, bool &value)
        {
            if (data.isBool()) {
                value = data.asBool();
                return true;
            } else {
                return true;
            }
        } 
    
    template<>
        bool glue_deserialize(Json::Value data, std::string &value)
        {
            if (data.isString()) {
                value = data.asString();
                return true;
            } else {
                return true;
            }
        }
}
