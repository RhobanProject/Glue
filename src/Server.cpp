#include <glue/Server.h>
#include <glue/Controller.h>
#include <mongoose/Server.h>

namespace Glue
{
    void Server::run()
    {
        Controller controller;
        Mongoose::Server server(8080);
        server.registerController(&controller);
        server.start();

        while (1) {
            sleep(10);
        }
    }
}
