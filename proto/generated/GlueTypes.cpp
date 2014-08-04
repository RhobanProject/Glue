#include "../Scene.h"
#include "GlueTypes.h"

namespace Glue
{
    template<>                                                                
        float glue_getter< float >(Node *node, int index)
        {                                                                     
            Node_get_float *n = dynamic_cast<Node_get_float *>(node);   
            return n->get_float(index);
        }                                       

    template<>                                                                
        void glue_setter< float >(Node *node, int index, float value)   
        {                                                                     
            Node_set_float *n = dynamic_cast<Node_set_float *>(node);   
            n->set_float(index, value);
        }                                               
    
    template<>                                                                
        int glue_getter< int >(Node *node, int index)
        {                                                                     
            Node_get_int *n = dynamic_cast<Node_get_int *>(node);   
            return n->get_int(index);
        }                                                                     

    template<>                                                                
        void glue_setter< int >(Node *node, int index, int value)   
        {                                                                     
            Node_set_int *n = dynamic_cast<Node_set_int *>(node);   
            n->set_int (index, value);
        }                                               


    void glue_linker(Scene *scene, std::string type, Node *from, int start, Node *to, int end)
    {
        if (type == "float" ) {                                   
            scene->addLink(new Link<float>(from, start, to, end)); 
        }
        if (type == "int") {
            scene->addLink(new Link<int>(from, start, to, end));
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
}
