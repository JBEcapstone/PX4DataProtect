#include <stdio.h>
#include "hmap.h"

int main()
{
    HMAP h;

    char data[3] = {'a','b','c'};
    h.add(data);

    h.print();

}