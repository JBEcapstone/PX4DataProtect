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

	chain() {
		leaf = (chain_node *)malloc(sizeof(chain_node));
		leaf->b = block();
		leaf->prev = NULL;
		genesis = leaf;
	}
	/*
		ü���� file�κ��� import �Ѵ�.
		@input: filename
		@output: -
	*/
	void chain_import() {

	}

	/*
		ü���� file�� export �Ѵ�.
		@input: -
		@output: -
	*/
	void chain_export() {

	}

	/*
		ü�ο� �����͸� �߰��Ѵ�.
		@input: �߰��� ������ data
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

#ifdef DEBUG
			printf("timestamp: %d\n", cur->prev->b.getTimeStamp());
#endif		
			
			trace = cur;
			cur = cur->prev;

		}
		//leaf block�� �ƴ� �� 
		if (trace != leaf) {

			//���� ������ �ؽð� ��������
			trace->b.getPrevHash(prevHash);

			//���� �� �ؽ��ϱ�
			cur->b.makehash(curHash);

#ifdef DEBUG
			printf("previous hash: %s, cur Hash: %s\n", prevHash, curHash);
#endif // DEBUG

			for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
				if (prevHash[i] !=  curHash[i]) {
					printf("%d\n", strcmp((char*)prevHash, (char*)curHash));
					printf("���� �ؽð��� �ٸ��ϴ�.\n");
					return -1;
				}
			}
			
		}		

		return cur->b.verify(timestamp, data);
	}

private:
	chain_node* genesis , *leaf, *cur ;
};

