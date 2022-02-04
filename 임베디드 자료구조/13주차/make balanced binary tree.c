#include <stdio.h>
#include <stdlib.h>

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

typedef struct node {
	int key;
	struct node* left;
	struct node* right;
}node;

node* head;

void init_tree() {
	head = (node*)calloc(1, sizeof(node));
}

node* bti_search(int key, node* base, int* num) {			// 원하는 key값을 가진 노드를 탐색하는 함수,num은 데이터길이임
	node* s;
	s = base->left;		// 시작 노드는 base의 왼쪽에 위치
	printf("\nbase->left");			// 위치 출력을 위함
	while (key != s->key && s != NULL) {
		if (key < s->key) {
			s = s->left;	// 만약 key값이 현재 노드보다 작으면 왼쪽으로 트리탐색
			printf("->left");			// 위치 출력을 위함
		}
		else {
			s = s->right;					// 만약 key값이 현재 노드보다 크면 오른쪽으로 트리탐색
			printf("->right");			// 위치 출력을 위함
		}
	}
	if (s == NULL) return NULL;		// 만약 찾는 key값의 노드가 없으면 NULL 반환
	else return s;						// 찾으면 key값의 노드주소 반환
}

node* bti_insert(int key, node* base, int* num) {
	node* p, * s;
	p = base;			// parent node			
	s = base->left;					// son node, 트리의 뿌리는 base의 left에서 시작한다
	while (s != NULL) {					// NULL에 도착할 때 까지 반복, while문을 통해 입력받은 key값이 들어갈 s노드의 부모 p주소를 찾는다.
		p = s;								// 현재 s노드를 p노드에 저장		
		if (key < s->key) s = s->left;		// 집어넣는 key값이 현재노드의 key값보다 작으면 s노드주소를 왼쪽아래로 이동
		else s = s->right;					// 집어넣는 key값이 현재노드의 key값보다 크면 s노드주소를 오른쪽아래로 이동
	}
	s = (node*)malloc(sizeof(node));		// s노드 메모리 공간을 생성 (입력한 key값이 들어갈 s노드)
	s->key = key;							// s노드 안에 입력받은 key값을 저장
	s->left = NULL;
	s->right = NULL;						// 집어넣은 s노드의 양쪽 끝단을 NULL로 그라운드시킴
	if (key < p->key || p == base) p->left = s;		// 만약 parent node 가 base거나 key값이 p 노드의 key값보다 작으면 왼쪽 아래로 트리형성
	else p->right = s;								// 만약 parent node 가 base거나 key값이 p 노드의 key값보다 크면 오른쪽 아래로 트리형성
	(*num)++;			// 데이터 길이 증가 (트리 길이)
	return s;			// 해당 노드 주소 반환 (key값이 들어있는)
}

node* bti_delete(int key, node* base, int* num) {	// delete 함수의 condition을 줄인 향상된 버전의 delete 함수
	node* parent, * son, * del, * nexth;
	parent = base;
	del = base->left;						// base의 왼쪽에 트리의 뿌리가 있다. parent - del 노드는 부모 - 자식 관계이다
	while (key != del->key && del != NULL) {			// 입력받은 key값과 같은 값을 찾거나 NULL값 까지 다다를때까지 반복
		parent = del;					// 현재 노드를 parent노드에 저장
		if (del->key > key) del = del->left;		// 현재 노드의 key값이 입력받은 key값보다 크면 왼쪽 아래로 이동
		else del = del->right;						// 현재 노드의 key값이 입력받은 key값보다 작으면 오른쪽 아래로 이동
	}		// 지우려는 노드의 parent, del 주소를 찾는 반복문

	if (del == NULL) return NULL;		// 지우려는 노드를 못찾으면 NULL 반환

	//---------여기까진 bti_delete1함수와 동일  (del node를 찾는 동작)

	if (del->right == NULL) son = del->left;	// del node의 오른쪽 자식이 없으면 왼쪽 자식노드가 son노드가 된다
	else if (del->right->left == NULL) {		// del node의 오른쪽 자식노드는 있지만 , 오른쪽 자식노드의 왼쪽자식노드는 없다면 
		son = del->right;					// 오른쪽 자식노드가 son node가 된다.
		son->left = del->left;				// 오른쪽 자식노드(son node) 를 del node와 교체한다.
	}
	else {							// del node가 root일 경우
		nexth = del->right;
		while (nexth->left->left != NULL) nexth = nexth->left;	// 오른쪽편에서 , 가장 왼쪽에 있는 노드의 부모 노드를 nexth로 설정
		son = nexth->left;			// 오른쪽편에서, 가장 왼쪽에 있는 노드를 son으로 설정
		nexth->left = son->right;	// nexth와 son노드의 오른쪽 성분들을 연결
		son->left = del->left;
		son->right = del->right;		// root노드를 son 노드로 대체 (연결을 바꿈)
	}

	//----------아래 부분역시 bti_delete1함수와 동일 (parent -son 노드 연결)

	if (key < parent->key || parent == base)		// case 1,2,3이 공통 수행해야 하는 작업, parent와 son 노드를 연결
		parent->left = son;					// parent값이 son보다 크면 son은 왼쪽아래 위치
	else
		parent->right = son;				// parent값이 son보다 작으면 son은 오른쪽아래 위치


	free(del);		// del 노드 지우기
	(*num)--;		// 데이터 길이 줄이기

	return parent;		// 지운 노드의 부모노드 반환
}

#define MAX 100
node* queue[MAX];				// level order traverse 를 위한 큐로 노드의 주소 저장
int front, rear;				//큐를 위한 인덱스

void init_queue() {					// 큐 초기화
	front = rear = 0;
}

int is_queue_empty() {				// 큐가 비었는지 검사, 비었으면 1반환
	return (front == rear);
}

node* put(node* t) {				// 큐에 트리주소를 넣기위한 put 함수
	queue[rear] = t;
	rear = ++rear % MAX;
	return t;
}

node* get() {						// 큐에서 트리주소를 꺼내기 위한 get함수
	node* t;
	t = queue[front];
	front = ++front % MAX;
	return t;
}

void visit(node* t) {								// 방문한 트리의 값을 출력하는 함수
	printf("%2c", int2name(t->key));
}

void levelorder_traverse(node* t) {					// level order traverse 를 위한 함수
	put(t);

	while (!is_queue_empty()) {
		t = get();
		visit(t);

		if (t->left != NULL) put(t->left);
		if (t->right != NULL) put(t->right);
	}
}

void bti_sort(node* p, int* a, int* index) {			// 주어진 binary tree를 배열로 뽑아내는 함수 (Inorder traverse와 비슷)
	if (p != NULL) {		// 종료조건: tree노드가 NULL에 다다르면 종료
		bti_sort(p->left, a, index);			// 왼쪽노드 방문 (작은 노드)
		a[(*index)++] = p->key;					// 해당 노드 방문하여 배열에 tree data를 입력 
		bti_sort(p->right, a, index);			// 오른쪽노드 방문 (큰 노드)
	}
}

node* balance(int N, int* a, int* index) {				// 뽑아낸 배열을 통해 균현잡힌 binary tree 형성
	int nl, nr;									
	node* p;
	if (N > 0) {		//  종료조건: 데이터 길이가 0이되면 종료
		nl = (N - 1) / 2;						// nl은 배열 중간값(root)를 제외한 왼쪽 배열 길이
		nr = N - nl - 1;						// nr은 배열 중간값(root)를 제외한 오른쪽 배열 길이
		p = (node*)malloc(sizeof(node));
		p->left = balance(nl, a, index);		// 왼쪽노드 형성 (작은 노드)
		p->key = a[(*index)++];					// 현재 방문 노드에 배열값을 집어넣는다.
		p->right = balance(nr, a, index);		// 오른쪽 노드 형성 (큰 노드)

		return p;
	}
	else return NULL;
}

int main(void) {
	init_tree();
	init_queue();

	int i, len = 0, index = 0;
	int sort[MAX];
	node* base = calloc(1, sizeof(node));
	

	bti_insert(name2int('F'), head, &len);
	bti_insert(name2int('B'), head, &len);
	bti_insert(name2int('O'), head, &len);
	bti_insert(name2int('A'), head, &len);
	bti_insert(name2int('D'), head, &len);
	bti_insert(name2int('L'), head, &len);
	bti_insert(name2int('C'), head, &len);
	bti_insert(name2int('G'), head, &len);
	bti_insert(name2int('M'), head, &len);
	bti_insert(name2int('H'), head, &len);
	bti_insert(name2int('N'), head, &len);
	bti_insert(name2int('K'), head, &len);			// 강의 자료에 주어진 트리 그리기

	printf("\nOrigianl Tree\n");
	levelorder_traverse(head->left);				// levelorder traverse 로 트리 표현

	printf("\n\nL path is ");
	bti_search(name2int('L'), head, &len);			// L 노드가 어디있는지 search

	bti_sort(head->left, sort, &index);				// sort함수를 통해 binary tree 값을 sort배열에 추출한다.

	index = 0;			// 인덱스 초기화

	base->left = balance(len, sort, &index);				// base에 balanced binary tree의 root노드 주소를 반환


	printf("\n\nBalanced Tree\n");
	levelorder_traverse(base->left);						// balance tree 출력

	printf("\n\nL path is ");
	bti_search(name2int('L'), base, &len);			// L 노드가 어디있는지 search
	printf("\n");

}