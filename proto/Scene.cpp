#include "Scene.h"

namespace Glue
{
    void Scene::add(Node *node)
    {
        nodes.push_back(node);
    }

    void Scene::connect(std::string type, Node *from, std::string start, Node *to, std::string end)
    {
        links.push_back(glue_linker(this, type, from, glue_name_to_index(start), to, glue_name_to_index(end)));
    }

    void Scene::tick()
    {
        for (auto link : links) {
            link->tick();
        }
    }
}
