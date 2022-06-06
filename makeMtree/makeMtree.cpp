#include <stdio.h>
#include "hmap.h"
#include "block.h"

uint8_t data[257][10];
int main()
{
    block b;
    for (int i = 0; i <= 256; i++) {
        for (int j = 0; j < 10; j++) {
            if (j == 9)data[i][j] = '\0';
            else data[i][j] = 97 + (i * j) % 26;
        }

        b.add(data[i], i + 1);
    }
    
    uint8_t nisemono[10] = { 'a','b','c','d','e','f','g','h','f','\0' };

    printf("%d번째 노드 검증결과: %d\n", 25, b.verify(25, data[24]));
    printf("%d번째 노드 검증결과: %d\n", 25, b.verify(25, nisemono));
   
  /* for (int i = 0; i <= 256; i++) {
       printf("%d번째 노드 검증결과: %d\n", i+1, b.verify(i+1, data[i]));
   }*/
}