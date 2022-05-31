#pragma once
#include <stddef.h>
#include<stdio.h>


const int MAX_SIZE = 100; // �ؽ� ���� ũ�� ����

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
        key ���ڿ��� �ؽ�ȭ�ϰ� �� ���� HMAP �� �߰���
    */
    void add(char* key);


    /*
        key ���ڿ��� �ؽð��� Ȯ���ϰ� �̸� ��������.
    */
    int find(char* key);

    /*
        ������ print �Լ�
        ȣ���ϸ� HMAP ��ҵ��� ����
    */
    void print();

};
