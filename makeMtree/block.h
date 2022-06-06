#pragma once
#include <stdint.h>
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include "KISA_SHA256.h"

const int DATA_NUM = 256;
const int TIMESET = 100;

class block
{
public:
	struct Node {
		uint32_t timestamp;
		uint8_t hmac[SHA256_DIGEST_VALUELEN+1];

		Node* operator=(Node* other) {
			for(int i = 0 ; i<SHA256_DIGEST_BLOCKLEN;i++)hmac[i] = other->hmac[i];

			this->timestamp = other->timestamp;
		}
	};

	block() {

		//Ʈ�� ũ�� ����
		int sum = DATA_NUM;
		treesize = 0;

		//Ʈ�� �ε���
		next_node = DATA_NUM-1;

		while (sum > 0) {
			treesize += sum;
			sum /= 2;
		}

		if ((tree = (Node*)malloc(sizeof(Node)*treesize)) != NULL) {
			return;
		}
	
	}

	// Ʈ���� ������ �߰�
	// ���� timestamp �� ���� timestamp + timeset ���� ũ�� �� ��� ����& ���� ��� �ؽ�
	// �߰� ������ 1, ����(��� ��带 ���������� tree build)�� 0 ����
	int add(uint8_t data[], uint32_t timestamp) {

		printf("data: %s, node: %d\n", data,next_node);
		if (next_node == treesize) {
			
			build_m_tree();
			return 0;
		}

		uint8_t* enc = (uint8_t*)malloc(sizeof(uint8_t)* (SHA256_DIGEST_VALUELEN + 1));
		SHA256_Encrpyt(data, sizeof(data)/sizeof(uint8_t), enc);
		enc[SHA256_DIGEST_VALUELEN] = '\0';
		//printf("plain:%s, hmac: %s\n",data, enc);

		for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
			tree[next_node].hmac[i] = enc[i];
		}
		
		tree[next_node].hmac[SHA256_DIGEST_VALUELEN] = '\0';
		tree[next_node].timestamp = timestamp;

		printf("node(%d): %s\n=================\n", next_node,tree[next_node].hmac);
		/*for (int i = 0; i < 32; i++) {
			printf("(%d) %c", i, enc[i]);
		}
		printf("\n");*/
		//free(enc);
		next_node++;
		return 1;

	}

	// ��Ŭ Ʈ�� ����
	void build_m_tree() {
		int makenode = treesize - 1 - DATA_NUM, front, rear;
		uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];
		uint8_t digest[SHA256_DIGEST_VALUELEN + 1];

		while (makenode >= 0) {

			front = makenode * 2 + 1;
			rear = makenode * 2 + 2;
			printf("\n=============\nmakenode: %d\n", makenode);
			//printf("front(%d): %s, rear(%d): %s\n", front,tree[front].hmac, rear,tree[rear].hmac);

			for (int i = 0; i < SHA256_DIGEST_VALUELEN * 2; i++) {
				if (i < SHA256_DIGEST_VALUELEN) hash_concat[i] = tree[front].hmac[i];
				else hash_concat[i] = tree[rear].hmac[i - SHA256_DIGEST_VALUELEN];
			}

			hash_concat[SHA256_DIGEST_VALUELEN * 2] = '\0';
			//printf("hash_concat:%s \n", hash_concat);
			
			SHA256_Encrpyt(hash_concat, 64, digest);
			digest[SHA256_DIGEST_VALUELEN] = '\0';
			
			for(int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
				tree[makenode].hmac[i] = digest[i];
			}
			tree[makenode].hmac[SHA256_DIGEST_VALUELEN] = '\0';
			tree[makenode].timestamp = 0;
			printf("digest: %s\n", tree[makenode].hmac);

			makenode -= 1;

			//to test
			//if (makenode != 127) break;
		}

		merkle_root = tree[0].hmac;
		
	}

	//��� ã��
	//��������� ���� return(0 ~ DATA_NUM - 1)
	int search(uint32_t timestamp) {
		int front = DATA_NUM - 1;
		int rear = treesize - 1;
		int mid;
		while (front <= rear) {
			mid = (front + rear) / 2;
			if (tree[mid].timestamp < timestamp) {
				front = mid + 1;
			}
			else if(tree[mid].timestamp > timestamp) {
				rear = mid - 1;
			}
			else {
				printf("mid: %d\n", mid);
				return mid - (DATA_NUM - 1);
			}
		}
		return -1;
	}

	// ������ ����
	int verify(uint32_t timestamp, uint8_t data[]) {

		int idx = search(timestamp);
		uint8_t digest[SHA256_DIGEST_VALUELEN + 1];
		uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];

		//�켱 ���� ��� �ؽð� Ȯ��
		SHA256_Encrpyt(data, sizeof(data)/sizeof(uint8_t), digest);
		digest[SHA256_DIGEST_VALUELEN] = '\0';

		if (strcmp((char*)digest, (char*)tree[(DATA_NUM - 1) + idx].hmac)) {
			printf("digest: %s, tree[%d]: %s\n", digest, idx, tree[(DATA_NUM - 1) + idx].hmac);
			return -1;
		}

		//Ʈ�� �Ž��� �ö󰡸� Ȯ��
		int front, rear, cur = (DATA_NUM - 1) + idx;
		
		while (1) {
			if (cur % 2) {
				front = cur;
				rear = cur + 1;
			}
			else {
				front = cur - 1;
				rear = cur;
			}

			cur = (front - 1) / 2;
			

			for (int i = 0; i < SHA256_DIGEST_VALUELEN * 2; i++) {
				if (i < SHA256_DIGEST_VALUELEN) hash_concat[i] = tree[front].hmac[i];
				else hash_concat[i] = tree[rear].hmac[i - SHA256_DIGEST_VALUELEN];
			}

			hash_concat[SHA256_DIGEST_VALUELEN * 2] = '\0';

			SHA256_Encrpyt(hash_concat, SHA256_DIGEST_VALUELEN*2, digest);
			printf("front(%d): %s, rear(%d): %s\n", front, tree[front].hmac,rear, tree[rear].hmac);
			printf("concat: %s\n", hash_concat);
			digest[SHA256_DIGEST_VALUELEN] = '\0';

			if (strcmp((char*)digest, (char*)tree[cur].hmac)) {
				printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
				return -1;
			}

			if (!cur) break;
		}

		//root ���� ��
		if (strcmp((char*)digest, (char*)merkle_root)) {
			printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
			return -1;
		}
		
		return 1;
	}

	// ������ ����(����)
	void verify(uint32_t time_start, uint32_t time_end) {

	}



private:
	// tree: tree�� ��尪, sess: Ž���� ���� ��� ��ġ
	Node* tree, sess = {0,0};

	//treesize: tree ũ��, next_node: tree ������ �������� �߰��� ��� �ε���
	int treesize, next_node;
	uint8_t block_num = -1;
	uint32_t previous_hash = -1;
	uint8_t *merkle_root;
	SHA256_INFO sha;
};

