#ifndef _GLUE_H
#define _GLUE_H

#include <string>
#include <vector>
#include <json/json.h>

namespace Glue
{
    class Scene;
    class LinkBase;
    class Node;

    /**
     * Access an input/output of a given node, this will be specialized for
     * each supported type during the generation
     */
    template<typename T>
    T glue_getter(Node *node, int index, int subindex)
    {}
    template<typename T>
    void glue_setter(Node *node, int index, int subindex, T value)
    {}

    /**
     * This have to be generated to instanciate the right link depending on which
     * type is used
     */
    LinkBase *glue_link(std::string type, Node *from, int from_index, int form_subindex, 
            Node *to, int to_index, int to_subindex);

    /**
     * Conversion to get the index of an i/o name
     */
    int glue_name_to_index(std::string name);

    /**
     * Creates an instance of a given type with given data for its nodes
     */
    Node *glue_instanciate(std::string type, Json::Value data);
    
    /**
     * Tell if type from can be convert to type to
     * In this case, glue_convert_[from]_to_[to]() should also exist
     */
    bool glue_is_convertible(std::string from, std::string to);
}

#endif
