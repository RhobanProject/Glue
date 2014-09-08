#include <iostream>
#include <sstream>
#include <fstream>
#include <glue/Scene.h>

namespace Glue
{
    void Scene::add(Node *node)
    {
        nodes[node->glue_id] = node;
        outLinks[node->glue_id] = {};
        inLinks[node->glue_id] = {};
    }

    void Scene::connect(int linkId, int from_id, std::string start, int from_subindex,
            int to_id, std::string end, int to_subindex)
    {
        Node *from = nodes[from_id];
        Node *to = nodes[to_id];
        int from_index = glue_name_to_index(start);
        int to_index = glue_name_to_index(end);
        std::string from_type = from->glue_output_type(from_index);
        std::string to_type = to->glue_input_type(to_index);

        if (from_type == "" || to_type == "") {
            std::ostringstream oss;
            oss << "Can't connect I/O " << start << " (" << from_type << ") to " << end << " (" << to_type <<")";
            throw oss.str();
        }

        if (!glue_is_convertible(from_type, to_type)) {
            std::ostringstream oss;
            oss << "Type " << from_type << " can't be converted to " << to_type;
            throw oss.str();
        }

        from->glue_prepare(from_index, from_subindex);
        to->glue_prepare(to_index, to_subindex);

        LinkBase *link = glue_link(to_type, from, from_index, from_subindex, to, to_index, to_subindex);
        link->id = linkId;
        links[linkId] = link;
        outLinks[from_id].emplace_back(link, to);
        inLinks[to_id].emplace_back(link, from);
    }

    void Scene::load(std::string content)
    {
        data = content;
        Json::Value root;
        Json::Reader reader;

        if (reader.parse(content, root)) {
            if (!root.isMember("blocks") || !root.isMember("edges")
                || !root["blocks"].isArray() || !root["edges"].isArray()) {
                throw std::string("No entry blocks and/or edges in the scene");
            }

            for (unsigned int i=0; i<root["blocks"].size(); i++) {
                try {
                    loadBlock(root["blocks"][i]);
                } catch (std::string error) {
                    std::cerr << "Error loading scene: " << error << std::endl;
                }
            }

            for (unsigned int i=0; i<root["edges"].size(); i++) {
                try {
                    loadEdge(root["edges"][i]);
                } catch (std::string error) {
                    std::cerr << "Error loading scene: " << error << std::endl;
                }
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
        std::string type = block["type"].asString();

        if (!block.isMember("values") || !block["values"].isObject()) {
            std::ostringstream oss;
            oss << "Block " << type << " without values";
            throw oss.str();
        }

        int id = block["id"].asInt();
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
        if (connector.size()>3 || connector.size()==0) {
            throw std::string("Bad connector size");
        }
        if (!connector[0].isString()) {
            throw std::string("Connector first index should be a string");
        }
        if (connector.size()==3 && !connector[2].isInt()) {
            throw std::string("Connector second index should be an int");
        }

        *index = connector[0].asString();

        if (connector.size()==3) {
            *subindex = connector[2].asInt();
        }
    }

    void Scene::loadFile(std::string filename)
    {
        std::ifstream ifs(filename.c_str());
        if (!ifs.is_open()) {
            throw std::string("Unable to open scene file: " + filename);
        }
        std::string content((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));

        load(content);
    }

    void Scene::tick()
    {
        //Build topological sort if 
        //not already done
        if (tickList.empty()) {
            buildTopologicalSort();
        }

        //Call links and nodes tick handler
        for (auto& t : tickList) {
            t->glue_tick(0);
        }
    }
            
    void Scene::buildTopologicalSort()
    {
        std::list<Node*> setToVisit;
        std::map<int, bool> setBeVisited;

        for (auto& n : nodes) {
            if (inLinks[n.second->glue_id].empty()) {
                setToVisit.push_back(n.second);
                setBeVisited[n.second->glue_id] = true;
            } else {
                setBeVisited[n.second->glue_id] = false;
            }
        }
        if (setToVisit.empty() && !nodes.empty()) {
            throw std::string("No leaf node for topological sort");
        }

        while (!setToVisit.empty()) {
            Node* n = setToVisit.front();
            setToVisit.pop_front();
            for (auto& c : inLinks[n->glue_id]) {
                tickList.push_back(c.first);
            }
            tickList.push_back(n);
            for (auto& c : outLinks[n->glue_id]) {
                if (setBeVisited[c.second->glue_id] == false) {
                    setToVisit.push_back(c.second);
                    setBeVisited[c.second->glue_id] = true;
                }
            }
        }
    }
}
