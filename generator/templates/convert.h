#include <string>
#include <cstdlib>
{% include "headers.cpp" %}

namespace Glue
{
    {% for from in glue.compatibilities %}
    {% for to in glue.compatibilities[from] %}
    {{ to }} glue_convert_{{ from|te }}_{{ to|te }}({{ from }} value);
    {% endfor %}
    {% endfor %}
}
