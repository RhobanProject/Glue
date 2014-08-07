#include <string>
#include <glue/glue.h>
#include <glue/Scene.h>
#include "generated/GlueConstant.h"
#include "generated/GluePrinter.h"

int main()
{
    Glue::Scene scene;
    
    // Creates a constant
    Glue::Node *node1 = Glue::glue_instanciate("Constant", "{\"value\":123.6}");
    node1->glue_id = 1;
    scene.add(node1);

    // Creates a printer
    Glue::Node *node2 = Glue::glue_instanciate("Printer", "");
    node2->glue_id = 2;
    scene.add(node2);

    // Connects the constant to the printer
    scene.connect(1, 1, "value", 2, "print");

    // Tick
    scene.tick();
}
