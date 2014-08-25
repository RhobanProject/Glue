#ifndef _GLUE_CONTROLLER_H
#define _GLUE_CONTROLLER_H

#include <mongoose/WebController.h>

namespace Glue
{
    class Controller : public Mongoose::WebController
    {
        void setup();
    };
}

#endif
