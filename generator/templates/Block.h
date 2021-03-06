#include <vector>
#include <map>
#include <set>
#include <string>
#include <glue/Node.h>
#include "GlueTypes.h"

{% for block in blocks %}
#ifndef _GLUE_{{ block.name }}_H
#define _GLUE_{{ block.name }}_H

#include "{{ block.file }}"

namespace Glue
{
    class Glue{{ block.name }} : public {{ block.fullclass }}, public Node,
    {% for type in block.types %}
    public Node_{{ type|te }} {% if not loop.last %},{% endif %}
    {% endfor %}
    {   
        public:
            void glue_import(Json::Value data);

            {% for type in block.types %}
            {{ type }} glue_get_{{ type|te }}(int index, int subindex);
            void glue_set_{{ type|te }}(int index, int subindex, {{ type }} value_);
            {% endfor %}

            std::string glue_input_type(int index);
            std::string glue_output_type(int index);
            
            void glue_prepare(int index, int subindex);
            
            void glue_tick(float elapsed);

            {% for event in block.all_events() %}
            void glue_{{ event }}();
            {% endfor %}
    };  
}

#endif
{% endfor %}
