#include <stdio.h>
#include "Add.h"

void Add::compute(int e)
{
    sum = 0;
    for (unsigned int i=0; i<terms.size(); i++) {
        sum += terms[i];
    }
}
