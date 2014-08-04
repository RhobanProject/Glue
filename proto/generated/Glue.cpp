#include "../Scene.h"
#include "GlueTypes.h"
#include "GluePrinter.h"
#include "GlueConstant.h"

namespace Glue
{
    template<>                                                                
        float glue_getter<float>(Node *node, int index)
        {                                                                     
            Node_get_float *n = dynamic_cast<Node_get_float *>(node);   
            return n->get_float(index);
        }                                       

    template<>                                                                
        void glue_setter<float>(Node *node, int index, float value)   
        {                                                                     
            Node_set_float *n = dynamic_cast<Node_set_float *>(node);   
            n->set_float(index, value);
        }                                               
    
    template<>                                                                
        int glue_getter<int>(Node *node, int index)
        {                                                                     
            Node_get_int *n = dynamic_cast<Node_get_int *>(node);   
            return n->get_int(index);
        }                                                                     

    template<>                                                                
        void glue_setter<int>(Node *node, int index, int value)   
        {                                                                     
            Node_set_int *n = dynamic_cast<Node_set_int *>(node);   
            n->set_int (index, value);
        }                                               


    LinkBase *glue_link(std::string type, Node *from, int start, Node *to, int end)
    {
        if (type == "float" ) {                                   
            return new Link<float>(from, start, to, end);
        }
        if (type == "int") {
            return new Link<int>(from, start, to, end);
        }
    }
    
    int glue_name_to_index(std::string name)
    {
        if (name == "value") {
            return INDEX_VALUE;
        }
        if (name == "print") {
            return INDEX_PRINT;
        }
    }
    
    Node *glue_instanciate(std::string type, std::string data)
    {
        Node *node = NULL;
        if (type == "Constant") {
            node = new GlueConstant;
        }
        if (type == "Printer") {
            node = new GluePrinter;
        }
        node->glue_import(data);

        return node;
    }
}
