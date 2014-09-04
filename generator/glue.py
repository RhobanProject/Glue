#!/usr/bin/python3

import re, sys, os, codecs, shutil
from jinja2 import Template, FileSystemLoader, Environment
from CppHeaderParser import CppHeaderParser3 as headerParser

def glue_type_escape(typename):
    if typename=='std::string':
        return 'string'
    return typename.replace('*', 'star').replace(' ', '_').replace('::', '__').replace('<','_').replace('>','_')

class GlueException(Exception):
    pass

def glue_error(message):
    raise GlueException('Glue fatal error: %s\n' % message)

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
        state = 0
        name = ''
        value = ''
        escaping=False
        data += ';'
        for c in range(0, len(data)):
            c = data[c]
            if state == 0: # Reading the name
                if c == '=':
                    state = 1
                elif c == ';': # Next field without value
                    if name.strip():
                        params[name.strip()] = True
                        name = ''
                else:
                    name += c
            elif state == 1: # Reading the value
                if c == ';' and not escaping: # Exiting the field
                    if name.strip():
                        params[name.strip()] = value.strip()
                    name = ''
                    value = ''
                    state = 0
                elif c == '`': # Entering/exiting escaping
                    escaping = not escaping
                else:
                    value += c
        
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
        self.itemType = self.type
        self.meta = {}

    def add_meta(self, meta):
        for entry, value in meta.items():
            if entry == 'default':
                self.default = value
            elif entry == 'multiple':
                self.multiple=True
                if self.type.startswith('std::map<'):
                    self.itemType = self.type[9:-1]
                    self.multiple = True
                if self.type.startswith('std::vector<'):
                    self.prepare = '%s.resize(%s+1)' % (self.name, '%s')
                    self.itemType = self.type[12:-1]
                    self.multiple = True
            else:
                if type(value) == bool:
                    value = {True: 'true', False: 'false'}[value]
                self.meta[entry] = value

    def accessor_type(self):
        if self.multiple:
            return self.itemType
        else:
            return self.type

    def attributes(self):
        return ' '.join(self.attrs)

    def add_attr(self, attr):
        if attr not in self.attrs:
            self.attrs += [attr]

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
        if 'family' in meta:
            family = meta['family']
            del meta['family']
        else:
            family = 'core'
        if 'name' in meta:
            name = meta['name']
            del meta['name']
        self.name = name
        self.namespace = namespace
        if namespace:
            self.fullclass = namespace+'::'+self.fullclass
        self.fields = {}
        self.types = []
        self.meta = meta
        self.family = family
        self.file = ''
        self.events = {}

    @classmethod
    def all_events(cls):
        return ['load', 'unload', 'start', 'stop']

    def add_event_method(self, event, method, check=True):
        name = method['name']
        if check and len(method['parameters']):
            glue_error('Event method %s::%s() should not take any parameters'
                    % (self.name, name))
        if event not in self.events:
            self.events[event] = []
        self.events[event] += [name]

    def add_tick_method(self, method):
        name = method['name']
        if len(method['parameters'])>1:
            glue_error('Tick method %s::%s() should take 0 or 1 parameters'
                    % (self.name, name))
        if len(method['parameters']):
            type = method['parameters'][0]['type']
            if type == 'int':
                self.add_event_method('tick_int', method, False)
            elif type == 'float' or type == 'double':
                self.add_event_method('tick_float', method, False)
            else:
                glue_error('Tick method %s::%s() has unsupported parameter type %s'
                        % (self.name, name, type))
        else:
            self.add_event_method('tick', method, False)

    def get_event_methods(self, event):
        return self.events.get(event, [])

    def id(self):
        return '%s.%s' % (self.family, self.name)

    def add_type(self, typeName):
        if typeName not in self.types:
            self.types += [typeName]

    def create_field(self, typeName, name, annotation):
        params = annotation.params

        if 'name' in params:
            name = params['name']
            del params['name']

        if name in self.fields:
            field = self.fields[name]
        else:
            default = None
            field = GlueField(typeName, name)
            self.fields[field.name] = field
            self.add_type(field.accessor_type())

        field.add_meta(params)
       
        return field

    def add_input_method(self, method, annotation):
        typeName = None
        multiple = False
        if len(method['parameters']) == 0 or len(method['parameters'])>2:
            glue_error("Input method %s::%s() should have one or two arguments"
                    % (self.name, method['name']))
        if len(method['parameters'])==1:
            param = method['parameters'][0]
            typeName = param['type']
        if len(method['parameters'])==2:
            multiple = True
            if method['parameters'][0]['type'] != 'int':
                glue_error('Input method %s::%s() is multiple, first argument should be int'
                    % (self.name, method['name']))
            param = method['parameters'][1]
            typeName = 'std::vector<'+param['type']+'>'
        field = self.create_field(param['type'], method['name'], annotation)
        if multiple:
            field.multiple = True
        field.write = '%s(%s)' % (method['name'], '%s')
        field.write_sub = '%s(%s, %s)' % (method['name'], '%s', '%s')
        field.add_attr('input')

    def add_output_method(self, method, annotation):
        typeName = method['rtnType']
        multiple = False
        if typeName == 'void':
            glue_error("Output method %s::%s() should not return void"
                    % (self.name, method['name']))
        if len(method['parameters']) > 1:
            glue_error("Output method %s::%s() should take 0 or 1 argument"
                    % (self.name, method['name']))
        if len(method['parameters'])==1:
            multiple = True
            if method['parameters'][0]['type'] != 'int':
                glue_error("Output method %s::%s() is multiple, its argument should be int"
                        % (self.name, method['name']))
            typeName = 'std::vector<'+typeName+'>'
        field = self.create_field(method['rtnType'], method['name'], annotation)
        if multiple:
            field.multiple = True
        field.read = '%s()' % method['name']
        field.read_sub = '%s(%s)' % (method['name'], '%s')
        field.add_attr('output')
    
    def add_input_prop(self, prop, annotation):
        typeName = prop['type']
        field = self.create_field(typeName, prop['name'], annotation)
        field.read = '%s' % prop['name']
        field.write = '%s = %s' % (prop['name'], '%s')
        field.read_sub = '%s[%s]' % (prop['name'], '%s')
        field.write_sub = '%s[%s] = %s' % (prop['name'], '%s', '%s')
        field.add_attr('input')

    def add_output_prop(self, prop, annotation):
        typeName = prop['type']
        field = self.create_field(typeName, prop['name'], annotation)
        field.read = '%s' % prop['name']
        field.write = '%s = %s' % (prop['name'], '%s')
        field.read_sub = '%s[%s]' % (prop['name'], '%s')
        field.write_sub = '%s[%s] = %s' % (prop['name'], '%s', '%s')
        field.add_attr('output')

    def add_parameter_prop(self, prop, annotation):
        field = self.create_field(prop['type'], prop['name'], annotation)
        field.add_attr('editable')
        field.write = '%s = %s' % (prop['name'], '%s')

    @classmethod
    def create(cls, annotation, data):
        name = data['name']
        block = GlueBlock(annotation.params, name, data['namespace'])
        
        for visible in data['methods']:
            for method in data['methods'][visible]:
                if 'doxygen' in method:
                    annotations = GlueAnnotation.get_annotations(method['doxygen'])
                    for annotation in annotations:
                        if annotation.name == 'Input':
                            block.add_input_method(method, annotation)
                        elif annotation.name == 'Output':
                            block.add_output_method(method, annotation)
                        elif annotation.name == 'Parameter':
                            glue_error('Parameter can not be applied to methods')
                        elif annotation.name == 'Tick':
                            block.add_tick_method(method)
                        elif annotation.name.lower() in GlueBlock.all_events():
                            block.add_event_method(annotation.name.lower(), method)
                        else:
                            glue_error('Unknown annotation: "'+annotation.name+'" on method '+method['name'])
        
        for visible in data['properties']:
            for prop in data['properties'][visible]:
                if 'doxygen' in prop:
                    annotations = GlueAnnotation.get_annotations(prop['doxygen'])
                    for annotation in annotations:
                        if annotation.name == 'Input':
                            block.add_input_prop(prop, annotation)
                        elif annotation.name == 'Output':
                            block.add_output_prop(prop, annotation)
                        elif annotation.name == 'Parameter':
                            block.add_parameter_prop(prop, annotation)
                        else:
                            glue_error('Unknown annotation: "'+annotation.name+'" on property '+prop['name'])
               
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
        self.env.lstrip_blocks = True
        self.env.trim_blocks = True

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
        if 'doxygen' in classInfo:
            annotations = GlueAnnotation.get_annotations(classInfo['doxygen'])
            for annotation in annotations:
                if annotation.name == 'Block':
                    self.add_block(GlueBlock.create(annotation, classInfo))
                else:
                    glue_error('Unknown annotation: "'+annotation.name+'" on class '+classInfo['name'])

    def render(self, tplname, filename=None, variables={}, web=False):
        if filename == None:
            filename = tplname
        template = self.env.get_template(tplname)
        variables['glue'] = self
        data = template.render(**variables)

        # Avoid rendering if the file already exist and has
        # the same contents
        if web:
            output_dir = self.web_dir
        else:
            output_dir = self.output_dir
        filename = os.path.join(output_dir, filename)
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

    def generate_files(self, glue_dir, output_dir, web_dir):
        self.apply_compatibilities()
        self.output_dir = output_dir
        self.web_dir = web_dir
        self.render('convert.h')
        self.render('deserialize.h')
        self.render('glue.cpp')
        self.render('GlueTypes.h')
        for name, blocks in self.files.items():
            self.render('Block.h', 'Glue'+name+'.h', {'file': name, 'blocks': blocks})
            self.render('Block.cpp', 'Glue'+name+'.cpp', {'file': name, 'blocks': blocks})
        shutil.copytree(os.path.join(glue_dir, 'www'), web_dir)
        shutil.copytree(os.path.join(glue_dir, 'blocks.js'), os.path.join(web_dir, 'blocks.js'))
        self.render('glue.js', 'js/glue_data.js', {}, True)

