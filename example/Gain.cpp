#include <stdio.h>
#include "Gain.h"

float Gain::output()
{
    printf("Gain=%f Value=%f\n", gain, value);
    return gain*value;
}
