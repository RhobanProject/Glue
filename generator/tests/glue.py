#!/usr/bin/python3

import random, unittest, os
from glue import Glue

class TestGlue(unittest.TestCase):
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

    # Helpers

    def get_file(self, name='files'):
        return os.path.join(os.path.dirname(__file__), name)

    def clean_output(self):
        directory = self.get_file('output')
        shutil.rmtree(directory)
        os.makedirs(directory)
        return output

if __name__ == '__main__':
    unittest.main()
