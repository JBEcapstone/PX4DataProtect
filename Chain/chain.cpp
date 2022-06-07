#include "chain.h"

chain::chain() {
	if ((leaf = (chain_node*)malloc(sizeof(chain_node))) == NULL) {
		printf("chain_node �Ҵ� ����\n");
		return;
	}
	leaf->b = block();
	leaf->prev = NULL;
	genesis = leaf;
}

void chain::chain_import(const char filename[]) {
	fp = fopen(filename, "rb");
	chain_node* get_node;
	cur = NULL;
	while (!feof(fp)) {
		
		if ((get_node = (chain_node*)malloc(sizeof(chain_node))) == NULL) {
			printf("chain_node �Ҵ翡 �����Ͽ����ϴ�.\n");
			break;
		}
		else {
			get_node->b = block();
		}
		if (get_node->b.read_file(fp) == -1) {
			printf("�ҷ����⿡ �����Ͽ����ϴ�.\n");
			
			break;
		}
		else {
			get_node->prev = NULL;
		}
		
		if (cur != NULL) {
			cur->prev = get_node;
			printf("prev timestamp: %d\n", get_node->b.getTimeStamp());
			
		}
		else {
			leaf->prev = get_node;
			
		}
		cur = get_node;
		genesis = cur;
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
	trace = leaf;
	cur = leaf->prev;

	while (1) {
		if ((cur->prev != NULL && (cur->prev->b.getTimeStamp() < timestamp)) || cur->prev == NULL) break;
#ifdef DEBUG
		printf("timestamp: %d\n", cur->prev->b.getTimeStamp());
#endif		

		trace = cur;
		cur = cur->prev;
		//leaf block�� �ƴ� �� 
		if (cur == leaf) {

		}
		else {
			//���� ������ �ؽð� ��������
			trace->b.getPrevHash(prevHash);

			//���� �� �ؽ��ϱ�
			cur->b.makehash(curHash);

#ifdef DEBUG
			printf("previous hash:");
			print_hash(prevHash);
			printf("current Hash: ");
			print_hash(curHash);
#endif // DEBUG

			for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
				if (prevHash[i] != curHash[i]) {
					printf("%d\n", strcmp((char*)prevHash, (char*)curHash));
					printf("���� �ؽð��� �ٸ��ϴ�.\n");
					return -1;
				}
				
			}
			printf("���� �ؽð��� ��ġ�մϴ�.\n");
		}
	}
	
	return cur->b.verify(timestamp, data);
}