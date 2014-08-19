#include <stdio.h>
#include "Gain.h"

float Gain::output()
{
    return gain*value;
}
