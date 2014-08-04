#ifndef _GLUE_CONSTANT_H
#define _GLUE_CONSTANT_H

#include "GlueTypes.h"
#include "../Constant.h"

namespace Glue
{
    class GlueConstant : public Constant, public Node,
    public Node_set_float, public Node_get_float,
    public Node_get_int
    {
        public:
            float get_float(int index);
            void set_float(int index, float value_);
            int get_int(int index);
    };
}

#endif
