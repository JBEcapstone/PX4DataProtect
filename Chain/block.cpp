#include "block.h"
#define DEBUG  1;

int Strcmp(uint8_t a[], uint8_t b[], int size) {
	for (int i = 0; i < size; i++) {
		if (a[i] > b[i]) return 1;
		else if (a[i] < b[i]) return -1;
	}

	return 0;
}
void block::blockcpy(Node& src, Node& dst) {
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		dst.hmac[i] = src.hmac[i];
	}

	dst.hmac[SHA256_DIGEST_VALUELEN] = '\0';
	dst.timestamp = src.timestamp;
}

void block::setHmac(Node& n, uint8_t mac[]) {
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		n.hmac[i] = mac[i];
	}
	n.hmac[SHA256_DIGEST_VALUELEN] = '\0';
}

block::block() {
	//트리 크기 설정
	int sum = DATA_NUM;
	treesize = 0;

	//트리 인덱스
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

	setHmac(tree[next_node], enc);
	tree[next_node].timestamp = timestamp;

#ifdef DEBUG
	printf("node(%d): %s\n=================\n", next_node, tree[next_node].hmac);
#endif // DEBUG

	next_node++;
	return 1;

}

// 머클 트리 빌드
void block::build_m_tree() {
	int makenode = treesize - 1 - DATA_NUM, front, rear;
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];

	//블록 정보인 block_timestamp 정의
	block_timestamp = tree[treesize - 1].timestamp;
	while (makenode >= 0) {

		front = makenode * 2 + 1;
		rear = makenode * 2 + 2;

#ifdef DEBUG
		printf("\n=============\nmakenode: %d\n", makenode);
#endif // DEBUG

		for (int i = 0; i < SHA256_DIGEST_VALUELEN * 2; i++) {
			if (i < SHA256_DIGEST_VALUELEN) hash_concat[i] = tree[front].hmac[i];
			else hash_concat[i] = tree[rear].hmac[i - SHA256_DIGEST_VALUELEN];
		}

		hash_concat[SHA256_DIGEST_VALUELEN * 2] = '\0';

		SHA256_Encrpyt(hash_concat, 64, digest);
		digest[SHA256_DIGEST_VALUELEN] = '\0';

		setHmac(tree[makenode], digest);
		tree[makenode].timestamp = 0;
#ifdef DEBUG
		printf("digest: %s\n", tree[makenode].hmac);

#endif // DEBUG



		makenode -= 1;

		//to test
		//if (makenode != 127) break;
	}

	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		merkle_root[i] = tree[0].hmac[i];
	}
}

//노드 찾기
//리프노드의 순서 return(0 ~ DATA_NUM - 1)
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


// 위변조 검증
int block::verify(uint32_t timestamp, uint8_t data[]) {

	int idx = search(timestamp);
	if (idx == -1) {
		return 0;
	}
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];

	//우선 리프 노드 해시값 확인
	SHA256_Encrpyt(data, sizeof(data) / sizeof(uint8_t), digest);
	digest[SHA256_DIGEST_VALUELEN] = '\0';

	if (Strcmp(digest, tree[(DATA_NUM - 1) + idx].hmac, SHA256_DIGEST_VALUELEN)) {

#ifdef DEBUG
		printf("digest: %s, tree[%d]: %s\n", digest, idx, tree[(DATA_NUM - 1) + idx].hmac);
#endif // DEBUG

		return -1;
	}

	//트리 거슬러 올라가며 확인
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

		if (Strcmp(digest, tree[cur].hmac, SHA256_DIGEST_VALUELEN)) {
			printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
			return -1;
		}

		if (!cur) break;
	}

	//root 값과 비교
	if (Strcmp(digest, merkle_root, SHA256_DIGEST_VALUELEN)) {
#ifdef DEBUG
		printf("digest: %s, tree[%d]: %s\n", digest, cur, tree[cur].hmac);
#endif // DEBUG


		return -1;
	}

	return 1;
}

int block::verify(uint32_t time_start, uint32_t time_end, uint8_t* data[]) {
	if ((sess = (Node*)malloc(sizeof(Node) * treesize)) != NULL) {
		
	}
	else {
		printf("할당 실패\n");
	}

	int idx_start = search(time_start);
	int idx_end = search(time_end);
	int idx, front, rear;
	uint8_t digest[SHA256_DIGEST_VALUELEN + 1];
	uint8_t hash_concat[SHA256_DIGEST_VALUELEN * 2 + 1];

	if (idx_start<0 || idx_start>DATA_NUM || idx_end<0 || idx_end>DATA_NUM || idx_start > idx_end) {
		return -1;
	}

	//리프 노드들 확인
	for (int i = idx_start; i <= idx_end; i++) {
		idx = i + (DATA_NUM - 1);
		SHA256_Encrpyt(data[i-idx_start], sizeof(data[i-idx_start]) / sizeof(uint8_t), digest);
		digest[SHA256_DIGEST_VALUELEN] = '\0';

		setHmac(sess[idx], digest);
		sess[idx].timestamp = 0;
#ifdef DEBUG
		printf("idx: %d, digest: %s\n", idx,sess[idx].hmac);
#endif // DEBUG
		
	}

	front = idx_start + (DATA_NUM - 1);
	rear = idx_end +(DATA_NUM - 1);
	while (front && rear) {
		if (front % 2) front = (front - 1) / 2;
		else {
			blockcpy(tree[front - 1], sess[front - 1]);
			front = (front - 2) / 2;
		}

		if (rear % 2) {
			blockcpy(tree[rear + 1], sess[rear + 1]);
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
			digest[SHA256_DIGEST_VALUELEN] = '\0';
#ifdef DEBUG
			printf("front(%d): %s, rear(%d): %s\n", front, sess[front].hmac, rear, sess[rear].hmac);
			printf("concat: %s\n", hash_concat);
#endif // DEBUG

			setHmac(sess[cur], digest);
			sess[cur].timestamp = 0;

		}
		
	}
		
	// root 값과 비교
	if (Strcmp(sess[0].hmac, merkle_root, SHA256_DIGEST_VALUELEN)) {
#ifdef DEBUG
		printf("merkle root: %s, sess[0]: %s\n", merkle_root, sess[0].hmac);
#endif // DEBUG
		delete[] sess;
		return -1;
	}

	delete[] sess;
	return 1;

}

void block:: makehash(uint8_t result[]) {
	uint8_t content[SHA256_DIGEST_VALUELEN * 2 + 4];
	content[0] = block_timestamp & (0b11111111);
	content[1] = (block_timestamp & ((0b11111111) << 8)) / (0b11111111);
	content[2] = (block_timestamp & ((0b11111111) << 16)) / (0b11111111);
	content[3] = (block_timestamp & ((0b11111111) << 24)) / (0b11111111);

	for (int i = 4; i < SHA256_DIGEST_VALUELEN + 4; i++) {
		content[i] = previous_hash[i - 4];
	}

	for (int i = SHA256_DIGEST_VALUELEN + 4; i < 2 * SHA256_DIGEST_VALUELEN + 4; i++) {
		content[i] = merkle_root[i - SHA256_DIGEST_VALUELEN - 4];
	}

	SHA256_Encrpyt(content, SHA256_DIGEST_VALUELEN + 4, result);

}

void block::setPrevHash(uint8_t h[]) {
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		previous_hash[i] = h[i];
	}
	previous_hash[SHA256_DIGEST_VALUELEN] = '\0';
}

void block::getPrevHash(uint8_t result[]) {
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		result[i] = previous_hash[i];
	}
}

int block::write_file(FILE *fp) {
	/*
		uint32_t block_timestamp = -1; 
		uint8_t previous_hash[SHA256_DIGEST_VALUELEN];
		uint8_t *merkle_root;
	*/
	fwrite(&block_timestamp, sizeof(uint32_t), 1, fp);
	fwrite(&previous_hash, sizeof(uint8_t) * SHA256_DIGEST_VALUELEN, 1, fp);
	fwrite(&merkle_root, sizeof(uint8_t) * SHA256_DIGEST_VALUELEN, 1, fp);
	for (int i = 0; i < DATA_NUM; i++) {
		fwrite((void *)&tree[i], sizeof(Node), 1, fp);
	}
	return 0;
}

int block::read_file(FILE* fp) {
	uint32_t read_timestamp;
	uint8_t read_prev_hash[SHA256_DIGEST_VALUELEN];
	uint8_t read_merkle_root[SHA256_DIGEST_VALUELEN];
	sess = (Node*)malloc(sizeof(Node) * treesize);

	
	fread(&read_timestamp, sizeof(uint32_t), 1, fp);
	fread(&read_prev_hash, sizeof(uint8_t) * SHA256_DIGEST_VALUELEN, 1, fp);
	fread(&read_merkle_root, sizeof(uint8_t) * SHA256_DIGEST_VALUELEN, 1, fp);
	for (int i = 0; i < DATA_NUM; i++) {
		fread((void*)&sess[i], sizeof(Node), 1, fp);

#ifdef DEBUG
		printf("match in %d: %d\n", i,Strcmp(sess[i].hmac, tree[i].hmac, SHA256_DIGEST_VALUELEN));
#endif
	}

	printf("timestamp: %d, prev hash: %s, merkle root: %s, sess root: %s\n", read_timestamp, read_prev_hash, read_merkle_root, sess[0].hmac);
	
	return 0;
		
} 