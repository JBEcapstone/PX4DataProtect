#include <stdio.h>
#include "hmap.h"
#include "block.h"

int main()
{
    block b;

    uint8_t data[] = {'c','b','c', '\0'};
    uint8_t data2[] = { 'b','b','c','\0'};
    b.add(data,1);
    b.add(data2,2);
    b.add(data, 3);

}