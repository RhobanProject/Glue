#ifndef _GLUE_SCENE_H
#define _GLUE_SCENE_H

#include "Glue.h"
#include "Node.h"
#include "Link.h"

namespace Glue
{
    class Scene
    {
        public:
            void add(Node *node);
            void addLink(LinkBase *link);
            void connect(std::string type, Node *from, std::string start, Node *to, std::string end);
            void tick();

            std::vector<Node*> nodes;
            std::vector<LinkBase*> links;
    };
}

#endif
