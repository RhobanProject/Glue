#include <string>
#include "glue.h"
#include "Scene.h"
#include "generated/GlueConstant.h"
#include "generated/GluePrinter.h"

int main()
{
    Glue::Scene scene;
    
    // Creates a constant
    Glue::Node *node1 = Glue::glue_instanciate("Constant", "123.6");
    scene.add(node1);

    // Creates a printer
    Glue::Node *node2 = Glue::glue_instanciate("Printer", "");
    scene.add(node2);

    // Connects the constant to the printer
    scene.connect("int", node1, "value", node2, "print");

    // Tick
    scene.tick();
}
