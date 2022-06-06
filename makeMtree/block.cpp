#include "block.h"

block::block() {

	//Ʈ�� ũ�� ����
	int sum = DATA_NUM;
	treesize = 0;

	//Ʈ�� �ε���
	next_node = DATA_NUM - 1;

	while (sum > 0) {
		treesize += sum;
		sum /= 2;
	}

	if ((tree = (Node*)malloc(sizeof(Node) * treesize)) != NULL) {
		return;
	}

}


int block::add(uint8_t data[], uint32_t timestamp) {

#ifdef DEBUG
	printf("data: %s, node: %d\n", data, next_node);
#endif // DEBUG


	if (next_node == treesize) {

		build_m_tree();
		return 0;
	}

	uint8_t* enc = (uint8_t*)malloc(sizeof(uint8_t) * (SHA256_DIGEST_VALUELEN + 1));
	SHA256_Encrpyt(data, sizeof(data) / sizeof(uint8_t), enc);
	enc[SHA256_DIGEST_VALUELEN] = '\0';
	//printf("plain:%s, hmac: %s\n",data, enc);

	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		tree[next_node].hmac[i] = enc[i];
	}

	tree[next_node].hmac[SHA256_DIGEST_VALUELEN] = '\0';
	tree[next_node].timestamp = timestamp;

#ifdef DEBUG
	printf("node(%d): %s\n=================\n", next_node, tree[next_node].hmac);
#endif // DEBUG


	/*for (int i = 0; i < 32; i++) {
		printf("(%d) %c", i, enc[i]);
	}
	printf("\n");*/
	//free(enc);
	next_node++;
	return 1;

}

// ��Ŭ Ʈ�� ����
void block::build_m_tree() {
	int makenode = treesize - 1 - DATA_NUM, front, rear;
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];

	while (makenode >= 0) {

		front = makenode * 2 + 1;
		rear = makenode * 2 + 2;

#ifdef DEBUG
		printf("\n=============\nmakenode: %d\n", makenode);
#endif // DEBUG


		//printf("front(%d): %s, rear(%d): %s\n", front,tree[front].hmac, rear,tree[rear].hmac);

		for (int i = 0; i < SHA256_DIGEST_VALUELEN * 2; i++) {
			if (i < SHA256_DIGEST_VALUELEN) hash_concat[i] = tree[front].hmac[i];
			else hash_concat[i] = tree[rear].hmac[i - SHA256_DIGEST_VALUELEN];
		}

		hash_concat[SHA256_DIGEST_VALUELEN * 2] = '\0';
		//printf("hash_concat:%s \n", hash_concat);

		SHA256_Encrpyt(hash_concat, 64, digest);
		digest[SHA256_DIGEST_VALUELEN] = '\0';

		for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
			tree[makenode].hmac[i] = digest[i];
		}
		tree[makenode].hmac[SHA256_DIGEST_VALUELEN] = '\0';
		tree[makenode].timestamp = 0;
#ifdef DEBUG
		printf("digest: %s\n", tree[makenode].hmac);

#endif // DEBUG



		makenode -= 1;

		//to test
		//if (makenode != 127) break;
	}

	merkle_root = tree[0].hmac;

}

//��� ã��
//��������� ���� return(0 ~ DATA_NUM - 1)
int block::search(uint32_t timestamp) {
	int front = DATA_NUM - 1;
	int rear = treesize - 1;
	int mid;
	while (front <= rear) {
		mid = (front + rear) / 2;
		if (tree[mid].timestamp < timestamp) {
			front = mid + 1;
		}
		else if (tree[mid].timestamp > timestamp) {
			rear = mid - 1;
		}
		else {

#ifdef DEBUG
			printf("mid: %d\n", mid);
#endif // DEBUG


			return mid - (DATA_NUM - 1);
		}
	}
	return -1;
}


// ������ ����
int block::verify(uint32_t timestamp, uint8_t data[]) {

	int idx = search(timestamp);
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];

	//�켱 ���� ��� �ؽð� Ȯ��
	SHA256_Encrpyt(data, sizeof(data) / sizeof(uint8_t), digest);
	digest[SHA256_DIGEST_VALUELEN] = '\0';

	if (strcmp((char*)digest, (char*)tree[(DATA_NUM - 1) + idx].hmac)) {

#ifdef DEBUG
		printf("digest: %s, tree[%d]: %s\n", digest, idx, tree[(DATA_NUM - 1) + idx].hmac);
#endif // DEBUG

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

		SHA256_Encrpyt(hash_concat, SHA256_DIGEST_VALUELEN * 2, digest);
#ifdef DEBUG
		printf("front(%d): %s, rear(%d): %s\n", front, tree[front].hmac, rear, tree[rear].hmac);
		printf("concat: %s\n", hash_concat);
#endif // DEBUG


		digest[SHA256_DIGEST_VALUELEN] = '\0';

		if (strcmp((char*)digest, (char*)tree[cur].hmac)) {
			printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
			return -1;
		}

		if (!cur) break;
	}

	//root ���� ��
	if (strcmp((char*)digest, (char*)merkle_root)) {
#ifdef DEBUG
		printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
#endif // DEBUG


		return -1;
	}

	return 1;
}

int block::verify(uint32_t time_start, uint32_t time_end, uint8_t* data[]) {
	if ((sess = (Node*)malloc(sizeof(Node) * treesize)) != NULL) {
		return;
	}

	int idx_start = search(time_start);
	int idx_end = search(time_end);
	int idx, front, rear;
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];

	if (idx_start<0 || idx_start>DATA_NUM || idx_end<0 || idx_end>DATA_NUM || idx_start > idx_end) {
		return -1;
	}

	//���� ���� Ȯ��
	for (int i = idx_start; i <= idx_end; i++) {
		idx = idx_start + (DATA_NUM - 1);
		SHA256_Encrpyt(data[idx], sizeof(data[idx]) / sizeof(uint8_t), digest);
		digest[SHA256_DIGEST_VALUELEN] = '\0';

		for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
			sess[idx].hmac[i] = digest[i];
		}
		sess[idx].hmac[SHA256_DIGEST_VALUELEN] = '\0';
		sess[idx].timestamp = 0;
#ifdef DEBUG
		printf("digest: %s\n", sess[idx].hmac);
#endif // DEBUG
		
	}

	front = idx_start + (DATA_NUM - 1);
	rear = idx_end; +(DATA_NUM - 1);

	while (front && rear) {
		if (front % 2) front = (front - 1) / 2;
		else {
			sess[front - 1] = tree[front - 1];
			front = (front - 2) / 2;
		}

		if (rear % 2) {
			sess[rear + 1] = tree[rear + 1];
			rear = (rear - 1) / 2;
		}
		else rear = (rear - 2) / 2;

		for (int cur = front; cur <= rear; cur++) {
			for (int j = 0; j < SHA256_DIGEST_VALUELEN * 2; j++) {
				if (j < SHA256_DIGEST_VALUELEN) hash_concat[j] = sess[cur * 2 + 1].hmac[j];
				else hash_concat[j] = sess[cur * 2 + 2].hmac[j - SHA256_DIGEST_VALUELEN];
			}

			hash_concat[SHA256_DIGEST_VALUELEN * 2] = '\0';

			SHA256_Encrpyt(hash_concat, SHA256_DIGEST_VALUELEN * 2, digest);
#ifdef DEBUG
			printf("front(%d): %s, rear(%d): %s\n", front, sess[front].hmac, rear, sess[rear].hmac);
			printf("concat: %s\n", hash_concat);
#endif // DEBUG


			digest[SHA256_DIGEST_VALUELEN] = '\0';

			for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
				sess[cur].hmac[i] = digest[i];
			}
			sess[cur].hmac[SHA256_DIGEST_VALUELEN] = '\0';
			sess[cur].timestamp = 0;

		}
		
	}


		
		// root ���� ��
		if (strcmp((char*)sess[0].hmac, (char*)merkle_root)) {
#ifdef DEBUG
			printf("merkle root: %s, sess[0]: %s\n", merkle_root, sess[0].hmac);
#endif // DEBUG

			delete[] sess;
			return -1;
		}
	
	delete[] sess;
	return 1;
	

	
}
