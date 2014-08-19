#include <vector>
#include <map>
#include <set>
#include <string>
{% for header in glue.headers %}
#include "{{ header }}"
{% endfor %}
