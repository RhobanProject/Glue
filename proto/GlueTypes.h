#ifndef _GLUE_TYPES_H
#define _GLUE_TYPES_H

#include <glue/glue.h>

namespace Glue
{
    enum Indexes {
        INDEX_VALUE,
        INDEX_PRINT
    };

    class Node_get_float                                            
    {                                                                   
        public:                                                         
            virtual float glue_get_float(int index)=0;              
    };                                                                  
                                                                        
    class Node_set_float
    {                                                                   
        public:                                                         
            virtual void glue_set_float(int index, float value)=0;
    };                                                                  

    class Node_get_int                                            
    {                                                                   
        public:                                                         
            virtual int glue_get_int(int index)=0;              
    };                                                                  
                                                                        
    class Node_set_int
    {                                                                   
        public:                                                         
            virtual void glue_set_int (int index, int value)=0;
    };                                                                  
}

#endif
