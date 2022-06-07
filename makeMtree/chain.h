#pragma once

#include<stdint.h>
#include"block.h"
class chain
{
	struct chain_node {
		block b;
		chain_node* prev;
	};
public:

	chain() {
		leaf = (chain_node *)malloc(sizeof(chain_node));
		leaf->b = block();
		genesis = leaf;
	}
	/*
		체인을 file로부터 import 한다.
		@input: filename
		@output: -
	*/
	void chain_import() {

	}

	/*
		체인을 file로 export 한다.
		@input: -
		@output: -
	*/
	void chain_export() {

	}

	/*
		체인에 데이터를 추가한다.
		@input: 추가할 데이터 data
	*/
	void add(uint8_t data[], uint32_t timestamp) {
		uint8_t blockhash[SHA256_DIGEST_VALUELEN];
		if (!leaf->b.add(data, timestamp)) {
			chain_node *new_chain = (chain_node *)malloc(sizeof(chain_node));
			new_chain->b = block();
			leaf->b.makehash(blockhash);

			new_chain->b.setPrevHash(blockhash);
			new_chain->prev = leaf;

			leaf = new_chain;
			leaf->b.add(data, timestamp);
		}		
	}

	int verify(uint32_t timestamp, uint8_t data[]) {
		chain_node* trace = NULL;
		uint8_t prevHash[SHA256_DIGEST_VALUELEN], curHash[SHA256_DIGEST_VALUELEN];
		cur = leaf;
		
		while (cur->prev != NULL && cur->prev->b.getTimeStamp() > timestamp) {
			trace = cur;
			cur = cur->prev;
		}
		
		//genesis block이 아닐 때 
		if (cur->prev != NULL) {

			//다음 블럭에서 해시값 가져오기
			trace->b.getPrevHash(prevHash);

			//현재 블럭 해시하기
			cur->b.makehash(curHash);
			if (strcmp((char*)prevHash, (char*)curHash)) {
				printf("블럭의 해시값이 다릅니다.\n");
				return -1;
			}
		}		

		return cur->b.verify(timestamp, data);
	}

private:
	chain_node* genesis , *leaf, *cur ;
};

