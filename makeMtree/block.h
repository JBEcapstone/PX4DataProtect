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

		//트리 크기 설정
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

	// 트리에 데이터 추가
	// 만약 timestamp 가 기존 timestamp + timeset 보다 크면 새 노드 생성& 기존 노드 해시
	// 추가 성공시 1, 실패시 0 리턴
	int add(uint8_t data[], uint32_t timestamp) {

	}

	// 머클 트리 빌드
	void build_m_tree() {

	}

	//노드 찾기
	void search(uint32_t timestamp) {

	}

	// 위변조 검증
	void verify(uint32_t timestamp) {

	}

	// 위변조 검증(구간)
	void verify(uint32_t time_start, uint32_t time_end) {

	}



private:
	Node* tree, sess = {0,0};
	int treesize;
	uint8_t block_num = -1;
	uint32_t previous_hash = -1;
};

