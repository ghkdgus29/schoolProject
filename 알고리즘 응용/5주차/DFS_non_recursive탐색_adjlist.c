#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

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

int check[MAX_NODE];

#define MAX 100
int stack[MAX];
int top;

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
}

void nrDFS_adjlist(node* a[], int V) {						// non recursive 한 DFS 탐색을 위한 함수이다.
	int i;													// 인덱싱을 위한 i 변수 선언
	node* t;												// list에 접근하기 위한 t 포인터 변수 선언
	top = -1;												// init_stack();
	for (i = 0; i < V; i++) check[i] = 0;					// 모든 노드들에 대해 check값을 0으로 초기화
	for (i = 0; i < V; i++) {								// 모든 노드들에 대해 탐색하기 위한 for 루프
		if (check[i] == 0) {								// 만약 방문하지 않은 노드라면 if 문 수행
			push(i);										// i노드값을 stack에 집어넣는다.
			check[i] = 1;									// i노드를 방문했음을 기록		
			while (!stack_empty()) {						// 만약 stack이 비어있지않다면 while 루프를 계속돈다.
				i = pop();									// stack에 있는 i노드값을 꺼내온다.
				printf("%3c->", int2name(i));				// 꺼내온 i 값을 출력한다.
				for (t = a[i]; t != NULL; t = t->next) {
					if (check[t->vertex] == 0) {
						push(t->vertex);		           // for 루프를 통해 i노드와 링크연결되어있는 값들의 
						check[t->vertex] = 1;              // 노드값들을 stack에 전부 저장하고 방문했음을 기록한다.
					}
				}
			}
		}
	}

}


void main() {
	int V, E;
	input_adjlist(GL, &V, &E);
	nrDFS_adjlist(GL, V);
	printf("\n");

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