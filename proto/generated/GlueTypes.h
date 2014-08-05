#ifndef _GLUE_TYPES_H
#define _GLUE_TYPES_H

#include "../glue.h"

namespace Glue
{
    enum Indexes {
        INDEX_VALUE,
        INDEX_PRINT
    };

    class Node_get_float                                            
    {                                                                   
        public:                                                         
            virtual float get_float(int index)=0;              
    };                                                                  
                                                                        
    class Node_set_float
    {                                                                   
        public:                                                         
            virtual void set_float (int index, float value)=0;
    };                                                                  

    class Node_get_int                                            
    {                                                                   
        public:                                                         
            virtual int get_int(int index)=0;              
    };                                                                  
                                                                        
    class Node_set_int
    {                                                                   
        public:                                                         
            virtual void set_int (int index, int value)=0;
    };                                                                  
}

#endif
