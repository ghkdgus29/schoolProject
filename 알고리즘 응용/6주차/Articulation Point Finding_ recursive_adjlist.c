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
int order;
int son_of_root;

int AP_recur(node* a[], int i) {
	node* t;
	int min, m;
	check[i] = min = ++order;
	for (t = a[i]; t != NULL; t = t->next) {
		if (i == 0 && check[t->vertex] == 0) son_of_root++;
		if (check[t->vertex] == 0) {
			m = AP_recur(a, t->vertex);
			if (m < min) min = m;
			if (m >= check[i] && i != 0) printf("*%c %2d : %d\n", int2name(i), check[i], m);
			else printf(" %c %2d : %d\n", int2name(i), check[i], m);	
		}
		else {
			if (check[t->vertex] < min) min = check[t->vertex];
		}
	}
	return min;
}

void AP_search(node* a[], int V) {
	int i, n = 0;
	node* t;

	for (i = 0; i < V; i++) check[i] = 0;
	order = son_of_root = 0;
	AP_recur(a, 0);

	if (son_of_root > 1) printf("*");
	else printf(" ");
	printf("%c son : %d\n", int2name(0), son_of_root);
}

void main() {
	int V, E;						// node �� edge�� �Է��ϱ� ���� ����
	input_adjlist(GL, &V, &E);		// adjlist �������� �׷��� ����
	printf("\n");					
	AP_search(GL, V);				// AP_search �Լ��� ȣ���Ͽ� DFS Ž���� �ϸ� Articulation point �� ã�´�.
}