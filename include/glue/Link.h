#ifndef _GLUE_LINK_H
#define _GLUE_LINK_H

#include "glue.h"

namespace Glue
{
    class LinkBase
    {
        public:
            virtual void tick()=0;
            int id;
    };

    template<typename T>
        class Link : public LinkBase
    {
        public:
            Link(Node *from_, int start_, Node *to_, int end_)
                : from(from_), start(start_), to(to_), end(end_)
            {
            }

            void tick()
            {
                T tmp = glue_getter<T>(from, start);
                glue_setter<T>(to, end, tmp);
            }

            Node *from;
            int start;
            Node *to;
            int end;
    };
}

#endif
