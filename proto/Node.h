#ifndef _GLUE_NODE_H
#define _GLUE_NODE_H

namespace Glue
{
    class Node
    {
        public:
            virtual ~Node() {}
            virtual void glue_import(std::string data)=0;
    };
}

#endif
