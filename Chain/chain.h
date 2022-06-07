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
		ü���� file�κ��� import �Ѵ�.
		@input: filename
		@output: -
	*/
	void chain_import();

	/*
		ü���� file�� export �Ѵ�.
		@input: filename
		@output: -
	*/
	void chain_export(const char filename[]);

	/*
		ü�ο� �����͸� �߰��Ѵ�.
		@input: �߰��� ������ data
	*/
	void add(uint8_t data[], uint32_t timestamp);

	/*
		ü�� ������ �����͸� �����Ѵ�.
		@input: timestamp: �������� timestamp
				data: ������
		@ouput: 1: ���� ����
				0: �ش� �����͸� ã�� ����
				-1: ���� ����
	*/
	int verify(uint32_t timestamp, uint8_t data[]);

private:
	chain_node* genesis , *leaf, *cur ;
	FILE* fp;
};

