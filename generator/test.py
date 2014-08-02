
from glue import GlueAnnotation

data = '''
/**
 * Glue:Data()
 * Glue:Pouet(a=12; b=[1,2.34,5.44])
 */
'''

print(GlueAnnotation.get_annotations(data))
