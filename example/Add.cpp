#include <stdio.h>
#include "Add.h"

int Add::sum()
{
    int s = 0;
    for (unsigned int i=0; i<terms.size(); i++) {
        s += terms[i];
    }

    return s;
}
