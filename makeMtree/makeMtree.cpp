#include <stdio.h>
#include "hmap.h"
#include "block.h"

uint8_t *data[257];
int main()
{
    block b;
    for (int i = 0; i <= 256; i++) {
        data[i] = (uint8_t*)malloc(sizeof(uint8_t) * 10);
        for (int j = 0; j < 10; j++) {
            if (j == 9)data[i][j] = '\0';
            else data[i][j] = 97 + (i * j) % 26;
        }

        b.add(data[i], i + 1);
    }


    uint8_t *data2[6];
    for (int i = 0; i < 6; i++) {
        data2[i] = (uint8_t *)malloc(sizeof(uint8_t)*10);
        for (int j = 0; j < 10; j++) {
            data2[i][j] = data[i + 24][j];
        }
        
    }
    
    uint8_t nisemono[10] = { 'a','b','c','d','e','f','g','h','f','\0' };

    printf("%d~%d번째 노드 검증결과: %d\n", 25,30, b.verify(25,30, data2));
    printf("%d~%d번째 노드 검증결과: %d\n", 26, 31, b.verify(26, 31, data2));
    printf("%d~%d번째 노드 검증결과: %d\n", 1, 256, b.verify(1, 256, data));
    //printf("%d번째 노드 검증결과: %d\n", 25, b.verify(25, nisemono));
   
  /* for (int i = 0; i <= 256; i++) {
       printf("%d번째 노드 검증결과: %d\n", i+1, b.verify(i+1, data[i]));
   }*/
}