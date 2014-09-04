#ifndef _GLUE_NODE_H
#define _GLUE_NODE_H

#include <string>
#include <json/json.h>
#include "Tick.h"

namespace Glue
{
    class Node : public Tick
    {
        public:
            /**
             * Load serialized data for the block
             */
            virtual void glue_import(Json::Value data)=0;

            /**
             * Tells the type of the nth input/output
             */
            virtual std::string glue_input_type(int index)=0;
            virtual std::string glue_output_type(int index)=0;

            /**
             * Called when a link is created
             */
            virtual void glue_prepare(int index, int subindex)=0;
            
            /**
             * Node identifier
             */
            int glue_id;

            /**
             * Events method
             */
            virtual void glue_tick(float elapsed)=0;
            virtual void glue_start()=0;
            virtual void glue_stop()=0;
            virtual void glue_load()=0;
            virtual void glue_unload()=0;
    };
}

#endif
