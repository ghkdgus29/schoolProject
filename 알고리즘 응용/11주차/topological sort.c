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

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed �׷����� adj list �̿��ؼ� �׸���
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

void print_directed_adjlist(node* a[MAX_NODE], int V) {			// �׷��� ���
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
}															// ���ü���

typedef struct head {										// indegree �� ����ϱ� ���� head struct
	int count;
	struct node* next;
}head;

head network[MAX_NODE];										// indegree ���� �׷���������¸� ������ network �迭 

void set_count_indegree(head net[], int V) {				// indegree �� ��� , net[] �� GL[]�� ���� �׷��� ������ �����ð��̴�.
	int i, j;
	int count;
	node* t;
	for (i = 0; i < V; i++) {								// ���� ��� ����
		count = 0;						// �������� �ʱ�ȭ
		for (j = 0; j < V; j++) {							// ó������ ��� ��忡 ���Ͽ�
			for (t = net[j].next; t; t = t->next) {			// �׷��� ���� ���� �˻�, ���� �׷��� ���Ἲ���� 
				if (t->vertex == i) count++;				// ���س��� ���� ���� �ִٸ� (�������� ����) ������������ ������Ų��.
			}
		}
		net[i].count = count;								// �������� ���� �ش� network �迭�� ����
	}
}

void topsort(head net[], int V) {							// �������� �Լ�
	int i, j, k;
	node* ptr;
	init_stack();				// �����ʱ�ȭ

	set_count_indegree(net, V);						// �������� ����

	for (i = 0; i < V; i++) if (!net[i].count) push(i);			// ���� ������������ �������� �ʴ´ٸ� (��������� ��� ����Ǹ�) push
	for (i = 0; i < V; i++) {
		if (stack_empty()) return -1;						// ������ ��� ����
		else {
			j = pop();											// j �� pop
			printf("%c ",int2name(j));							// j ��� �湮 (���)
			for (ptr = net[j].next; ptr; ptr = ptr->next) {		// j��� �׷��� ���� ���� �˻�, 
				k = ptr->vertex;								// k�� j��� �׷��� ���� ���е��� vertex�� ����
				net[k].count--;									// j��� ���� ���е��� ���������� ���� (j��带 ���������ϱ�)
				if (!net[k].count) push(k);						// ���� j��� ���� ���� ���е� �� ���� ������ ��� ����� ���� push
			}
		}
	}
}

void main() {
	fp = fopen("topsort.txt", "rt");

	int V, E;

	input_directed_adjlist(GL, &V, &E);			// ���Ϸ� ���� ���� �׷��� ���
	printf("\n\nOriginal graph\n");
	print_directed_adjlist(GL, V);
	
	for (int i = 0; i < V; i++) network[i].next = GL[i];		// GL�� �ִ� �׷��� ���������� network�� �ű��.

	printf("\n\nTop sort\n");					// Top sort 
	topsort(network, V);
	printf("\n");
	
	fclose(fp);
}