#!/usr/bin/python3

import sys, os, shutil
from glue import Glue, GlueException

try:
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
    glue_dir = sys.argv[1]

    # Getting output directory, creating it if necessary
    output_dir = sys.argv[2]
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    # Getting output directory for web files
    web_dir = sys.argv[3]
    if os.path.isdir(web_dir):
        shutil.rmtree(web_dir)

    # Type compatibilities
    compatibilities = sys.argv[4]
    if compatibilities != ' ':
        glue.set_compatibilities(compatibilities)

    # Additional headers
    headers = sys.argv[5]
    if headers != ' ':
        glue.headers = headers.split(',')

    # Parse the headers
    files = sys.argv[6:]
    for headerFile in files:
        glue.parse(headerFile)

    glue.generate_files(glue_dir, output_dir, web_dir)
except GlueException as e:
    print()
    print('! '+str(e))
    print()
    exit(1)
