#ifndef _TEST_GAINS_H
#define _TEST_GAINS_H

/**
 * @Glue:Block()
 */
class Gains
{
    public:
        /**
         * @Glue:Parameter(default=[1])
         */
        std::vector<float> gains;

        /**
         * @Glue:Input(multiple; dimension=gains)
         */
        std::vector<float> input;

        /**
         * Computes inputs[i]*gains[i]
         *
         * @Glue:Output(name=output; multiple)
         */
        float getOutput(int i);
};

#endif
