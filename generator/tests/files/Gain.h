#ifndef _TEST_GAIN_H
#define _TEST_GAIN_H

namespace SomeProject
{
    /**
     * Glue:Block(family=signal; small; description=
     * Some gain
     * )
     */
    class Gain
    {
        public:
            /**
             * The gain
             *
             * Glue:Parameter(default=1.0)
             * Glue:Input()
             */
            float gain;

            /**
             * The input
             *
             * Glue:Input()
             */
            float input;

            /**
             * Computes the output
             *
             * Glue:Output(name=output)
             */ 
            float getOutput();
    };
}

#endif
