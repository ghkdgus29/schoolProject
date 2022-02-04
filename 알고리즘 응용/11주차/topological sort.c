#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;

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

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed 그래프를 adj list 이용해서 그린다
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

void print_directed_adjlist(node* a[MAX_NODE], int V) {			// 그래프 출력
	int i, j;
	node* p, * s;
	
	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));
		p = a[i];
		for (s = p; s != NULL; s = s->next) printf("-> %3c ", int2name(s->vertex));
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
		printf("stack underflow! \n");
		return -1;
	}
	return stack[top--];
}

int stack_empty() {
	return (top < 0);
}

void init_stack() {
	top = -1;
}															// 스택설정

typedef struct head {										// indegree 를 계산하기 위한 head struct
	int count;
	struct node* next;
}head;

head network[MAX_NODE];										// indegree 값과 그래프연결상태를 저장할 network 배열 

void set_count_indegree(head net[], int V) {				// indegree 값 계산 , net[] 은 GL[]로 부터 그래프 연결을 가져올것이다.
	int i, j;
	int count;
	node* t;
	for (i = 0; i < V; i++) {								// 기준 노드 설정
		count = 0;						// 선행차수 초기화
		for (j = 0; j < V; j++) {							// 처음부터 모든 노드에 대하여
			for (t = net[j].next; t; t = t->next) {			// 그래프 연결 성분 검사, 만약 그래프 연결성분중 
				if (t->vertex == i) count++;				// 기준노드와 같은 값이 있다면 (선행차수 존재) 선행차수값을 증가시킨다.
			}
		}
		net[i].count = count;								// 선행차수 값을 해당 network 배열에 저장
	}
}

void topsort(head net[], int V) {							// 위상정렬 함수
	int i, j, k;
	node* ptr;
	init_stack();				// 스택초기화

	set_count_indegree(net, V);						// 선행차수 설정

	for (i = 0; i < V; i++) if (!net[i].count) push(i);			// 만약 선행차수값이 존재하지 않는다면 (선행과정이 모두 이행되면) push
	for (i = 0; i < V; i++) {
		if (stack_empty()) return -1;						// 스택이 비면 종료
		else {
			j = pop();											// j 에 pop
			printf("%c ",int2name(j));							// j 노드 방문 (출력)
			for (ptr = net[j].next; ptr; ptr = ptr->next) {		// j노드 그래프 연결 성분 검사, 
				k = ptr->vertex;								// k에 j노드 그래프 연결 성분들의 vertex값 저장
				net[k].count--;									// j노드 연결 성분들의 선행차수값 감소 (j노드를 수행했으니까)
				if (!net[k].count) push(k);						// 만약 j노드 이후 연결 성분들 중 선행 과정이 모두 이행된 값은 push
			}
		}
	}
}

void main() {
	fp = fopen("topsort.txt", "rt");

	int V, E;

	input_directed_adjlist(GL, &V, &E);			// 파일로 부터 읽은 그래프 출력
	printf("\n\nOriginal graph\n");
	print_directed_adjlist(GL, V);
	
	for (int i = 0; i < V; i++) network[i].next = GL[i];		// GL에 있는 그래프 연결정보를 network에 옮긴다.

	printf("\n\nTop sort\n");					// Top sort 
	topsort(network, V);
	printf("\n");
	
	fclose(fp);
}