#ifndef _GLUE_H
#define _GLUE_H

#include <string>
#include <vector>

namespace Glue
{
    class Node
    {
        public:
            virtual ~Node() {}
    };
    
    template<typename T>
    T glue_getter(Node *node, std::string name)
    {
    }

    template<typename T>
    void glue_setter(Node *node, std::string name, T value)
    {
    }

    template<typename T, typename Q>
    Q glue_convert(T t)
    {
        return (Q)t;
    }

    class Scene;
    void glue_linker(Scene *scene, std::string type, Node *from, std::string start, Node *to, std::string end);

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

    class LinkBase
    {
        public:
            virtual void tick()=0;
    };

    template<typename T>
        class Link : public LinkBase
    {
        public:
            Link(Node *from_, std::string start_, Node *to_, std::string end_)
                : from(from_), start(start_), to(to_), end(end_)
            {
            }

            void tick()
            {
                T tmp = glue_getter<T>(from, start);
                glue_setter<T>(to, end, tmp);
            }

            Node *from, *to;
            std::string start, end;
    };

    class Scene
    {
        public:
            void add(Node *node)
            {
                nodes.push_back(node);
            }

            void addLink(LinkBase *link)
            {
                links.push_back(link);
            }

            void connect(std::string type, Node *from, std::string start, Node *to, std::string end)
            {
                glue_linker(this, type, from, start, to, end);
            }

            void tick()
            {
                for (auto link : links) {
                    link->tick();
                }
            }

            std::vector<Node*> nodes;
            std::vector<LinkBase*> links;
    };
}

#endif
