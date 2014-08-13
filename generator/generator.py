#!/usr/bin/python3

import sys, os
from glue import Glue

# The Glue generator, generates:
#
# - glue.cpp: glue functions
#       glue_getter, glue_setter, glue_link, glue_name_to_index,
#       glue_instanciate, glue_is_convertible
# - deserialize.h, convert.h: headers for methods that converts and
#   deserializes compatible types
# - GlueTypes.h: all type classes (Node_get_* and Node_set_*), with
#       their methods glue_import, glue_output_type, glue_get_* and glue_set_*
# - Glue*.cpp: all the special classes inheriting
# - blocks.json: blocks meta descriptions

glue = Glue()

# Getting output directory, creating it if necessary
output_dir = sys.argv[1]
if not os.path.isdir(output_dir):
    os.makedirs(output_dir)

# Type compatibilities
compatibilities = sys.argv[2]
glue.set_compatibilities(compatibilities)

# Parse the headers
files = sys.argv[3:]
for headerFile in files:
    glue.parse(headerFile)

glue.generate_files(output_dir)
