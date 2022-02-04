#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int GM[MAX_NODE][MAX_NODE];										// adjmatrix 를 나타내기 위한 행렬

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// adjmatrix를 구현하는 함수이다.
	char vertex[3];													// AB, AC 와 같은 선분을 저장하는 배열
	int i, j, k;
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);											// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM 배열을 0으로 전부 초기화한다.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 1;							// 대각 성분을 모두 1로 초기화한다.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);										// 선분의 정보를 vertex배열에 저장한다. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// 입력받은 선분 정보를 정수화한다.
		a[i][j] = 1;
		a[j][i] = 1;												// 입력받은 정보를 바탕으로 해당하는 행렬 성분을 1로 체크한다.(선을 잇는다.)
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix를 출력하는 함수이다.
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%3c", int2name(i));				// 행렬에서 열에 해당하는 꼭지점 성분들을 출력한다. (맨위 가로줄)

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%3c", int2name(i));									// 행렬에서 행에 해당하는 꼭지점 성분들을 출력한다. (왼쪽 세로줄)
		for (j = 0; j < V; j++) printf("%3d", a[i][j]);				// 행렬 성분들을 해당하는 0과 1로 나타낸다.
		printf("\n");
	}
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
	node* p, * s;											// linked list를 이용하기 위한 p와 free를 위한 s를 선언한다.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. 와 같은 vertex 성분들을 왼쪽 시작부분에 단어화하여 출력한다.
		p = GL[i];											// p에 GL[i]에 해당하는 주소값을 저장한다. 즉 linked list에 접근하기 위함이다.
		while (p != NULL) {									// 만약 p주소가 NULL 값을 가리키면 while문을 빠져나간다.
			printf("-> %3c ", int2name(p->vertex));			// p주소의 node 구조체의 vertex값을 단어화하여 출력한다.
			s = p;											// p주소값을 s주소에 할당한다.
			p = p->next;									// p주소값을 다음 노드로 옮긴다.
			free(s);										// 출력이 끝난 노드는 제거한다.
		}
	}
}

//-------------------↑↑↑↑↑↑graph 그리기↑↑↑↑↑↑---------------------------------
//-------------------↓↓↓↓↓↓graph 탐색하기-DFS↓↓↓↓↓---------------------------------

int check[MAX_NODE];

void DFS_recur_matrix(int a[][MAX_NODE], int V, int i) {
	int j;
	check[i] = 1;
	printf("%3c->", int2name(i));
	for (j = 0; j < V; j++) {
		if (a[i][j] != 0) {
			if (check[j] == 0) {
				DFS_recur_matrix(a, V, j);
			}
		}
	}
}

void DFS_adjmatrix(int a[][MAX_NODE], int V) {
	int i;
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) DFS_recur_matrix(a, V, i);
	}
}

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

void nrDFS_adjmatrix(int a[][MAX_NODE], int V) {
	int i, j;
	top = -1; // init_stack();
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) {
			push(i);
			check[i] = 1;
			while (!stack_empty()) {
				i = pop();
				printf("%3c->", int2name(i));
				for (j = 0; j < V; j++) {
					if (a[i][j] != 0) {
						if (check[j] == 0) {
							push(j);
							check[j] = 1;
						}
					}
				}
			}
		}
	}
}

void count_components(int a[][MAX_NODE], int V) {				// 단절된 그래프 개수 세기
	int i, j, k, cnt = 0;

	init_stack();
	for (i = 0; i < V; i++)
		check[i] = 0;

	for (i = 0; i < V; i++) {
		if (check[i] == 0) {
			push(i);
			check[i] = 1;
			cnt++;
			while (!is_stack_empty()) {
				k = pop();
				visit(k);
				for (j = 0; j < V; j++) {
					if (a[k][j] != 0) {
						if (check[j] == 0) {
							push(j);
							check[j] = 1;
						}
					}
				}
			}
		}
	}

	printf("\n\n그래프 개수 %d!!", cnt);
}

void DFS_recur_list(node* a[], int V, int i) {
	node* t;
	check[i] = 1;
	printf("%3c->", int2name(i));
	for (t = a[i]; t != NULL; t = t->next) {
		if (check[t->vertex] == 0) DFS_recur_list(a, V, t->vertex);
	}
}

void DFS_adjlist(node* a[], int V) {
	int i;
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) DFS_recur_list(a, V, i);
	}
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
	int V, E;									// 꼭지점V와 선분E를 나타내는 변수선언

	input_adjmatrix(GM, &V, &E);				// adjmatrix 입력


	printf("\n");
	DFS_adjmatrix(GM, V);						// recursive DFS adjmatrix 함수 실행
	printf(" Done\n");
	nrDFS_adjmatrix(GM, V);						// non recursive DFS adjmatrix 함수 실행
	printf(" Done\n");

	printf("\n\n");

	input_adjlist(GL, &V, &E);					// adjlist 입력


	printf("\n");
	DFS_adjlist(GL, V);							// recursive DFS adjlist 함수를 실행						
	printf(" Done\n");

	nrDFS_adjlist(GL, V);						// non recursive DFS adjlist 함수를 실행
	printf(" Done\n");

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