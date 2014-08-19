#include <glue/glue.h>
{% include "headers.cpp" %}

namespace Glue
{
{% for type in glue.types %}
    template<>    
        {{ type }} glue_getter<{{ type }}>(Node *node, int index, int subindex)
        {    
            Node_get_{{ type|te }} *n = dynamic_cast<Node_get_{{ type|te }} *>(node);   
            return n->glue_get_{{ type|te }}(index, subindex);
        }    

    template<>    
        void glue_setter<{{ type }}>(Node *node, int index, int subindex, {{ type }} value)   
        {    
            Node_set_{{ type|te }} *n = dynamic_cast<Node_set_{{ type|te }} *>(node);
            n->glue_set_{{ type|te }}(index, subindex,value);
        }  
{% endfor %}
    
    LinkBase *glue_link(std::string type, Node *from, int from_index, int from_subindex,
            Node *to, int to_index, int to_subindex)
    {
        {% for type in glue.types %}
        if (type == "{{ type }}" ) {
            return new Link<{{ type }}>(from, from_index, from_subindex, to, to_index, to_subindex);
        }
        {% endfor %}

        return NULL;
    }

    int glue_name_to_index(std::string name)
    {
        {% for field in glue.fields %}
        if (name == "{{ field }}") {
            return INDEX_{{ field|upper }};
        }
        {% endfor %}

        return -1;
    }

    Node *glue_instanciate(std::string family, std::string type, Json::Value data)
    {
        Node *node = NULL;
        {% for block in glue.blocks.values() %}
        if (family == "{{ block.family }}" && type == "{{ block.name }}") {
            node = new Glue{{ block.name }};
        }
        {% endfor %}
        if (node != NULL) {
            node->glue_import(data);
        }

        return node;
    }

    bool glue_is_convertible(std::string from, std::string to)
    {
        if (from == to) {
            return true;
        }
 
        {% for from in glue.compatibilities %}
        {% for to in glue.compatibilities[from] %}
        if (from == "{{ from }}" && to == "{{ to }}") {
            return true;
        }
        {% endfor %}
        {% endfor %}

        return false;
    }
}
