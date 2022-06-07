#include<stdint.h>
#include<stdio.h>
#include "KISA_SHA256.h"

void print_hash(uint8_t b[]);
void print_hash(uint8_t b[], int size);
int Strcmp(uint8_t a[], uint8_t b[], int size);
int Strcpy(uint8_t a[], uint8_t b[], int size);