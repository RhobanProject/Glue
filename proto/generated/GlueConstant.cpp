#include "GlueConstant.h"

namespace Glue
{
    void GlueConstant::glue_import(std::string data)
    {
        if (!glue_deserialize<float>(data, value)) {
            value = 0.0;
        }
    }
    
    std::string GlueConstant::glue_output_type(int index)
    {
        switch (index) {
            case INDEX_VALUE:
                return "float";
            break;
        }
    }

    float GlueConstant::glue_get_float(int index) {
        switch (index) {
            case INDEX_VALUE:
                return value; 
                break;
        }
    }

    void GlueConstant::glue_set_float(int index, float value_) {
        switch (index) {
            case INDEX_VALUE:
                value = value_;
                break;
        }
    }

    int GlueConstant::glue_get_int(int index)
    {
        switch (index) {
            case INDEX_VALUE:
                return Glue::glue_convert<float, int>(glue_get_float(index));
                break;
        }
    }
}
