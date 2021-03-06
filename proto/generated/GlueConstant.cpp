#include <iostream>
#include <glue/glue.h>
#include "deserialize.h"
#include "convert.h"
#include "GlueConstant.h"

namespace Glue
{
    void GlueConstant::glue_import(Json::Value data)
    {
        if (data.isMember("value")) {
            if (!glue_deserialize_float(data["value"], value)) {
                value = 0.0;
            }
        }
    }

    std::string GlueConstant::glue_input_type(int index)
    {
        return "";
    }
    
    std::string GlueConstant::glue_output_type(int index)
    {
        switch (index) {
            case INDEX_VALUE:
                return "float";
            break;
        }

        return "";
    }

    float GlueConstant::glue_get_float(int index, int subindex) {
        switch (index) {
            case INDEX_VALUE:
                return value; 
                break;
        }

        return 0.0;
    }

    void GlueConstant::glue_set_float(int index, int subindex, float value_) {
        switch (index) {
            case INDEX_VALUE:
                value = value_;
                break;
        }
    }

    int GlueConstant::glue_get_int(int index, int subindex)
    {
        switch (index) {
            case INDEX_VALUE:
                return Glue::glue_convert_float_int(glue_get_float(index, subindex));
                break;
        }

        return 0;
    }
}
