#include "tools.h"
void print_hash(uint8_t b[]) {
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		printf("%x", b[i]);
	}
	printf("\n");
}
void print_hash(uint8_t b[],int size) {
	for (int i = 0; i < size; i++) {
		printf("%x", b[i]);
	}
	printf("\n");
}
int Strcmp(uint8_t a[], uint8_t b[], int size) {
	for (int i = 0; i < size; i++) {
		if (a[i] > b[i]) return 1;
		else if (a[i] < b[i]) return -1;
	}

	return 0;
}

int Strcpy(uint8_t a[], uint8_t b[], int size) {
	for (int i = 0; i < size; i++) {
		a[i] = b[i];
	}

	return 0;
}