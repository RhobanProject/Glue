
namespace SomeProject
{
    /**
     * Glue:Block(family=signal; small; description=
     * Some gain
     * )
     */
    class Gain
    {
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
