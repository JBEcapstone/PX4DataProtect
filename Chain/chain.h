#pragma once

#include<stdint.h>
#include"block.h"

#define DEBUG 1
class chain
{
	struct chain_node {
		block b;
		chain_node* prev = NULL;
	};
public:

	chain();
	/*
		체인을 file로부터 import 한다.
		@input: filename
		@output: -
	*/
	void chain_import();

	/*
		체인을 file로 export 한다.
		@input: filename
		@output: -
	*/
	void chain_export(const char filename[]);

	/*
		체인에 데이터를 추가한다.
		@input: 추가할 데이터 data
	*/
	void add(uint8_t data[], uint32_t timestamp);

	/*
		체인 내부의 데이터를 검증한다.
		@input: timestamp: 데이터의 timestamp
				data: 데이터
		@ouput: 1: 검증 성공
				0: 해당 데이터를 찾지 못함
				-1: 검증 실패
	*/
	int verify(uint32_t timestamp, uint8_t data[]);

private:
	chain_node* genesis , *leaf, *cur ;
	FILE* fp;
};

