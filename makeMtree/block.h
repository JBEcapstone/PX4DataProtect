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
		트리에 데이터 추가
		@input:	data, timestamp
		@output:	1 데이터 추가 성공시
					0 블록의 리프 노드가 포화되어 데이터 추가에 실패했을 시
	*/
	int add(uint8_t data[], uint32_t timestamp);

	/*
		위변조 검증
		@input:		timestamp, data
		@output:	1  위변조가 검출되지 않았을 경우
					-1 위변조가 검출되었을 경우
	*/
	int verify(uint32_t timestamp, uint8_t data[]);

	/*
		위변조 검증(구간)
		@input:		시작 timestamp, 끝 timestamp
		@output:	1  위변조가 검출되지 않았을 경우
					-1 위변조가 검출되었을 경우
	*/

	int verify(uint32_t time_start, uint32_t time_end, uint8_t* data[]);



private:
	// tree: tree의 노드값
	Node* tree, * sess;

	//treesize: tree 크기, next_node: tree 구성시 다음으로 추가될 노드 인덱스
	int treesize, next_node;

	/*
		블록 정보
		block_timestamp: 블록이 만들어진 시간
		previous_hash: 이전 블럭의 해시값
		merkle_root: 머클 트리의 루트값
	*/
	uint8_t block_timstamp = -1;
	uint32_t previous_hash = -1;
	uint8_t* merkle_root;

	//해시를 위한 구조체
	SHA256_INFO sha;

	/*
		머클 트리 빌드
		@input: -
		@output: -
	*/
	void build_m_tree();

	/*
		timestamp 를 통해 해당하는 리프 노드 찾기
		@input:		timestamp
		@output:	블록 내 리프 노드의 순서(0~(DATA_NUM-1))
	*/
	int search(uint32_t timestamp);
}

