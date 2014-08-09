#!/usr/bin/python3

import re
from CppHeaderParser import CppHeaderParser3 as headerParser

def glue_error(message):
    print("Glue fatal error: %s\n" % message)
    exit(1)

class GlueAnnotation:
    def __init__(self, name, params):
        self.name = name
        self.params = params

    def __repr__(self):
        return ('{GlueAnnotation, name=%s, params=%s}' % (self.name, self.params))

    @classmethod
    def get_annotations(cls, data):
        annotations=[]
        matches = re.findall(r"Glue:(.*?)\(([^\)]*?)\)", data, re.MULTILINE|re.DOTALL)
        for match in matches:
            annotations += [GlueAnnotation(match[0], cls.get_params(match[1]))]
        return annotations

    @classmethod
    def get_params(cls, data):
        params = {}
        parts = data.split(';')
        for part in parts:
            if part.strip():
                equal = part.split('=', 1)
                if len(equal)>1:
                    params[equal[0].strip()] = equal[1].strip()
                else:
                    params[part.strip()] = True
        return params

class GlueField:
    def __init__(self, typeName, name, default):
        self.typeName = typeName
        self.name = name
        self.default = default
        self.read = ''
        self.write = ''
        self.attrs = []
        if default:
            self.attrs += ['editable']

class GlueBlock:
    def __init__(self, family, name, namespace):
        self.family = family
        self.name = name
        self.namespace = namespace
        self.fields = []
        self.types = []

    def id(self):
        return '%s.%s' % (self.family, self.name)

    def add_type(self, typeName):
        if typeName not in self.types:
            self.types += [typeName]

    def create_field(self, typeName, name, annotation):
        self.add_type(typeName)
        default = None
        if 'name' in annotation.params:
            name = annotation.params['name']
        if 'default' in annotation.params:
            default = annotation.params['default']
        return GlueField(typeName, name, default)

    def add_field(self, field):
        self.fields += [field]

    def add_input_method(self, method, annotation):
        if len(method['parameters']) != 1:
            glue_error("Input method %s::%s() should have exactly one argument"
                    % (self.name, method['name']))
        param = method['parameters'][0]
        field = self.create_field(param['type'], method['name'], annotation)
        field.write = '%s(%s)' % (method['name'], '%s')
        field.attrs += ['input']
        self.add_field(field)

    def add_output_method(self, method, annotation):
        if len(method['parameters']) != 0:
            glue_error("Output method %s::%s() should not take any argument"
                    % (self.name, method['name']))
        field = self.create_field(method['rtnType'], method['name'], annotation)
        field.read = '%s()' % method['name']
        field.attrs += ['output']
        self.add_fields(field)

    def add_input_prop(self, prop, annotation):
        field = self.create_field(prop['type'], method['name'], annotation)
        field.write = '%s = %s' % (prop['name'], '%s')
        field.attrs += ['input']
        self.add_field(field)

    def add_output_prop(self, prop, annotation):
        field = self.create_field(prop['type'], prop['name'], annotation)
        field.read = '%s' % prop['name']
        field.attrs += ['output']
        self.add_field(field)

    @classmethod
    def create(cls, family, data):
        name = data['name']
        block = GlueBlock(family, name, data['namespace'])
        
        for visible in data['methods']:
            for method in data['methods'][visible]:
                annotations = GlueAnnotation.get_annotations(method['doxygen'])
                for annotation in annotations:
                    if annotation.name == 'Input':
                        block.add_input_method(method, annotation)
                    if annotation.name == 'Output':
                        block.add_output_method(method, annotation)
        
        for visible in data['properties']:
            for prop in data['properties'][visible]:
                annotations = GlueAnnotation.get_annotations(prop['doxygen'])
                for annotation in annotations:
                    if annotation.name == 'Input':
                        block.add_input_prop(prop, annotation)
                    if annotation.name == 'Output':
                        block.add_output_prop(prop, annotation)
               
        return block

class Glue:
    def __init__(self):
        self.blocks = {}
        self.types = []

    def parse(self, filename):
        header = headerParser.CppHeader(filename)
        for cppClass in header.classes:
            self.parse_class(header.classes[cppClass])
                
    def add_block(self, block):
        self.blocks[block.id()] = block
        for typeName in block.types:
            if typeName not in self.types:
                self.types += [typeName]

    def parse_class(self, classInfo):
        annotations = GlueAnnotation.get_annotations(classInfo['doxygen'])
        for annotation in annotations:
            if annotation.name == 'Block':
                family = 'core'
                if 'family' in annotation.params:
                    family = annotation.params['family']
                self.add_block(GlueBlock.create(family, classInfo))

    def generate_files(self, output_dir):
        open(output_dir+'/glue.cpp', 'w').write('')
