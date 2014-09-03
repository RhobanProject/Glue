#ifndef _GLUE_LINK_H
#define _GLUE_LINK_H

#include "glue.h"
#include "Tick.h"

namespace Glue
{
    class LinkBase : public Tick
    {
        public:
            int id;
    };

    template<typename T>
        class Link : public LinkBase
    {
        public:
            Link(Node *from_, int from_index_, int from_subindex_, 
                    Node *to_, int to_index_, int to_subindex_)
                : from(from_), from_index(from_index_), from_subindex(from_subindex_),
                   to(to_), to_index(to_index_), to_subindex(to_subindex_)
            {
            }

            void glue_tick()
            {
                T tmp = glue_getter<T>(from, from_index, from_subindex);
                glue_setter<T>(to, to_index, to_subindex, tmp);
            }

            Node *from;
            int from_index, from_subindex;
            Node *to;
            int to_index, to_subindex;
    };
}

#endif
