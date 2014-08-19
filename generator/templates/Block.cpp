#include <sstream>
#include <iostream>
#include <glue/glue.h>
#include "deserialize.h"
#include "convert.h"
#include "Glue{{ file }}.h"

namespace Glue
{
{% for block in blocks %}
    void Glue{{ block.name }}::glue_import(Json::Value data)
    {
        {% for field in block.fields.values() %}{% if field.is_editable() %}
        if (data.isMember("{{ field.name }}")) {
            {{ field.type }} _glue_tmp;
            if (glue_deserialize_{{ field.type|te }}(data["{{ field.name }}"], _glue_tmp)) {
                {{ field.set("_glue_tmp") }};
            } else {
                {% if field.default %}
                std::string _glue_json = "{{ field.default }}";
                Json::Reader _glue_reader;
                Json::Value _glue_value;

                if (_glue_reader.parse(_glue_json, _glue_value)) {
                    if (glue_deserialize_{{ field.type|te }}(_glue_value, _glue_tmp)) {
                        {{ field.set("_glue_tmp") }};
                    }
                }
                {% endif %}
            }
        }
        {% endif %}{% endfor %}
    }

    std::string Glue{{ block.name }}::glue_input_type(int index)
    {
        switch (index) {
            {% for field in block.fields.values() %}
            {% if field.is_input() %}
            case INDEX_{{ field.name|upper }}:
                return "{{ field.type }}";
            break;
            {% endif %}
            {% endfor %}
        }

        return "";
    }
    
    std::string Glue{{ block.name }}::glue_output_type(int index)
    {
        switch (index) {
            {% for field in block.fields.values() %}
            {% if field.is_output() %}
            case INDEX_{{ field.name|upper }}:
                return "{{ field.type }}";
            break;
            {% endif %}
            {% endfor %}
        }

        return "";
    }

{% for type in block.types %}
    {{ type }} Glue{{ block.name }}::glue_get_{{ type|te }}(int index, int subindex) {
        switch (index) {
            {% for field in block.fields.values() %}
            {% if field.is_output() and field.type==type %}
            case INDEX_{{ field.name|upper }}:
                return {{ field.get_sub("subindex") }};
            break;
            {% endif %}
            {% if field.is_output() and field.is_convertible_to(type) %}
            case INDEX_{{ field.name|upper }}:
                return Glue::glue_convert_{{ field.type|te }}_{{ type|te }}({{ field.get_sub("subindex") }});
            break;
            {% endif %}
            {% endfor %}
        }

        std::ostringstream oss;
        oss << "Unknown index " << index << " for block {{ block.name }}";
        throw oss.str();
    }

    void Glue{{ block.name }}::glue_set_{{ type|te }}(int index, int subindex, {{ type }} _glue_value) {
        switch (index) {
            {% for field in block.fields.values() %}
            {% if field.is_input() and field.type==type %}
            case INDEX_{{ field.name|upper }}:
                {{ field.set_sub("_glue_value", "subindex") }};
            break;
            {% endif %}
            {% endfor %}
        }
    }
{% endfor %}
{% endfor %}
}
