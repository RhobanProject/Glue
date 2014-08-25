#ifndef _GAIN_H
#define _GAIN_H

/**
 * Glue:Block(family=Math; description="Output is gain*input")
 */
class Gain
{
    public:
        /**
         * Glue:Input()
         */
         float value;

        /**
         * Glue:Parameter(default=1.0)
         */
         float gain;

        /**
         * Glue:Output()
         */
         float output(); 
};

#endif
