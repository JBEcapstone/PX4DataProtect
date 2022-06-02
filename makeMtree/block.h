#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "KISA_SHA256.h"

const int DATA_NUM = 256;
const int TIMESET = 100;

class block
{
public:
	struct Node {
		uint32_t timestamp;
		uint32_t hmac;
	};

	block() {

		//Ʈ�� ũ�� ����
		int sum = DATA_NUM;
		treesize = 0;

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
	// �߰� ������ 1, ���н� 0 ����
	int add(uint8_t data[], uint32_t timestamp) {

	}

	// ��Ŭ Ʈ�� ����
	void build_m_tree() {

	}

	//��� ã��
	void search(uint32_t timestamp) {

	}

	// ������ ����
	void verify(uint32_t timestamp) {

	}

	// ������ ����(����)
	void verify(uint32_t time_start, uint32_t time_end) {

	}



private:
	Node* tree, sess = {0,0};
	int treesize;
	uint8_t block_num = -1;
	uint32_t previous_hash = -1;
};

