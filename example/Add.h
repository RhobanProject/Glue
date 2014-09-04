#ifndef _ADD_H
#define _ADD_H

#include <vector>

/**
 * Glue:Block(family=Math; description=
 * "Output is the sum of its imputs"
 * )
 */
class Add
{
    public:
        /**
         * Glue:Input(multiple; extensible)
         */
        std::vector<float> terms;

        /**
         * Glue:Output()
         */
        float sum;

        /**
         * Glue:Tick()
         */
        void compute(int elapsed);
};

#endif
