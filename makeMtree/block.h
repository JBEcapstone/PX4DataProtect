#pragma once
#include <stdint.h>
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include "KISA_SHA256.h"

//#define DEBUG 1
const int DATA_NUM = 256;
const int TIMESET = 100;

class block
{
public:
	struct Node {
		uint32_t timestamp;
		uint8_t hmac[SHA256_DIGEST_VALUELEN + 1];

		Node* operator=(Node* other) {
			for (int i = 0; i < SHA256_DIGEST_BLOCKLEN; i++)hmac[i] = other->hmac[i];
			hmac[SHA256_DIGEST_VALUELEN] = '\0';

			this->timestamp = other->timestamp;
		}
	};

	block();

	/*
		Ʈ���� ������ �߰�
		@input:	data, timestamp
		@output:	1 ������ �߰� ������
					0 ����� ���� ��尡 ��ȭ�Ǿ� ������ �߰��� �������� ��
	*/
	int add(uint8_t data[], uint32_t timestamp);

	/*
		������ ����
		@input:		timestamp, data
		@output:	1  �������� ������� �ʾ��� ���
					-1 �������� ����Ǿ��� ���
	*/
	int verify(uint32_t timestamp, uint8_t data[]);

	/*
		������ ����(����)
		@input:		���� timestamp, �� timestamp
		@output:	1  �������� ������� �ʾ��� ���
					-1 �������� ����Ǿ��� ���
	*/

	int verify(uint32_t time_start, uint32_t time_end, uint8_t* data[]);



private:
	// tree: tree�� ��尪
	Node* tree, * sess;

	//treesize: tree ũ��, next_node: tree ������ �������� �߰��� ��� �ε���
	int treesize, next_node;

	/*
		��� ����
		block_timestamp: ����� ������� �ð�
		previous_hash: ���� ���� �ؽð�
		merkle_root: ��Ŭ Ʈ���� ��Ʈ��
	*/
	uint8_t block_timstamp = -1;
	uint32_t previous_hash = -1;
	uint8_t* merkle_root;

	//�ؽø� ���� ����ü
	SHA256_INFO sha;

	/*
		��Ŭ Ʈ�� ����
		@input: -
		@output: -
	*/
	void build_m_tree();

	/*
		timestamp �� ���� �ش��ϴ� ���� ��� ã��
		@input:		timestamp
		@output:	��� �� ���� ����� ����(0~(DATA_NUM-1))
	*/
	int search(uint32_t timestamp);
}

