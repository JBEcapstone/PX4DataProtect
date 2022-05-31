#pragma once
#include <stddef.h>
#include<stdio.h>


const int MAX_SIZE = 100; // 해시 맵의 크기 구함

typedef unsigned long long ull;

struct Node {
    Node* prev;
    Node* next;
    ull value;

    Node(ull value) : prev(NULL), next(NULL), value(value) {};
};

class HMAP {

private:

    Node* hashTable[MAX_SIZE];

    ull toHash(char* str);
    int toHashedKey(ull value);

public:

    HMAP();

    /*
        key 문자열을 해시화하고 그 값을 HMAP 에 추가함
    */
    void add(char* key);


    /*
        key 문자열의 해시값을 확인하고 이를 리턴해줌.
    */
    int find(char* key);

    /*
        디버깅용 print 함수
        호출하면 HMAP 요소들이 나옴
    */
    void print();

};
