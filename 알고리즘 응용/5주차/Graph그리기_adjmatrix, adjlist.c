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

void main() {
	int V, E;
	input_adjmatrix(GM, &V, &E);
	print_adjmatrix(GM, V);

	printf("\n\n");

	input_adjlist(GL, &V, &E);
	print_adjlist(GL, V);
	printf("\n");


}