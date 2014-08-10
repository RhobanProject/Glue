#include <iostream>
#include <sstream>
#include <fstream>
#include <glue/Scene.h>

namespace Glue
{
    void Scene::add(Node *node)
    {
        nodes[node->glue_id] = node;
    }

    void Scene::connect(int linkId, int from_id, std::string start, int from_subindex,
            int to_id, std::string end, int to_subindex)
    {
        Node *from = nodes[from_id];
        Node *to = nodes[to_id];
        int from_index = glue_name_to_index(start);
        int to_index = glue_name_to_index(end);
        std::string type = to->glue_input_type(to_index);

        if (type == "") {
            throw std::string("Can't create a link because it does not exists as input");
        }

        LinkBase *link = glue_link(type, from, from_index, from_subindex, to, to_index, to_subindex);
        link->id = linkId;
        links[linkId] = link;
    }

    void Scene::load(std::string content)
    {
        Json::Value root;
        Json::Reader reader;

        if (reader.parse(content, root)) {
            if (!root.isMember("blocks") || !root.isMember("edges")
                || !root["blocks"].isArray() || !root["edges"].isArray()) {
                throw std::string("No entry blocks and/or edges in the scene");
            }

            for (unsigned int i=0; i<root["blocks"].size(); i++) {
                loadBlock(root["blocks"][i]);
            }

            for (unsigned int i=0; i<root["edges"].size(); i++) {
                loadEdge(root["edges"][i]);
            }
        } else {
            std::ostringstream oss;
            oss << "Parsing error: " << reader.getFormatedErrorMessages();
            throw oss.str();
        }
    }

    void Scene::loadBlock(Json::Value block)
    {
        if (!block.isMember("id") || !block["id"].isInt()) {
            throw std::string("Block without an id");
        }
        if (!block.isMember("type") || !block["type"].isString()) {
            throw std::string("Block without a type");
        }
        if (!block.isMember("values") || !block["values"].isObject()) {
            throw std::string("Block without values");
        }

        int id = block["id"].asInt();
        std::string type = block["type"].asString();
        Json::Value values = block["values"];

        Node *node = glue_instanciate(type, values);

        if (node == NULL) {
            std::ostringstream oss;
            oss << "Can't create a block with type " << type;
            throw oss.str();
        }

        node->glue_id = id;
        add(node);
    }

    void Scene::loadEdge(Json::Value edge)
    {
        if (!edge.isMember("id") || !edge["id"].isInt()) {
            throw std::string("Edge without an id");
        }
        if (!edge.isMember("block1") || !edge["block1"].isInt()) {
            throw std::string("Edge without a block1");
        }
        if (!edge.isMember("block2") || !edge["block2"].isInt()) {
            throw std::string("Edge without a block2");
        }
        if (!edge.isMember("connector1") || !edge["connector1"].isArray()) {
            throw std::string("Edge without a connector1");
        }
        if (!edge.isMember("connector2") || !edge["connector2"].isArray()) {
            throw std::string("Edge without a connector2");
        }

        int id = edge["id"].asInt();
        int block1 = edge["block1"].asInt();
        std::string index1;
        int subindex1=0;
        int block2 = edge["block2"].asInt();
        std::string index2;
        int subindex2=0;
        loadConnector(edge["connector1"], &index1, &subindex1);
        loadConnector(edge["connector2"], &index2, &subindex2);

        connect(id, block1, index1, subindex1, block2, index2, subindex2);
    }
        
    void Scene::loadConnector(Json::Value connector, std::string *index, int *subindex)
    {
        if (connector.size()>2 || connector.size()==0) {
            throw std::string("Bad connector size");
        }
        if (!connector[0].isString()) {
            throw std::string("Connector first index should be a string");
        }
        if (connector.size()==2 && !connector[1].isInt()) {
            throw std::string("Connector second index should be an int");
        }

        *index = connector[0].asString();

        if (connector.size()==2) {
            *subindex = connector[1].asInt();
        }
    }

    void Scene::loadFile(std::string filename)
    {
        std::ifstream ifs(filename.c_str());
        std::string content((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));

        load(content);
    }

    void Scene::tick()
    {
        for (auto link : links) {
            link.second->tick();
        }
    }
}
