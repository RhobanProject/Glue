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
        std::string type = to->glue_output_type(to_index);

        LinkBase *link = glue_link(type, from, from_index, from_subindex, to, to_index, to_subindex);
        link->id = linkId;
        links[linkId] = link;
    }

    void Scene::tick()
    {
        for (auto link : links) {
            link.second->tick();
        }
    }
}
