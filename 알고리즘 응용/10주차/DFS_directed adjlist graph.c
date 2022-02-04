#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;


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

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed adjlist를 구현하는 함수를 나타낸다.
	char vertex[3];										    // AB, AC 와 같은 선분을 저장하는 배열	
	int i, j;
	node* t;												// linked list 를 만들기 위한 t를 선언한다.
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);									// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL의 요소들을 NULL로 초기화시킨다.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		fscanf(fp, "%s", vertex);								// 선분정보를 vertex 배열에 저장한다.
		i = name2int(vertex[0]);							// 첫번째 꼭지점을 정수화하여 i에 저장한다.
		t = (node*)malloc(sizeof(node));					// t주소에 node구조체만큼의 공간을 할당한다.
		t->vertex = name2int(vertex[1]);					// t주소의 node 구조체에 두번째 꼭지점값을 정수화하여 저장한다.
		t->next = a[i];										// next에 NULL 값을 저장한다.
		a[i] = t;											// GL의 i번째 요소에 t의 주소를 저장한다.

	}
}

void print_directed_adjlist(node* a[], int V) {				// adjlist를 출력하기 위한 함수이다. GL배열을 매개변수로 받는다.
	int i, j;
	node* p, * s;											// linked list를 이용하기 위한 p와 free를 위한 s를 선언한다.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. 와 같은 vertex 성분들을 왼쪽 시작부분에 단어화하여 출력한다.
		p = a[i];											// p에 GL[i]에 해당하는 주소값을 저장한다. 즉 linked list에 접근하기 위함이다.
		for (s = p; s != NULL; s = s->next)
			printf("-> %3c ", int2name(s->vertex));
	}
}

#define MAX 1000
int stack[MAX];
int top;
int check[MAX];

int push(int t) {
	if (top >= MAX - 1) {
		printf(" stack overflow! \n");
		return -1;
	}
	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf(" stack underflow! \n");
		return -1;
	}
	return stack[top--];
}

int stack_empty() {
	if (top < 0) return 1;
	return 0;
}													// nr - DFS 를 사용하기 위한 스택 구성

void init_stack() {
	top = -1;
}


void DFS_directed_adjlist(node* a[], int V) {		// reachability 를 조사하는 함수, DFS 변형
	int i, j;
	node* t;
	init_stack();
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++)
			check[j] = 0;			// 모든 노드의 방문 초기화

		push(i);			// i노드 push
		check[i] = 1;		// i노드 방문체크
		printf("\n%c : ", int2name(i));		// i노드의 reachability 표시
		while (!stack_empty()) {		// stack이 빌때까지 반복
			j = pop();						// j노드는 stack에서 pop한 노드		
			printf("%3c->", int2name(j));	// j노드는 도달가능한 노드임을 출력
			for (t = a[j]; t != NULL; t = t->next) {		// j노드에 직접연결된 노드들 조사
				if (check[t->vertex] == 0) {		// 만약 방문하지 않은 노드라면
					push(t->vertex);				// 해당노드 push
					check[t->vertex] = 1;			// 해당노드 방문체크
				}
			}
		}
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");
	int V, E;

	input_directed_adjlist(GL, &V, &E);		// 그래프 읽어들이기

	printf("\n\nOriginal graph\n");			// 그래프 출력
	print_directed_adjlist(GL, V);
	printf("\n\n");

	DFS_directed_adjlist(GL, V);			// reachability 찾기

	fclose(fp);
}