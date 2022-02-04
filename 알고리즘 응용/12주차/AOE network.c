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
	int weight;
	struct node* next;
}node;

node* GL[MAX_NODE];

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed 그래프를 adj list 이용해서 그린다
	char vertex[3];
	int i, j, weight;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) a[i] = NULL;
	for (j = 0; j < *E; j++) {
		fscanf(fp, "%s %d", vertex, &weight);
		i = name2int(vertex[0]);
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[1]);
		t->weight = weight;
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
		for (s = p; s != NULL; s = s->next) printf("-> %3c:%2d ", int2name(s->vertex), s->weight);
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

typedef struct head {
	int count;
	int weight;
	struct node* next;
}head;

head network[MAX_NODE];
int earliest[MAX_NODE];
int latest[MAX_NODE];



void set_count_indegree(head net[], int V) {
	int i, j;
	int count;
	node* t;
	for (i = 0; i < V; i++) {
		count = 0;
		for (j = 0; j < V; j++) {
			for (t = net[j].next; t; t = t->next) {
				if (t->vertex == i)count++;
			}
		}
		net[i].count = count;
	}
}

void set_count_outdegree(head net[], int V) {
	int i;
	int count;
	node* t;
	for (i = 0; i < V; i++) {
		count = 0;
		for (t = net[i].next; t; t = t->next) {
			count++;
		}
		net[i].count = count;
	}
}

void forward_state(head net[], int V) {					// earliest 를 계산하는 함수
	int i, j, k;
	node* ptr;

	init_stack();
	set_count_indegree(net, V);

	for (i = 0; i < V; i++) earliest[i] = 0;			// 각 노드들의 earliest 초기화
	for (i = 0; i < V; i++)
		if (!net[i].count) push(i);						// 선행차수 값이 없으면 push
	for (i = 0; i < V; i++) {
		if (!stack_empty()) {
			j = pop();
			for (ptr = net[j].next; ptr; ptr = ptr->next) {		// j노드와의 연결성분들 조사
				k = ptr->vertex;								// k = j노드와의 연결 노드들
				net[k].count--;									// j노드를 방문했으니 k노드들(연결노드들) 선행차수값 감소
				if (!net[k].count)				// 선행차수값이 없으면
					push(k);					// k노드 push(방문)
				if (earliest[k] < earliest[j] + ptr->weight)		// k노드 까지의 earliest 가중치보다 다른 경로(j노드 까지 가중치 + <j, k>) 가 크면
					earliest[k] = earliest[j] + ptr->weight;		// earliest[k] 갱신, k노드 까지 가는 가장 큰 값이 저장됨
			}										// accumulation
		}
	}
}

void backward_state(head net[], int V) {
	int i, j, k, l;
	node* ptr;

	init_stack();
	set_count_outdegree(net, V);

	for (i = 0; i < V; i++) latest[i] = earliest[V - 1];		// latest를 earliest의 가장 큰 값으로 초기화
	for (i = 0; i < V; i++)
		if (!net[i].count) push(i);								// 후행차수값이 없으면 push
	for (i = 0; i < V; i++) {
		if (!stack_empty()) {
			j = pop();
			for (l = 0; l < V; l++) {				// 모든 노드 조사
				for (ptr = net[l].next; ptr; ptr = ptr->next) {		// 각 노드의 연결성분을 조사한다
					if (ptr->vertex == j) {			// 연결 성분중 j노드(방문노드)가 있는 경우
						k = l;						// k노드는 j노드 이전노드이다.
						net[k].count--;				// k노드의 후행차수 값을 감소시킨다.
						if (!net[k].count)
							push(k);				// 만약 k노드의 후행차수값이 없으면 k노드 push
						if (latest[k] > latest[j] - ptr->weight)		// 만약 k노드의 latest 가중치보다 다른 경로(latest[j] - <k,j>) 가 작으면
							latest[k] = latest[j] - ptr->weight;		// latest[k] 갱신, k노드 까지 뒤에서 가는 가장 작은 값이 저장된다
					}								// accumulation
				}
			}
		}
	}
}

char yes_no(int i) {				// 정수값이 0이면 Y, 다르면 N을 반환
	if (i == 0) return 'Y';
	return 'N';
}

void print_critical_activity(head net[], int V) {							// critical activity를 찾는 함수
	int i, j;
	printf("\nEdge     Early   Late   Late-Early   임계경로 ");
	node* t;
	for (i = 0; i < V; i++) {							// 모든 노드에 대하여
		for (t = net[i].next; t; t = t->next) {					// i노드의 연결 노드에 대하여
			printf("\n<%c %c>   %3d     %3d     %3d            %c", int2name(i), int2name(t->vertex), earliest[i],		// i노드, i연결노드, early<i, i연결> 출력
				latest[t->vertex]-t->weight, latest[t->vertex]-t->weight-earliest[i],	// late<i, i연결>, early-late 출력, late의 경우 latest[i연결노드]- <i, i연결노드> 값을 출력, <i, i연결노드> 가중치는 i연결노드가 갖고있다.
				yes_no(latest[t->vertex] - t->weight - earliest[i]));					// early- late 값을 판별해 같으면 Y, 다르면 N 출력
		}
	}
}

void main() {
	fp = fopen("graph_AOE.txt", "rt");

	int V, E;
	int i;

	input_directed_adjlist(GL, &V, &E);			// 파일로 부터 읽은 그래프 출력
	printf("\n\nOriginal graph\n");
	print_directed_adjlist(GL, V);

	for (i = 0; i < V; i++) {
		network[i].next = GL[i];				// network에 GL 그래프 값들 전달
	}

	forward_state(network, V);					// earliest	 만들기
	backward_state(network, V);					// latest	만들기

	printf("\n\nearlist\n");
	for (i = 0; i < V; i++) {
		printf("%3d", earliest[i]);				// earliest 출력
	}

	printf("\nlatest\n");
	for (i = 0; i < V; i++) {	
		printf("%3d", latest[i]);				// latest 출력
	}
	printf("\n\n");

	print_critical_activity(network, V);		// criticial_activity 출력
	fclose(fp);
}