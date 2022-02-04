#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;
int order = 0;

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

typedef struct node {
	int vertex;
	struct node* next;
}node;

node* GL[MAX_NODE];

void input_directed_adjlist(node* a[], int* V, int* E) {
	char vertex[3];
	int i, j;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) a[i] = NULL;
	for (j = 0; j < *E; j++) {
		fscanf(fp, "%s", vertex);
		i = name2int(vertex[0]);
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[1]);
		t->next = a[i];
		a[i] = t;
	}
}

void print_directed_adjlist(node* a[], int V) {
	int i, j;
	node* p, * s;

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));
		p = GL[i];
		for (s = p; s != NULL; s = s->next) 
			printf("->%3c ", int2name(s->vertex));
	}
}

#define MAX 1000
int stack[MAX];
int top;
int check[MAX];

int push(int t) {
	if (top >= MAX - 1) {
		printf("stack overflow!\n");
		return -1;
	}
	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf("stack underflow!\n");
		return -1;
	}
	return stack[top--];
}

int stack_empty() {
	return (top < 0);
}

void init_stack() {
	top = -1;
}

int strong_recur(node* g[], int i) {		// strong connect를 조사하는 함수
	int m, min, k, flag;
	node* t;
	check[i] = min = ++order;			// 노드의 방문순서에 따라 차수와 최소값 설정, check는 방문순서
	push(i);							// 방문노드 push
	for (t = g[i]; t != NULL; t = t->next) {	// 방문 노드의 연결노드들 조사
		if (check[t->vertex] == 0)				// 만약 연결노드를 방문하지 않았다면
			m = strong_recur(g, t->vertex);		// DFS 탐색처럼 계속해서 recursive 방문
		else
			m = check[t->vertex];				// 연결노드를 방문했다면 m = 해당 노드의 방문순서값
		if (m < min) min = m;			// 만약 m보다 min값이 크면 min = m, 즉 선행노드의 차수값이 min값이 된다.
	}
	if (min == check[i]) {			// 차수값과 방문순서가 같으면 해당노드의 연결들(스택) 모두 조사
		flag = 0;			// strongly connected 를 나타내는 flag , 0이면 해당사항없음

		while ((k = pop()) != i) {		// 스택에서 꺼낸 값이 현재 방문 노드와 같을 때 까지 반복
			printf("%c ", int2name(k));
			check[k] = MAX_NODE + 1;		// 중복을 방지하기 위해 방문순서값을 엄청 큰수로 변경
			flag = 1;			// strongly connected 를 나타내는 flag, 1이므로 꺼낸 스택값들은 모두 strong connect
		}
		if (flag) printf("%c\n", int2name(k));	// flag가 1일 경우 이 k노드까지 모두 strong connect
	}
	return min;		//  해당하는 최소차수 값 반환
}

void strong(node* g[], int V) {			// strong connect wrapper 함수
	int x, y;
	for (y = 0; y < V; y++)
		check[y] = 0;					// 방문 노드0으로 초기화 -> 모두 방문하지 않음
	for (y = 0; y < V; y++) {
		if (check[y] == 0)				// 만약 방문하지 않았다면
			strong_recur(g, y);			// strong recur 방문
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");

	int V, E;
	printf("original graph");
	input_directed_adjlist(GL, &V, &E);
	print_directed_adjlist(GL, V);				// 그래프 출력

	printf("\n\nstrong connect\n");

	strong(GL, V);								// strong connect 노드 출력

	fclose(fp);
}