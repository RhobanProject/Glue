#include <vector>
#include <map>
#include <set>
#include <string>
{% for header in glue.parsed %}
#include "{{ header }}"
{% endfor %}
