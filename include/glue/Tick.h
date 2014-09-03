#ifndef _GLUE_TICK_H
#define _GLUE_TICK_H

namespace Glue
{
    class Tick
    {
        public:
            virtual void glue_tick(float elapsed)=0;
    };
}

#endif
