#ifndef _TEST_ADD_H
#define _TEST_ADD_H

/**
 * Glue:Block()
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
        void compute();
};

#endif
