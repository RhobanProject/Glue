#ifndef _CONSTANT_H
#define _CONSTANT_H

#include <string>
#include <vector>

namespace Pouet
{
/**
 * Glue:Block(family=Math; size="small"; description=
 * "A simple constant,
 * this will output the value"
 * )
 */
class Constant
{
    public:
        /**
         * Glue:Input()
         * Glue:Output()
         * Glue:Parameter(default=12)
         */
        float value;

        /**
         * Glue:Parameter(default=[1,2,3])
         * Glue:Input(multiple)
         */
        std::vector<int> test;
};
}

#endif
