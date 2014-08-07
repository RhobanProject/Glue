#!/usr/bin/python3

import sys, os
from glue import Glue

glue = Glue()

# Getting output directory, creating it if necessary
output_dir = sys.argv[1]
if not os.path.isdir(output_dir):
    os.makedirs(output_dir)

# Parse the headers
files = sys.argv[2:]
for headerFile in files:
    glue.parse(headerFile)

open(output_dir+'/glue.cpp', 'w').write('')
