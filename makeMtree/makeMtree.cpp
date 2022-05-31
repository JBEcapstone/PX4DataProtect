#include <stdio.h>
#include "hmap.h"

int main()
{
    HMAP h;

    char data[4] = {'a','b','c', '\0'};
    char data2[4] = { 'b','b','c','\0'};
    h.add(data);
    h.add(data2);
    h.print();

}