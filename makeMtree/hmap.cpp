#include "hmap.h"

ull HMAP::toHash(char* str) {
    int c = 0;
    ull value = 26;

    while ((c = *str++) != '\0') {
        //printf("c: %d\n", c);
        value = ((value << 5) + value) + c;
        //printf("value: %ull\n", value);
    }

    

    return value;
}

int HMAP::toHashedKey(ull value) {
    return value % MAX_SIZE;
}

void HMAP::add(char* key)
{
    ull value = toHash(key);
    int hashedKey = toHashedKey(value);
    Node* ptr;
    Node* newNode = new Node(value);

    ptr = hashTable[hashedKey];
    if (ptr != NULL) ptr->prev = newNode;
    newNode->next = ptr;
    hashTable[hashedKey] = newNode;
}

HMAP::HMAP() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        hashTable[i] = NULL;
    }
}

int HMAP::find(char* key){
    ull value = toHash(key);
    int hashedKey = toHashedKey(value);

    for (Node* p = hashTable[hashedKey]; p != NULL; p = p->next) {
        if (p->value == value) {
            return p->value;
        }
    }

    return -1;
}

void HMAP::print() {
    Node* ptr;
    for (int i = 0; i < MAX_SIZE; i++) {
        ptr = hashTable[i];
        while (ptr != NULL) {
            printf("%ull ", ptr->value);
            ptr = ptr->next;
        }
        if (ptr != hashTable[i]) printf("\n");
    }
    delete ptr;
}