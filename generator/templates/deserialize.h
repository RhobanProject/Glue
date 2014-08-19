#ifndef _GLUE_DESERIALIZE_H
#define _GLUE_DESERIALIZE_H
#include <string>
#include <cstdlib>
{% include "headers.cpp" %}

namespace Glue
{
    {% for type in glue.deserialize %}
    bool glue_deserialize_{{ type|te }}(Json::Value data, {{ type }} &value);
    {% endfor %}
}
#endif
