#!/usr/bin/python3

import random, unittest
from glue import GlueAnnotation

class TestAnnotations(unittest.TestCase):
    def test_annotation_simple(self):
        test = '''
        /**
         * Glue:Annotation()
         */
        '''
        annotations = GlueAnnotation.get_annotations(test)
        self.assertEqual(1, len(annotations))
        self.assertEqual('Annotation', annotations[0].name)

    def test_annotation_multiple(self):
        test = '''
        /**
        * Glue:A(x=1; y=2)
        * Glue:B(m=3; n=2)
        */
        '''
        annotations = GlueAnnotation.get_annotations(test)
        self.assertEqual(2, len(annotations))
        a = annotations[0]
        b = annotations[1]
        self.assertEqual('A', a.name)
        self.assertEqual(2, len(a.params))
        self.assertEqual('1', a.params['x'])
        self.assertEqual('2', a.params['y'])
        self.assertEqual('B', b.name)
        self.assertEqual(2, len(b.params))
        self.assertEqual('3', b.params['m'])
        self.assertEqual('2', b.params['n'])


    def test_annotation_escape(self):
        test='''
        /**
         * Glue:Block(family=core; extensible; description=
         * A simple constant,
         * this will output the value a=b!; escaped=
         * `This can contains ;, because there is escape`
         * )
         */
        '''
        annotations = GlueAnnotation.get_annotations(test)
        self.assertEqual(1, len(annotations))
        self.assertEqual('Block', annotations[0].name)

        params = annotations[0].params
        self.assertEqual(4, len(params))
        self.assertEqual('core', params['family'])
        self.assertEqual(True, params['extensible'])
        self.assertEqual('A simple constant, this will output the value a=b!', params['description'])
        self.assertEqual('This can contains ;, because there is escape', params['escaped'])

if __name__ == '__main__':
    unittest.main()
