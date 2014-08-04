#ifndef _GLUE_H
#define _GLUE_H

#include <string>
#include <vector>
#include "Node.h"

namespace Glue
{ 
    template<typename T>
    T glue_getter(Node *node, int index)
    {
    }

    template<typename T>
    void glue_setter(Node *node, int index, T value)
    {
    }

    template<typename T, typename Q>
    Q glue_convert(T t)
    {
        return (Q)t;
    }

    class Scene;
    void glue_linker(Scene *scene, std::string type, Node *from, int start, Node *to, int end);
    int glue_name_to_index(std::string name);

    template<typename T>
    bool glue_deserialize(std::string data, T &var)
    {
        return false;
    }

    template<typename T>
        class Output
        {
            public:
                virtual T get(std::string name)=0;
        };

    template<typename T>
        class Input
        {
            public:
                virtual void set(std::string name, T value)=0;
        };
}

#endif
