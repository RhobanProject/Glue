#ifndef _ADD_H
#define _ADD_H

#include <vector>

/**
 * Glue:Block(description=
 * Output is the sum of its imputs
 * )
 */
class Add
{
    public:
        /**
         * Glue:Input()
         */
        std::vector<int> terms;

        /**
         * Glue:Output()
         */
        int sum();
};

#endif
