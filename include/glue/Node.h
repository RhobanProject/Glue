#ifndef _GLUE_NODE_H
#define _GLUE_NODE_H

#include <string>
#include <json/json.h>

namespace Glue
{
    class Node
    {
        public:
            /**
             * Load serialized data for the block
             */
            virtual void glue_import(Json::Value data)=0;

            /**
             * Tells the type of the nth input/output
             */
            virtual std::string glue_output_type(int index)=0;
            
            /**
             * Node identifier
             */
            int glue_id;
    };
}

#endif
