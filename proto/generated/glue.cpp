#include <iostream>
#include <glue/Scene.h>
#include "GlueTypes.h"
#include "GluePrinter.h"
#include "GlueConstant.h"

namespace Glue
{
    template<>                                                                
        float glue_getter<float>(Node *node, int index, int subindex)
        {                                                                     
            Node_get_float *n = dynamic_cast<Node_get_float *>(node);   
            return n->glue_get_float(index, subindex);
        }                                       

    template<>                                                                
        void glue_setter<float>(Node *node, int index, int subindex, float value)   
        {                                                                     
            Node_set_float *n = dynamic_cast<Node_set_float *>(node);
            n->glue_set_float(index, subindex,value);
        }                                               
    
    template<>                                                                
        int glue_getter<int>(Node *node, int index, int subindex)
        {                                                                     
            Node_get_int *n = dynamic_cast<Node_get_int *>(node);   
            return n->glue_get_int(index, subindex);
        }                                                                     

    template<>                                                                
        void glue_setter<int>(Node *node, int index, int subindex, int value)   
        {                                                                     
            Node_set_int *n = dynamic_cast<Node_set_int *>(node);   
            n->glue_set_int(index, subindex, value);
        }                                               


    LinkBase *glue_link(std::string type, Node *from, int from_index, int from_subindex,
            Node *to, int to_index, int to_subindex)
    {
        if (type == "float" ) {                                   
            return new Link<float>(from, from_index, from_subindex, to, to_index, to_subindex);
        }
        if (type == "int") {
            return new Link<int>(from, from_index, from_subindex, to, to_index, to_subindex);
        }

        return NULL;
    }
    
    int glue_name_to_index(std::string name)
    {
        if (name == "value") {
            return INDEX_VALUE;
        }
        if (name == "print") {
            return INDEX_PRINT;
        }

        return -1;
    }
    
    Node *glue_instanciate(std::string type, Json::Value data)
    {
        Node *node = NULL;
        if (type == "Constant") {
            node = new GlueConstant;
        }
        if (type == "Printer") {
            node = new GluePrinter;
        }
        if (node != NULL) {
            node->glue_import(data);
        }

        return node;
    }
}
