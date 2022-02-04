#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;


int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

typedef struct node {												// adjlist �� ���� node ����ü�̴�.
	int vertex;														// vertex�� ������ ������ ��Ÿ����
	struct node* next;												// next�� linked list�� ������ ��� �ּҸ� ��Ÿ����.
}node;

node* GL[MAX_NODE];											// NODE����ü�� �ּҸ� �����ϴ� GL �迭�� �����Ѵ�.

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed adjlist�� �����ϴ� �Լ��� ��Ÿ����.
	char vertex[3];										    // AB, AC �� ���� ������ �����ϴ� �迭	
	int i, j;
	node* t;												// linked list �� ����� ���� t�� �����Ѵ�.
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);									// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL�� ��ҵ��� NULL�� �ʱ�ȭ��Ų��.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		fscanf(fp, "%s", vertex);								// ���������� vertex �迭�� �����Ѵ�.
		i = name2int(vertex[0]);							// ù��° �������� ����ȭ�Ͽ� i�� �����Ѵ�.
		t = (node*)malloc(sizeof(node));					// t�ּҿ� node����ü��ŭ�� ������ �Ҵ��Ѵ�.
		t->vertex = name2int(vertex[1]);					// t�ּ��� node ����ü�� �ι�° ���������� ����ȭ�Ͽ� �����Ѵ�.
		t->next = a[i];										// next�� NULL ���� �����Ѵ�.
		a[i] = t;											// GL�� i��° ��ҿ� t�� �ּҸ� �����Ѵ�.

	}
}

void print_directed_adjlist(node* a[], int V) {				// adjlist�� ����ϱ� ���� �Լ��̴�. GL�迭�� �Ű������� �޴´�.
	int i, j;
	node* p, * s;											// linked list�� �̿��ϱ� ���� p�� free�� ���� s�� �����Ѵ�.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. �� ���� vertex ���е��� ���� ���ۺκп� �ܾ�ȭ�Ͽ� ����Ѵ�.
		p = a[i];											// p�� GL[i]�� �ش��ϴ� �ּҰ��� �����Ѵ�. �� linked list�� �����ϱ� �����̴�.
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
}													// nr - DFS �� ����ϱ� ���� ���� ����

void init_stack() {
	top = -1;
}


void DFS_directed_adjlist(node* a[], int V) {		// reachability �� �����ϴ� �Լ�, DFS ����
	int i, j;
	node* t;
	init_stack();
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++)
			check[j] = 0;			// ��� ����� �湮 �ʱ�ȭ

		push(i);			// i��� push
		check[i] = 1;		// i��� �湮üũ
		printf("\n%c : ", int2name(i));		// i����� reachability ǥ��
		while (!stack_empty()) {		// stack�� �������� �ݺ�
			j = pop();						// j���� stack���� pop�� ���		
			printf("%3c->", int2name(j));	// j���� ���ް����� ������� ���
			for (t = a[j]; t != NULL; t = t->next) {		// j��忡 ��������� ���� ����
				if (check[t->vertex] == 0) {		// ���� �湮���� ���� �����
					push(t->vertex);				// �ش��� push
					check[t->vertex] = 1;			// �ش��� �湮üũ
				}
			}
		}
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");
	int V, E;

	input_directed_adjlist(GL, &V, &E);		// �׷��� �о���̱�

	printf("\n\nOriginal graph\n");			// �׷��� ���
	print_directed_adjlist(GL, V);
	printf("\n\n");

	DFS_directed_adjlist(GL, V);			// reachability ã��

	fclose(fp);
}