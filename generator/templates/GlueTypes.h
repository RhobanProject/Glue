#ifndef _GLUE_TYPES_H
#define _GLUE_TYPES_H

#include <glue/glue.h>
{% include "headers.cpp" %}

namespace Glue
{
    enum Indexes {
        {% for field in glue.fields %}
            INDEX_{{ field|upper }} {% if not loop.last %},{% endif %}
        {% endfor %}
    };

    {% for type in glue.types %}
    class Node_{{ type|te }}
    {
        public:
            virtual {{ type }} glue_get_{{ type|te }}(int index, int subindex)=0;
            virtual void glue_set_{{ type|te }}(int index, int subindex, {{ type }} value)=0;
    };
    {% endfor %}
}

#endif
