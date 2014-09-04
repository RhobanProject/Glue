#!/usr/bin/python3

import random, unittest, os, shutil
from subprocess import Popen, PIPE
from glue import Glue

class TestGlue(unittest.TestCase):
    """
    Parses a simple file containing a block and tests that its input, output and
    parameter is recognized
    """
    def test_glue_basic(self):
        glue = Glue()
        glue.parse(self.get_file('files/Gain.h'))

        self.assertEqual(1, len(glue.blocks))
        block = list(glue.blocks.values())[0]
        self.assertEqual('Gain', block.name)
        self.assertEqual('SomeProject', block.namespace)
        self.assertEqual('signal', block.family)
        self.assertTrue('description' in block.meta)
        self.assertEqual('Some gain', block.meta['description'])
        self.assertTrue('small' in block.meta)
        self.assertEqual(True, block.meta['small'])
        self.assertEqual(3, len(block.fields))
        self.assertTrue('gain' in block.fields)
        self.assertTrue('input' in block.fields)
        self.assertTrue('output' in block.fields)

        gain = block.fields['gain']
        self.assertEqual(['editable', 'input'], gain.attrs)
        self.assertEqual('1.0', gain.default)
        self.assertEqual('float', gain.type)
        self.assertFalse(gain.multiple)

        input = block.fields['input']
        self.assertEqual(['input'], input.attrs)
        self.assertFalse(input.multiple)
        self.assertEqual('float', input.type)

        output = block.fields['output']
        self.assertEqual(['output'], output.attrs)
        self.assertFalse(output.multiple)
        self.assertEqual('getOutput()', output.get())
        self.assertEqual('float', output.type)

    """
    Parses a simple file and tests that inputs, outputs and parameters are OK,
    it contains some multiple I/Os
    """
    def test_glue_multiple(self):
        glue = Glue()
        glue.parse(self.get_file('files/Gains.h'))
        
        self.assertEqual(1, len(glue.blocks))
        block = list(glue.blocks.values())[0]
        self.assertEqual('Gains', block.name)
        
        self.assertEqual(3, len(block.fields))
        self.assertTrue('gains' in block.fields)
        self.assertTrue('input' in block.fields)
        self.assertTrue('output' in block.fields)

        gains = block.fields['gains']
        self.assertFalse(gains.multiple)
        self.assertEqual('std::vector<float>', gains.type)
        self.assertEqual('[1]', gains.default)
        self.assertEqual(['editable'], gains.attrs)

        input = block.fields['input']
        self.assertTrue(input.multiple)
        self.assertTrue('dimension' in input.meta)
        self.assertEqual('gains', input.meta['dimension'])
        self.assertEqual(['input'], input.attrs)
        self.assertEqual('std::vector<float>', input.type)
        self.assertEqual('float', input.accessor_type())

        output = block.fields['output']
        self.assertTrue(output.multiple)
        self.assertEqual('float', output.type)
        self.assertEqual('float', output.accessor_type())

    """
    Testing that parsing goes right, event with classes without comments
    """
    def test_glue_lifecycle(self):
        glue = Glue()
        glue.parse(self.get_file('files/Dummy.h'))
        self.assertEqual(1, len(glue.blocks))

    """
    Testing that files are indeed generated
    It also checks that C++ files have a correct syntax
    """
    def test_glue_generation_basic(self):
        glue = Glue()
        glue.parse(self.get_file('files/Gain.h'))
        glue.parse(self.get_file('files/Gains.h'))
        self.generate(glue)

        self.assertTrue(os.path.isdir(self.get_output('glue')))
        self.check_cpp_file('glue/glue.cpp')
        self.check_cpp_file('glue/convert.h')
        self.check_cpp_file('glue/deserialize.h')
        self.check_cpp_file('glue/GlueTypes.h')

        self.check_cpp_file('glue/GlueGain.cpp')
        self.check_cpp_file('glue/GlueGain.h')
        self.check_cpp_file('glue/GlueGains.cpp')
        self.check_cpp_file('glue/GlueGains.h')

        self.assertTrue(os.path.isdir(self.get_output('web')))
        self.assertTrue(os.path.isdir(self.get_output('web/blocks.js')))
        self.assertTrue(os.path.isdir(self.get_output('web/js')))
        self.assertTrue(os.path.isfile(self.get_output('web/js/glue_data.js')))
        self.assertTrue(os.path.isfile(self.get_output('web/index.html')))

    # Helpers

    def get_file(self, name='files'):
        return os.path.join(os.path.dirname(__file__), name)
    
    def get_output(self, filename):
        return os.path.join(self.get_file('output'), filename)

    def get_glue_dir(self):
        return os.path.join(os.path.dirname(__file__), '..', '..')

    def generate(self, glue):
        directory = self.get_file('output')
        if os.path.isdir(directory):
            shutil.rmtree(directory)
        glue_directory = self.get_glue_dir()
        output_directory = os.path.join(directory, 'glue')
        web_directory = os.path.join(directory, 'web')
        os.makedirs(output_directory)
        glue.generate_files(glue_directory, output_directory, web_directory)

    def check_cpp_file(self, filename):
        filename = self.get_output(filename)
        if os.path.isfile(filename):
            glue_dir = self.get_glue_dir()
            glue_include = os.path.join(glue_dir, 'include')
            json_include = os.path.join(glue_dir, 'json/include')
            cmd = ['g++', '-pedantic', '-I', glue_include, '-I', json_include, '-fsyntax-only', filename]
            process = Popen(cmd, stdout=PIPE, stderr=PIPE)
            rtn = process.wait()
            output = process.stdout.read()
            output += process.stderr.read()
            process.stdout.close()
            process.stderr.close()
            self.assertEqual(0, rtn, 'Parsing error: '+output.decode('utf-8'))
        else:
            self.assertTrue(False, 'File '+filename+' not generated')

if __name__ == '__main__':
    unittest.main()
