#ifndef _GLUE_CONSTANT_H
#define _GLUE_CONSTANT_H

#include <glue/Node.h>
#include "GlueTypes.h"
#include "../Constant.h"

namespace Glue
{
    class GlueConstant : public Constant, public Node,
    public Node_set_float, public Node_get_float,
    public Node_get_int
    {
        public:
            void glue_import(Json::Value data);
            float glue_get_float(int index, int subindex);
            void glue_set_float(int index, int subindex, float value_);
            int glue_get_int(int index, int subindex);
            std::string glue_output_type(int index);
    };
}

#endif
