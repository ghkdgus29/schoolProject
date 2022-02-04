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

void print_directed_adjlist(int a[MAX_NODE], int V) {			// 그래프 출력
	int i, j;
	node* p, * s;

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));
		p = GL[i];
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

typedef struct head {										// outdegree 를 계산하기 위한 head struct
	int count;
	struct node* next;
}head;

head network[MAX_NODE];										// outdegree 값과 그래프연결상태를 저장할 network 배열 

void set_count_outdegree(head net[], int V) {				// outdegree 값 계산 , net[] 은 GL[]로 부터 그래프 연결을 가져올것이다.
	int i;
	int count;
	node* t;
	for (i = 0; i < V; i++) {								// 기준 노드 설정
		count = 0;						// 후행차수 초기화
		for (t = net[i].next; t; t = t->next) {			// 그래프 연결 성분 검사
			count++;									// 그래프 연결 성분들의 수 만큼 후행차수값 증가
		}		
		net[i].count = count;								// 후행차수 값을 해당 network 배열에 저장
	}
}

void revtopsort(head net[], int V) {							// 역위상정렬 함수
	int i, j, k;
	node* ptr;
	init_stack();				// 스택초기화

	set_count_outdegree(net, V);						// 후행차수 설정

	for (i = 0; i < V; i++) if (!net[i].count) push(i);			// 만약 후행차수값이 존재하지 않는다면 (후행과정이 모두 이행되면) push
	for (i = 0; i < V; i++) {
		if (stack_empty()) return -1;						// 스택이 비면 종료
		else {
			j = pop();											// j 에 pop
			printf("%c ", int2name(j));							// j 노드 방문 (출력)
			for (k = 0; k < V; k++) {							// 모든 노드에 대해 
				for (ptr = net[k].next; ptr; ptr = ptr->next) {		// 연결 성분 검사 , k노드는 이전노드, j노드는 후행 노드임
					if (ptr->vertex == j) {						// 만약 후행노드 j가 연결되어 있는 k노드들은
						net[k].count--;							// 모두 후행차수값을 감소
						if (!net[k].count)						// 만약 후행차수값이 0인 k노드는
							push(k);							// push(방문)
					}
				}
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
	revtopsort(network, V);
	printf("\n");
	
	fclose(fp);
}