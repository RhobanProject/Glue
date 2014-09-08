#ifndef _GLUE_SCENE_H
#define _GLUE_SCENE_H

#include <map>
#include <list>
#include <json/json.h>
#include "glue.h"
#include "Node.h"
#include "Link.h"

namespace Glue
{
    class Scene
    {
        public:
            void add(Node *node);
            void connect(int linkId, int from_id, std::string start, int start_subindex,
                    int to_id, std::string end, int end_subindex);

            void tick();

            void load(std::string data);
            void loadEdge(Json::Value edge);
            void loadBlock(Json::Value block);
            void loadFile(std::string data);
            void loadConnector(Json::Value connector, std::string *index, int *subindex);

        protected:
            std::string data;
            std::map<int, Node*> nodes;
            std::map<int, LinkBase*> links;

            typedef std::pair<LinkBase*, Node*> LinkConnection;
            std::map<int, std::list<LinkConnection>> inLinks;
            std::map<int, std::list<LinkConnection>> outLinks;
            std::list<Tick*> tickList;

            void buildTopologicalSort();
            void visitNode(std::map<Node*,int>& markers, Node* n);
    };
}

#endif
