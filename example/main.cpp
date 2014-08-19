#include <iostream>
#include <string>
#include <glue/glue.h>
#include <glue/Scene.h>

int main()
{
    Glue::Scene scene;

    try {
        scene.loadFile("scene.json");
    } catch (std::string error) {
        std::cerr << "Error: " << error << std::endl;
    }

    scene.tick();
}
