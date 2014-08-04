#ifndef _GLUE_H
#define _GLUE_H

#include <string>
#include <vector>

namespace Glue
{
    class Scene;
    class Node;

    /**
     * Access an input/output of a given node, this will be specialized for
     * each supported type during the generation
     */
    template<typename T>
    T glue_getter(Node *node, int index)
    {}
    template<typename T>
    void glue_setter(Node *node, int index, T value)
    {}

    /**
     * This should be specialized for each compatible types that have special 
     * casting
     */
    template<typename T, typename Q>
    Q glue_convert(T t)
    {
        return (Q)t;
    }

    /**
     * This have to be generated to instanciate the right link depending on which
     * type is used
     */
    void glue_linker(Scene *scene, std::string type, 
            Node *from, int start, Node *to, int end);

    /**
     * Conversion to get the index of an i/o name
     */
    int glue_name_to_index(std::string name);

    /**
     * Deserializes data of a given type
     */
    template<typename T>
    bool glue_deserialize(std::string data, T &var)
    {
        return false;
    }

    /**
     * Creates an instance of a given type with given data for its nodes
     */
    Node *glue_instanciate(std::string type, std::string data);
}

#endif
