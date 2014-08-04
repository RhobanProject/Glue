#!/usr/bin/python3

import sys
from glue import Glue

glue = Glue()
files = sys.argv[1:]
for headerFile in files:
    glue.parse(headerFile)

