#include <iostream>
#include <string>
#include <glue/Server.h>
#include <glue/Scene.h>

int main()
{
    /*
    try {
        Glue::Scene scene;
        scene.loadFile("scene.json");
        scene.tick();
    } catch (std::string err) {
        std::cout << err << std::endl;
    }
    */

    Glue::Server server;
    server.run();
}
