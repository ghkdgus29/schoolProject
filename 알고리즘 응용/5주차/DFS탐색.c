#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int GM[MAX_NODE][MAX_NODE];										// adjmatrix �� ��Ÿ���� ���� ���

int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// adjmatrix�� �����ϴ� �Լ��̴�.
	char vertex[3];													// AB, AC �� ���� ������ �����ϴ� �迭
	int i, j, k;
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);											// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM �迭�� 0���� ���� �ʱ�ȭ�Ѵ�.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 1;							// �밢 ������ ��� 1�� �ʱ�ȭ�Ѵ�.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);										// ������ ������ vertex�迭�� �����Ѵ�. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// �Է¹��� ���� ������ ����ȭ�Ѵ�.
		a[i][j] = 1;
		a[j][i] = 1;												// �Է¹��� ������ �������� �ش��ϴ� ��� ������ 1�� üũ�Ѵ�.(���� �մ´�.)
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix�� ����ϴ� �Լ��̴�.
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%3c", int2name(i));				// ��Ŀ��� ���� �ش��ϴ� ������ ���е��� ����Ѵ�. (���� ������)

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%3c", int2name(i));									// ��Ŀ��� �࿡ �ش��ϴ� ������ ���е��� ����Ѵ�. (���� ������)
		for (j = 0; j < V; j++) printf("%3d", a[i][j]);				// ��� ���е��� �ش��ϴ� 0�� 1�� ��Ÿ����.
		printf("\n");
	}
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

void print_adjlist(int a[MAX_NODE], int V) {				// adjlist�� ����ϱ� ���� �Լ��̴�. GL�迭�� �Ű������� �޴´�.
	int i, j;
	node* p, * s;											// linked list�� �̿��ϱ� ���� p�� free�� ���� s�� �����Ѵ�.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. �� ���� vertex ���е��� ���� ���ۺκп� �ܾ�ȭ�Ͽ� ����Ѵ�.
		p = GL[i];											// p�� GL[i]�� �ش��ϴ� �ּҰ��� �����Ѵ�. �� linked list�� �����ϱ� �����̴�.
		while (p != NULL) {									// ���� p�ּҰ� NULL ���� ����Ű�� while���� ����������.
			printf("-> %3c ", int2name(p->vertex));			// p�ּ��� node ����ü�� vertex���� �ܾ�ȭ�Ͽ� ����Ѵ�.
			s = p;											// p�ּҰ��� s�ּҿ� �Ҵ��Ѵ�.
			p = p->next;									// p�ּҰ��� ���� ���� �ű��.
			free(s);										// ����� ���� ���� �����Ѵ�.
		}
	}
}

//-------------------�������graph �׸���������---------------------------------
//-------------------�������graph Ž���ϱ�-DFS������---------------------------------

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

void count_components(int a[][MAX_NODE], int V) {				// ������ �׷��� ���� ����
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

	printf("\n\n�׷��� ���� %d!!", cnt);
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
	int V, E;									// ������V�� ����E�� ��Ÿ���� ��������

	input_adjmatrix(GM, &V, &E);				// adjmatrix �Է�


	printf("\n");
	DFS_adjmatrix(GM, V);						// recursive DFS adjmatrix �Լ� ����
	printf(" Done\n");
	nrDFS_adjmatrix(GM, V);						// non recursive DFS adjmatrix �Լ� ����
	printf(" Done\n");

	printf("\n\n");

	input_adjlist(GL, &V, &E);					// adjlist �Է�


	printf("\n");
	DFS_adjlist(GL, V);							// recursive DFS adjlist �Լ��� ����						
	printf(" Done\n");

	nrDFS_adjlist(GL, V);						// non recursive DFS adjlist �Լ��� ����
	printf(" Done\n");

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