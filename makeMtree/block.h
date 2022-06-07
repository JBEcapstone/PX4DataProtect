#pragma once
#include <stdint.h>
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include "KISA_SHA256.h"


const int DATA_NUM = 256;
const int TIMESET = 100;

//typedef unsigned char uint8_t;

class block
{
public:
	struct Node {
		uint32_t timestamp;
		uint8_t hmac[SHA256_DIGEST_VALUELEN + 1];
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

	void makehash(uint8_t result[]);
	void setPrevHash(uint8_t h[]);
	uint32_t getTimeStamp() { return block_timestamp; };
	void getPrevHash(uint8_t result[]);



private:
	// tree: tree�� ��尪, sess: �ӽ� Ʈ��
	Node* tree, * sess;

	//treesize: tree ũ��, next_node: tree ������ �������� �߰��� ��� �ε���
	int treesize, next_node;

	/*
		��� ����
		block_timestamp: ����� ������� �ð�. build_m_tree() ���� ���ǵ�
		previous_hash: ���� ���� �ؽð�
		merkle_root: ��Ŭ Ʈ���� ��Ʈ��. build_m_tree() ���� ���ǵ�
	*/
	uint32_t block_timestamp = -1; 
	uint8_t previous_hash[SHA256_DIGEST_VALUELEN];
	uint8_t *merkle_root;

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

	/*
		����� ������ �����Ѵ�.
		@input:		src: ������ ���� 
					dst: ���� Ÿ��
	*/
	void blockcpy(Node& src, Node& dst);

	/*
		����� hmac�� �����Ѵ�.
		@input:		n: hmac�� ������ ���
					mac: hmac
	*/
	void setHmac(Node& n, uint8_t mac[]);
};

