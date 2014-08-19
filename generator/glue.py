#!/usr/bin/python3

import re, sys, os, codecs
from jinja2 import Template, FileSystemLoader, Environment
from CppHeaderParser import CppHeaderParser3 as headerParser

def glue_type_escape(typename):
    return typename.replace('*', 'star').replace(' ', '_')

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
        # Fixing multi-lines comment
        lines = data.split("\n")
        fixed_lines = []
        for line in lines:
            line = line.strip()
            if len(line) and line[0] == '*':
                line = line[1:]
            fixed_lines += [line.strip()]
        data = ' '.join(fixed_lines)

        # Parsing the data
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
    def __init__(self, meta, name, namespace):
        if 'family' not in meta:
            meta['family'] = 'core'
        if 'name' in meta:
            name = meta['name']
        self.name = name
        self.namespace = namespace
        self.fields = {}
        self.types = []
        self.meta = meta
        self.family = self.meta['family']

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
        field = GlueField(typeName, name, default)
        self.fields[name] = field
        return field

    def add_input_method(self, method, annotation):
        if len(method['parameters']) != 1:
            glue_error("Input method %s::%s() should have exactly one argument"
                    % (self.name, method['name']))
        param = method['parameters'][0]
        field = self.create_field(param['type'], method['name'], annotation)
        field.write = '%s(%s)' % (method['name'], '%s')
        field.attrs += ['input']

    def add_output_method(self, method, annotation):
        if len(method['parameters']) != 0:
            glue_error("Output method %s::%s() should not take any argument"
                    % (self.name, method['name']))
        field = self.create_field(method['rtnType'], method['name'], annotation)
        field.read = '%s()' % method['name']
        field.attrs += ['output']
    
    def add_parameter_method(self, method, annotation):
        if len(method['parameters']) != 1:
            glue_error("Parameter method %s::%s() should have exactly one argument"
                    % (self.name, method['name']))
        param = method['parameters'][0]
        field = self.create_field(param['type'], method['name'], annotation)
        field.attrs += ['editable']
        field.write = '%s(%s)' % (method['name'], '%s')

    def add_input_prop(self, prop, annotation):
        field = self.create_field(prop['type'], method['name'], annotation)
        field.write = '%s = %s' % (prop['name'], '%s')
        field.attrs += ['input']

    def add_output_prop(self, prop, annotation):
        field = self.create_field(prop['type'], prop['name'], annotation)
        field.read = '%s' % prop['name']
        field.attrs += ['output']

    def add_parameter_prop(self, prop, annotation):
        field = self.create_field(prop['type'], prop['name'], annotation)
        field.attrs += ['editable']
        field.write = '%s = %s' % (prop['name'], '%s')

    @classmethod
    def create(cls, annotation, data):
        name = data['name']
        block = GlueBlock(annotation.params, name, data['namespace'])
        
        for visible in data['methods']:
            for method in data['methods'][visible]:
                annotations = GlueAnnotation.get_annotations(method['doxygen'])
                for annotation in annotations:
                    if annotation.name == 'Input':
                        block.add_input_method(method, annotation)
                    if annotation.name == 'Output':
                        block.add_output_method(method, annotation)
                    if annotation.name == 'Parameter':
                        block.add_parameter_method(method, annotation)
        
        for visible in data['properties']:
            for prop in data['properties'][visible]:
                annotations = GlueAnnotation.get_annotations(prop['doxygen'])
                for annotation in annotations:
                    if annotation.name == 'Input':
                        block.add_input_prop(prop, annotation)
                    if annotation.name == 'Output':
                        block.add_output_prop(prop, annotation)
                    if annotation.name == 'Parameter':
                        block.add_parameter_prop(prop, annotation)
               
        return block

class Glue:
    def __init__(self):
        self.compatibilities = {}
        self.blocks = {}
        self.types = []
        self.parsed = []
        self.fields = []
        
        templates_dir = os.path.join(os.path.dirname(__file__), 'templates')
        loader = FileSystemLoader(templates_dir)
        self.env = Environment(loader=loader)
        self.env.filters['te'] = glue_type_escape

    def is_convertible(self, from_type, to_type):
        if from_type in self.compatibilities:
            return to_type in self.compatibilities[from_type]
        else:
            return False

    def add_compatibility(self, from_type, to_type):
        if from_type not in self.compatibilities:
            self.compatibilities[from_type] = []
        if to_type not in self.compatibilities[from_type]:
            self.compatibilities[from_type] += [to_type]

    def set_compatibilities(self, values):
        pairs = values.split(',')
        for pair in pairs:
            parts = pair.split('/')
            if len(parts)==2:
                from_type, to_type = parts
                self.add_compatibility(from_type, to_type)

    def parse(self, filename):
        self.parsed += [filename]
        header = headerParser.CppHeader(filename)
        for cppClass in header.classes:
            self.parse_class(header.classes[cppClass])
                
    def add_block(self, block):
        self.blocks[block.id()] = block
        for typeName in block.types:
            if typeName not in self.types:
                self.types += [typeName]
        for field in block.fields:
            if field not in self.fields:
                self.fields += [field]

    def parse_class(self, classInfo):
        annotations = GlueAnnotation.get_annotations(classInfo['doxygen'])
        for annotation in annotations:
            if annotation.name == 'Block':
                self.add_block(GlueBlock.create(annotation, classInfo))

    def render(self, filename, variables={}):
        template = self.env.get_template(filename)
        variables['glue'] = self
        data = template.render(**variables)

        # Avoid rendering if the file already exist and has
        # the same contents
        filename = os.path.join(self.output_dir, filename)
        try:
            f = codecs.open(filename, 'r', 'utf-8')
            if data == f.read():
                return
        except:
            pass

        outFile = codecs.open(filename, 'w', 'utf-8')
        outFile.write(data)
        outFile.close()

    def generate_files(self, output_dir):
        self.output_dir = output_dir
        self.render('glue.cpp')
        self.render('convert.h')
        self.render('GlueTypes.h')
