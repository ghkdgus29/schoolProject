#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int check[MAX_NODE];

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}

typedef struct node {												// adjlist 를 위한 node 구조체이다.
	int vertex;														// vertex는 꼭지점 성분을 나타내고
	struct node* next;												// next는 linked list의 다음번 노드 주소를 나타낸다.
}node;

node* GL[MAX_NODE];											// NODE구조체의 주소를 저장하는 GL 배열을 선언한다.

void input_adjlist(node* a[], int* V, int* E) {				// adjlist를 구현하는 함수를 나타낸다.
	char vertex[3];										    // AB, AC 와 같은 선분을 저장하는 배열	
	int i, j;
	node* t;												// linked list 를 만들기 위한 t를 선언한다.
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);									// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL의 요소들을 NULL로 초기화시킨다.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);								// 선분정보를 vertex 배열에 저장한다.
		i = name2int(vertex[0]);							// 첫번째 꼭지점을 정수화하여 i에 저장한다.
		t = (node*)malloc(sizeof(node));					// t주소에 node구조체만큼의 공간을 할당한다.
		t->vertex = name2int(vertex[1]);					// t주소의 node 구조체에 두번째 꼭지점값을 정수화하여 저장한다.
		t->next = a[i];										// next에 NULL 값을 저장한다.
		a[i] = t;											// GL의 i번째 요소에 t의 주소를 저장한다.

		i = name2int(vertex[1]);							// AB = BA 이므로 양방향으로 저장하기 위해 1과 0을 뒤집어서 한번 더 해준다.
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
	}
}

void print_adjlist(int a[MAX_NODE], int V) {				// adjlist를 출력하기 위한 함수이다. GL배열을 매개변수로 받는다.
	int i, j;
	node* p;											// linked list를 이용하기 위한 p와 free를 위한 s를 선언한다.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. 와 같은 vertex 성분들을 왼쪽 시작부분에 단어화하여 출력한다.
		p = GL[i];											// p에 GL[i]에 해당하는 주소값을 저장한다. 즉 linked list에 접근하기 위함이다.
		while (p != NULL) {									// 만약 p주소가 NULL 값을 가리키면 while문을 빠져나간다.
			printf("-> %3c ", int2name(p->vertex));			// p주소의 node 구조체의 vertex값을 단어화하여 출력한다.
													// p주소값을 s주소에 할당한다.
			p = p->next;									// p주소값을 다음 노드로 옮긴다.
													// 출력이 끝난 노드는 제거한다.
		}
	}
}


typedef struct dnode {
	int key;
	struct dnode* prev;
	struct dnode* next;
}dnode;

dnode* head, * tail;

void init_queue() {
	head = (dnode*)calloc(1, sizeof(dnode));
	tail = (dnode*)calloc(1, sizeof(dnode));
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

int put(int k) {
	dnode* t;
	if ((t = (dnode*)malloc(sizeof(dnode))) == NULL) {
		printf("Out of memory!\n");
		return -1;
	}

	t->key = k;
	tail->prev->next = t;
	t->prev = tail->prev;
	tail->prev = t;
	t->next = tail;
	return k;
}

int get() {
	dnode* t;
	int k;
	t = head->next;
	if (t == tail) {
		printf("\nQueue underflow\n");
		return -1;
	}
	k = t->key;
	head->next = t->next;
	t->next->prev = head;
	free(t);
	return k;
}

void visit(int i) {
	printf("%3c->", int2name(i));
}

int queue_empty() {
	if (head->next == tail) return 1;
	return 0;
}

void BFS_adjlist(node* a[], int V) {
	int i;
	int cnt = 0;
	node* t;
	init_queue();
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) {
			++cnt;
			put(i);
			check[i] = 1;
			while (!queue_empty()) {
				i = get();
				visit(i);
				for (t = a[i]; t != NULL; t = t->next) {
					if (check[t->vertex] == 0) {
						put(t->vertex);
						check[t->vertex] = 1;
					}
				}
			}
		}
	}
	printf("\n그래프 개수 : %d", cnt);
}

void main() {
	int V, E;

	input_adjlist(GL, &V, &E);
	print_adjlist(GL, V);

	printf("\n\n");

	BFS_adjlist(GL, V);

	node* p, * s;								// free를 통해 메모리 누수를 막아준다.
	int i;
	for (i = 0; i < V; i++) {
		p = GL[i];
		while (p != NULL) {
			s = p;
			p = p->next;
			free(s);
		}
	}

}