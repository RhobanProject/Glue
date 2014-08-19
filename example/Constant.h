#ifndef _CONSTANT_H
#define _CONSTANT_H

#include <vector>

namespace Pouet
{
/**
 * Glue:Block(family=core; size=small; description=
 * A simple constant,
 * this will output the value
 * )
 */
class Constant
{
    public:
        /**
         * Glue:Output(default=0.0)
         */
        float value;

        /**
         * Glue:Output()
         */
        int data(int index);
};
}

#endif
