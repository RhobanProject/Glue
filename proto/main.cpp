#include <string>
#include "Glue.h"
#include "Scene.h"
#include "generated/GlueConstant.h"
#include "generated/GluePrinter.h"

int main()
{
    Glue::Scene scene;
    
    // Creates a constant
    Glue::GlueConstant *constant = new Glue::GlueConstant;
    constant->value = 123.6;

    Glue::Node *node1 = constant;
    scene.add(node1);

    // Creates a printer
    Glue::GluePrinter *printer = new Glue::GluePrinter;

    Glue::Node *node2 = printer;
    scene.add(node2);

    // Connects the constant to the printer
    scene.connect("int", node1, "value", node2, "print");

    // Tick
    scene.tick();
}
