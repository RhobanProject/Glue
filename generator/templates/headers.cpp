{% for header in glue.parsed %}
#include "{{ header }}"
{% endfor %}
