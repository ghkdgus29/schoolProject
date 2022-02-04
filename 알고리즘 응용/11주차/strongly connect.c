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

int strong_recur(node* g[], int i) {		// strong connect�� �����ϴ� �Լ�
	int m, min, k, flag;
	node* t;
	check[i] = min = ++order;			// ����� �湮������ ���� ������ �ּҰ� ����, check�� �湮����
	push(i);							// �湮��� push
	for (t = g[i]; t != NULL; t = t->next) {	// �湮 ����� ������� ����
		if (check[t->vertex] == 0)				// ���� �����带 �湮���� �ʾҴٸ�
			m = strong_recur(g, t->vertex);		// DFS Ž��ó�� ����ؼ� recursive �湮
		else
			m = check[t->vertex];				// �����带 �湮�ߴٸ� m = �ش� ����� �湮������
		if (m < min) min = m;			// ���� m���� min���� ũ�� min = m, �� �������� �������� min���� �ȴ�.
	}
	if (min == check[i]) {			// �������� �湮������ ������ �ش����� �����(����) ��� ����
		flag = 0;			// strongly connected �� ��Ÿ���� flag , 0�̸� �ش���׾���

		while ((k = pop()) != i) {		// ���ÿ��� ���� ���� ���� �湮 ���� ���� �� ���� �ݺ�
			printf("%c ", int2name(k));
			check[k] = MAX_NODE + 1;		// �ߺ��� �����ϱ� ���� �湮�������� ��û ū���� ����
			flag = 1;			// strongly connected �� ��Ÿ���� flag, 1�̹Ƿ� ���� ���ð����� ��� strong connect
		}
		if (flag) printf("%c\n", int2name(k));	// flag�� 1�� ��� �� k������ ��� strong connect
	}
	return min;		//  �ش��ϴ� �ּ����� �� ��ȯ
}

void strong(node* g[], int V) {			// strong connect wrapper �Լ�
	int x, y;
	for (y = 0; y < V; y++)
		check[y] = 0;					// �湮 ���0���� �ʱ�ȭ -> ��� �湮���� ����
	for (y = 0; y < V; y++) {
		if (check[y] == 0)				// ���� �湮���� �ʾҴٸ�
			strong_recur(g, y);			// strong recur �湮
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");

	int V, E;
	printf("original graph");
	input_directed_adjlist(GL, &V, &E);
	print_directed_adjlist(GL, V);				// �׷��� ���

	printf("\n\nstrong connect\n");

	strong(GL, V);								// strong connect ��� ���

	fclose(fp);
}