﻿#include <stdio.h>
#include "block.h"
#include "chain.h"

uint8_t *data[514];
int main()
{
   chain c;
   //
  c.chain_import("test");
  /* for (int i = 0; i <= 514; i++) {
        data[i] = (uint8_t*)malloc(sizeof(uint8_t) * 10);
        for (int j = 0; j < 10; j++) {
            if (j == 9)data[i][j] = '\0';
            else data[i][j] = 97 + (i * j) % 26;
        }

        c.add(data[i], i + 1);
    }
   printf("??\n");*/

  uint8_t data2[10];
  for (int j = 0; j < 10; j++) {
      if (j == 9)data[j] = '\0';
      else data2[j] = (uint8_t)(97 + (121 * j) % 26);
  }

    printf("검증 결과: %d\n",c.verify(122, data2));


    int a;
    scanf_s("\%d\n", &a);
    c.chain_export("test");
   

    scanf_s("\%d\n", &a);
    /*
    uint8_t nisemono[10] = { 'a','b','c','d','e','f','g','h','f','\0' };

    printf("%d~%d번째 노드 검증결과: %d\n", 25,30, .verify(25,30, data2));
    printf("%d~%d번째 노드 검증결과: %d\n", 26, 31, b.verify(26, 31, data2));
    printf("%d~%d번째 노드 검증결과: %d\n", 1, 256, b.verify(1, 256, data));*/
    //printf("%d번째 노드 검증결과: %d\n", 25, b.verify(25, nisemono));
   
  /* for (int i = 0; i <= 256; i++) {
       printf("%d번째 노드 검증결과: %d\n", i+1, b.verify(i+1, data[i]));
   }*/

}