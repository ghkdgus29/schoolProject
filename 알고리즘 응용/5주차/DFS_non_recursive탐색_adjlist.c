#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

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

void input_adjlist(node* a[], int* V, int* E) {				// adjlist�� �����ϴ� �Լ��� ��Ÿ����.
	char vertex[3];										    // AB, AC �� ���� ������ �����ϴ� �迭	
	int i, j;
	node* t;												// linked list �� ����� ���� t�� �����Ѵ�.
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);									// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL�� ��ҵ��� NULL�� �ʱ�ȭ��Ų��.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);								// ���������� vertex �迭�� �����Ѵ�.
		i = name2int(vertex[0]);							// ù��° �������� ����ȭ�Ͽ� i�� �����Ѵ�.
		t = (node*)malloc(sizeof(node));					// t�ּҿ� node����ü��ŭ�� ������ �Ҵ��Ѵ�.
		t->vertex = name2int(vertex[1]);					// t�ּ��� node ����ü�� �ι�° ���������� ����ȭ�Ͽ� �����Ѵ�.
		t->next = a[i];										// next�� NULL ���� �����Ѵ�.
		a[i] = t;											// GL�� i��° ��ҿ� t�� �ּҸ� �����Ѵ�.

		i = name2int(vertex[1]);							// AB = BA �̹Ƿ� ��������� �����ϱ� ���� 1�� 0�� ����� �ѹ� �� ���ش�.
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

void nrDFS_adjlist(node* a[], int V) {						// non recursive �� DFS Ž���� ���� �Լ��̴�.
	int i;													// �ε����� ���� i ���� ����
	node* t;												// list�� �����ϱ� ���� t ������ ���� ����
	top = -1;												// init_stack();
	for (i = 0; i < V; i++) check[i] = 0;					// ��� ���鿡 ���� check���� 0���� �ʱ�ȭ
	for (i = 0; i < V; i++) {								// ��� ���鿡 ���� Ž���ϱ� ���� for ����
		if (check[i] == 0) {								// ���� �湮���� ���� ����� if �� ����
			push(i);										// i��尪�� stack�� ����ִ´�.
			check[i] = 1;									// i��带 �湮������ ���		
			while (!stack_empty()) {						// ���� stack�� ��������ʴٸ� while ������ ��ӵ���.
				i = pop();									// stack�� �ִ� i��尪�� �����´�.
				printf("%3c->", int2name(i));				// ������ i ���� ����Ѵ�.
				for (t = a[i]; t != NULL; t = t->next) {
					if (check[t->vertex] == 0) {
						push(t->vertex);		           // for ������ ���� i���� ��ũ����Ǿ��ִ� ������ 
						check[t->vertex] = 1;              // ��尪���� stack�� ���� �����ϰ� �湮������ ����Ѵ�.
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

	node* p, * s;								// free�� ���� �޸� ������ �����ش�.
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