#ifndef _GLUE_SCENE_H
#define _GLUE_SCENE_H

#include <map>
#include "glue.h"
#include "Node.h"
#include "Link.h"

namespace Glue
{
    class Scene
    {
        public:
            void add(Node *node);
            void connect(int linkId, int from_id, std::string start, int to_id, std::string end);
            void tick();

            std::map<int, Node*> nodes;
            std::map<int, LinkBase*> links;
    };
}

#endif
