var glue_blocks = [
{% for block in glue.blocks.values() %}
// Block {{ block.name }} defined in
// {{ block.file }}
{
    "name": "{{ block.name }}",
    "family": "{{ block.family }}",
    {% for name, meta in block.meta.items() %}
    "{{ name }}": {{ meta }},
    {% endfor %}

    "fields": [
    {% for name, field in block.fields.items() %}
    {
        "name": "{{ field.name }}",
        "type": "{{ field.accessor_type()|te }}{% if field.multiple %}[]{% endif %}",
        {% for name, meta in field.meta.items() %}
        "{{ name }}": {{ meta }},
        {% endfor %}
        {% if field.is_editable() %}
        "defaultValue": {{ field.default }},
        {% endif %}
        "attrs": "{{ field.attributes() }}"
    }{% if not loop.last %},{% endif %}

    {% endfor %}
    ]
}{% if not loop.last %},{% endif%}

{% endfor %}
];

var glue_convertibles = {
    {% for from_type, types in glue.compatibilities.items() %}
    "{{ from_type|te }}": [
    {% for to_type in types %}
    "{{ to_type|te }}"
    {% if not loop.last %},{% endif %}
    {% endfor %}
    ]
    {% if not loop.last %},{% endif %}
    {% endfor %}
};
