#!/usr/bin/python3

import sys, os, re, codecs

class Template(object):
    def __init__(self, filename):
        self.content = open(os.path.join(os.path.dirname(__file__), 'templates', filename), 'r', encoding='utf-8').read()
        self.variables = {}

    def render(self, filename):
        data = self.content

        for name, value in self.variables.items():
            data = data.replace('%'+name+'%', value)

        # Avoid rendering if the file already exist and has
        # the same contents
        try:
            f = codecs.open(filename, 'r', 'utf-8')
            if data == f.read():
                return
        except:
            pass

        outFile = codecs.open(filename, 'w', 'utf-8')
        outFile.write(data)
        outFile.close()

    def set(self, variable, name):
        self.variables[variable] = str(name)

    def append(self, variable, value):
        self.variables[variable] = self.variables.get(variable, '') + str(value)

