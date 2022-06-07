#include "chain.h"

chain::chain() {
	leaf = (chain_node*)malloc(sizeof(chain_node));
	leaf->b = block();
	leaf->prev = NULL;
	genesis = leaf;
}

void chain::chain_import(const char filename[]) {
	fp = fopen(filename, "rb");

	cur = leaf->prev;
	while (cur != NULL) {

		cur->b.read_file(fp);
		cur = cur->prev;
	}

	fclose(fp);
}

void chain::chain_export(const char filename[]) {
	fp = fopen(filename, "wb");

	cur = leaf->prev;
	while (cur != NULL) {

		cur->b.write_file(fp);
		cur = cur->prev;
	}

	fclose(fp);
}

void chain::add(uint8_t data[], uint32_t timestamp) {
	uint8_t blockhash[SHA256_DIGEST_VALUELEN];
	if (!leaf->b.add(data, timestamp)) {
		chain_node* new_chain = (chain_node*)malloc(sizeof(chain_node));
		new_chain->b = block();
		leaf->b.makehash(blockhash);

		new_chain->b.setPrevHash(blockhash);
		new_chain->prev = leaf;

		leaf = new_chain;
		leaf->b.add(data, timestamp);
	}
}

int chain::verify(uint32_t timestamp, uint8_t data[]) {
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
	//leaf block이 아닐 때 
	if (trace != leaf) {

		//다음 블럭에서 해시값 가져오기
		trace->b.getPrevHash(prevHash);

		//현재 블럭 해시하기
		cur->b.makehash(curHash);

#ifdef DEBUG
		printf("previous hash: %s, cur Hash: %s\n", prevHash, curHash);
#endif // DEBUG

		for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
			if (prevHash[i] != curHash[i]) {
				printf("%d\n", strcmp((char*)prevHash, (char*)curHash));
				printf("블럭의 해시값이 다릅니다.\n");
				return -1;
			}
		}

	}

	return cur->b.verify(timestamp, data);
}