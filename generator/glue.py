#!/usr/bin/python3

import re, sys, os, codecs
from jinja2 import Template, FileSystemLoader, Environment
from CppHeaderParser import CppHeaderParser3 as headerParser

def glue_type_escape(typename):
    if typename=='std::string':
        return 'string'
    return typename.replace('*', 'star').replace(' ', '_').replace('::', '__')

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
    def __init__(self, typeName, name):
        self.type = typeName
        self.name = name
        self.default = None
        self.read = ''
        self.read_sub = ''
        self.write = ''
        self.write_sub = ''
        self.prepare = ''
        self.attrs = []
        self.multiple = False
        self.convertible = []

    def get_prepare(self, subindex):
        if self.prepare:
            return self.prepare % subindex
        else:
            return ''

    def add_convertible(self, convertible):
        if convertible not in self.convertible:
            self.convertible += [convertible]

    def is_convertible_to(self, to):
        return (to in self.convertible)

    def set_default(self, default):
        self.default = default
        self.attrs += ['editable']

    def is_input(self):
        return ('input' in self.attrs)

    def is_output(self):
        return ('output' in self.attrs)

    def is_editable(self):
        return ('editable' in self.attrs)

    def get(self):
        return self.read

    def get_sub(self, sub):
        if self.multiple:
            return self.read_sub % sub
        else:
            return self.read

    def set(self, value):
        return self.write % (value)

    def set_sub(self, value, sub):
        if self.multiple:
            return self.write_sub % (sub, value)
        else:
            return self.write % value

class GlueBlock:
    def __init__(self, meta, name, namespace):
        self.classname = name
        self.fullclass = name
        if 'family' not in meta:
            meta['family'] = 'core'
        if 'name' in meta:
            name = meta['name']
        self.name = name
        self.namespace = namespace
        if namespace:
            self.fullclass = namespace+'::'+self.fullclass
        self.fields = {}
        self.types = []
        self.meta = meta
        self.family = self.meta['family']
        self.file = ''

    def id(self):
        return '%s.%s' % (self.family, self.name)

    def add_type(self, typeName):
        if typeName not in self.types:
            self.types += [typeName]

    def create_field(self, typeName, name, annotation):
        if name in self.fields:
            field = self.fields[name]
        else:
            self.add_type(typeName)
            default = None
            if 'name' in annotation.params:
                name = annotation.params['name']
            field = GlueField(typeName, name)
            self.fields[name] = field
        
        if 'default' in annotation.params:
            field.set_default(annotation.params['default'])

        return field

    def add_input_method(self, method, annotation):
        multiple = False
        if len(method['parameters']) == 0 or len(method['parameters'])>2:
            glue_error("Input method %s::%s() should have one or two arguments"
                    % (self.name, method['name']))
        if len(method['parameters'])==1:
            param = method['parameters'][0]
        if len(method['parameters'])==2:
            if method['parameters'][0]['type'] != 'int':
                glue_error('Input method %s::%s() is multiple, first argument should be int'
                    % (self.name, method['name']))
            param = method['parameters'][1]
            multiple = True
        field = self.create_field(param['type'], method['name'], annotation)
        field.multiple = multiple
        field.write = '%s(%s)' % (method['name'], '%s')
        field.write_sub = '%s(%s, %s)' % (method['name'], '%s', '%s')
        field.attrs += ['input']

    def add_output_method(self, method, annotation):
        multiple = False
        if len(method['parameters']) > 1:
            glue_error("Output method %s::%s() should take 0 or 1 argument"
                    % (self.name, method['name']))
        if len(method['parameters'])==1:
            if method['parameters'][0]['type'] != 'int':
                glue_error("Output method %s::%s() is multiple, its argument should be int"
                        % (self.name, method['name']))
            multiple = True
        field = self.create_field(method['rtnType'], method['name'], annotation)
        field.multiple = multiple
        field.read = '%s()' % method['name']
        field.read_sub = '%s(%s)' % (method['name'], '%s')
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
        multiple = False
        prepare = ''
        typeName = prop['type']
        if typeName.startswith('std::map<'):
            typeName = typeName[9:-1]
            multiple = True
        if typeName.startswith('std::vector<'):
            prepare = '%s.resize(%s+1)' % (prop['name'], '%s')
            typeName = typeName[12:-1]
            multiple = True
        field = self.create_field(typeName, prop['name'], annotation)
        field.multiple = multiple
        field.read = '%s' % prop['name']
        if prepare:
            field.prepare = prepare
        field.write = '%s = %s' % (prop['name'], '%s')
        field.read_sub = '%s[%s]' % (prop['name'], '%s')
        field.write_sub = '%s[%s] = %s' % (prop['name'], '%s', '%s')
        field.attrs += ['input']

    def add_output_prop(self, prop, annotation):
        multiple = False
        typeName = prop['type']
        prepare = ''
        if typeName.startswith('std::map<'):
            typeName = typeName[9:-1]
            multiple = True
        if typeName.startswith('std::vector<'):
            prepare = '%s.resize(%s+1)' % (prop['name'], '%s')
            typeName = typeName[12:-1]
            multiple = True
        field = self.create_field(typeName, prop['name'], annotation)
        field.multiple = multiple
        if prepare:
            field.prepare = prepare
        field.read = '%s' % prop['name']
        field.write = '%s = %s' % (prop['name'], '%s')
        field.read_sub = '%s[%s]' % (prop['name'], '%s')
        field.write_sub = '%s[%s] = %s' % (prop['name'], '%s', '%s')
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
        self.files = {}
        self.deserialize = []
        self.headers = []
        
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
        self.add_type(from_type)
        self.add_type(to_type)
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
        self.parsing = filename
        self.current_file = os.path.basename(filename).split('.',2)[0]
        self.files[self.current_file] = []
        self.parsed += [filename]
        header = headerParser.CppHeader(filename)
        for cppClass in header.classes:
            self.parse_class(header.classes[cppClass])

    def add_type(self, typeName):
        if typeName not in self.types:
            self.types += [typeName]
                
    def add_block(self, block):
        block.file = self.parsing
        self.blocks[block.id()] = block
        self.files[self.current_file] += [block]
        for typeName in block.types:
            self.add_type(typeName)
        for field in block.fields:
            if field not in self.fields:
                self.fields += [field]
        for field in block.fields.values():
            if field.is_editable():
                if field.type not in self.deserialize:
                    self.deserialize += [field.type]

    def parse_class(self, classInfo):
        annotations = GlueAnnotation.get_annotations(classInfo['doxygen'])
        for annotation in annotations:
            if annotation.name == 'Block':
                self.add_block(GlueBlock.create(annotation, classInfo))

    def render(self, tplname, filename=None, variables={}):
        if filename == None:
            filename = tplname
        template = self.env.get_template(tplname)
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

    def apply_compatibilities(self):
        for block in self.blocks.values():
            for field in block.fields.values():
                if field.type in self.compatibilities:
                    for to_type in self.compatibilities[field.type]:
                        field.add_convertible(to_type)
                        block.add_type(to_type)

    def generate_files(self, output_dir):
        self.apply_compatibilities()
        self.output_dir = output_dir
        self.render('convert.h')
        self.render('deserialize.h')
        self.render('glue.cpp')
        self.render('GlueTypes.h')
        for name, blocks in self.files.items():
            self.render('Block.h', 'Glue'+name+'.h', {'file': name, 'blocks': blocks})
            self.render('Block.cpp', 'Glue'+name+'.cpp', {'file': name, 'blocks': blocks})

