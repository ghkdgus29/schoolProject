#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100				// ���� �ִ�ũ�⸦ ��Ÿ����.
#define UNSEEN (-INT_MAX)			// �������� ���� ��带 ��Ÿ����.

typedef struct node {				// �׷����� �׸��� ���� ���
	int vertex;
	int weight;
	struct node* next;
}node;

node* G[MAX_NODE];					// �׷����� ���� �迭, �׷����� adj list ���·� ����
int check[MAX_NODE];				// �׷��� ������ ����ġ�� �����ϱ� ���� �迭
int parent[MAX_NODE];				// �׷��� ����� �θ� �����ϱ� ���� �迭
FILE* fp;				
int nheap = 0;						// �������� ũ��
int heap[MAX_NODE];					// ������ ����

int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

void input_adjlist(node* g[], int* V, int* E) {		// ���Ϸκ��� ����� ������, ���� ����, ����ġ�� �Է¹޾�
	char vertex[3];									// �׷����� �����ϴ� �Լ�
	int i, j, w;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) g[i] = NULL;
	for (j = 0; j < *E; j++) {
		fscanf(fp, "%s %d", vertex, &w);
		i = name2int(vertex[0]);
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[1]);
		t->weight = w;
		t->next = g[i];
		g[i] = t;

		i = name2int(vertex[1]);
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[0]);
		t->weight = w;
		t->next = g[i];
		g[i] = t;
	}
}

void print_adjlist(node* g[], int V) {								// ������ �׷����� ����ϴ� �Լ�, ���� ����ġ�� ����Ѵ�.
	int i;
	node* t;
	for (i = 0; i < V; i++) {
		printf("\n%c : ", int2name(i));
		for (t = g[i]; t != NULL; t = t->next) {
			printf("-> %c:%d ", int2name(t->vertex), t->weight);
		}
	}
}

void print_heap(int h[]) {											// �������� �ִ� ���� �� ��忡 �ش��ϴ� ����ġ�� ����ϴ� �Լ�
	int i;
	printf("\n");
	for (i = 1; i <= nheap; i++) printf("%c:%d ", int2name(h[i]), check[h[i]]);
}

int visit(int k) {													// ����� �湮�� ��Ÿ���� �Լ�
	printf("--> %c", int2name(k));
}

void upheap(int h[], int k) {										// �������� ���ο� ��带 ����־� �����Ѵ�. ������ cost(����ġ)�� ���� ���� ���� �ö󰣴�.
	int v;
	v = h[k];
	while (check[h[k / 2]] <= check[v] && k / 2 > 0) {
		h[k] = h[k / 2];
		k /= 2;
	}
	h[k] = v;
}

void downheap(int h[], int k) {										// ���������� k��° ������ �����ϰ� ���� �ٽ� ������ ���� �Լ��� 
	int i, v;														// ���� ���� ��带 �Ѹ��� �Űܿ� �ٽ� ����ġ�� ���� �����Ѵ�.
	v = h[k];
	while (k <= nheap / 2) {
		i = k << 1;				// 2*k
		if (i < nheap && check[h[i]] < check[h[i + 1]]) i++;
		if (check[v] >= check[h[i]]) break;
		h[k] = h[i];
		k = i;
	}
	h[k] = v;
}

int pq_empty() {							// ���� ������� �˻��ϴ� �Լ��� ����� ��� 1, �ƴҰ�� 0�� ����
	if (nheap == 0) return 1;
	return 0;
}

int pq_extract(int h[]) {					// ������ �Ѹ��� �ش��ϴ� ������ �����ϰ� , �ٽ������ϴ� �Լ�
	int v = h[1];
	h[1] = h[nheap--];
	downheap(h, 1);
	return v;
}

void adjust_heap(int h[], int n) {						// �����е��� �ٽ� �����ϱ� ���� �Լ�
	int k;
	for (k = n / 2; k >= 1; k--) downheap(h, k);
}

int pq_update(int h[], int v, int p) {				// ���� ������ ��������� ��尡 ó������ִ� ����̸� if(check[v] == UNSEEN)����
	if (check[v] == UNSEEN) {						// �����Ͽ� �Է¹��� cost ���� v��° �������� ����ְ� ����
		h[++nheap] = v;
		check[v] = p;
		upheap(h, nheap);
		return 1;
	}
	else {											// ���� �ѹ� �̻� ����־��� ����ϰ��, cost���� �񱳸� �����Ͽ� 		
		if (check[v] < p) {							// cost�� ������ ����, cost�� ũ�� �״�� �д�.
			check[v] = p;
			adjust_heap(h, nheap);
			return 1;
		}
		else return 0;
	}
}

void pq_init() {								// ���� �ʱ�ȭ��Ű�� ���� �Լ�
	nheap = 0;
}

void PFS_adjlist(node* g[], int V) {			// ���� ������ �Լ����� ��� ������ �Լ��� PFSŽ���� �����ϴ� �Լ��̴�.
	int i;										// �����߿� parent �� check �迭�� ä�������� parent ���� cost�� ����Ѵ�.
	node* t;									// ���� visit�Լ��� ���� �湮�� ��带 ��������ν� Ž�������� Ȯ���� �� �ִ�.
	pq_init();
	for (i = 0; i < V; i++) {
		check[i] = UNSEEN;
		parent[i] = 0;
	}

	for (i = 0; i < V; i++) {
		if (check[i] == UNSEEN) {
			parent[i] = -1;
			pq_update(heap, i, UNSEEN);
			while (!pq_empty()) {
				print_heap(heap);
				i = pq_extract(heap);
				check[i] = -check[i];
				visit(i);
				for (t = g[i]; t != NULL; t = t->next) {
					if (check[t->vertex] < 0) {
						if (pq_update(heap, t->vertex, -t->weight))
							parent[t->vertex] = i;
					}
				}
			}	
		}	
	}
}

void print_tree(int p[], int V) {							// ������ �θ� ����ϴ� �Լ��ν� ��Ʈ�� @�� ��µȴ�.
	int i;
	printf("\n\nTree structure\nson :    ");
	for (i = 0; i < V; i++) {
		printf("%c ", int2name(i));
	}
	printf("\n------------------------------------\nparent : ");
	for (i = 0; i < V; i++) {
		printf("%c ", int2name(parent[i]));
	}
}

void print_cost(int c[], int V) {							// ������ ����ġ(cost)�� ��� ���� ������ ��θ� Ž���ϴ� 
	int i, sum = 0;											// �ּҺ���� ����Ѵ�.
	for (i = 1; i < V; i++) sum += check[i];
	printf("%d\n", sum);
}

void main() {
	int V, E;
	fp = fopen("graph.txt", "rt");							// �׷����� ���� ������ ����ִ� �ؽ�Ʈ����

	input_adjlist(G, &V, &E);
	printf("\nOrigianl graph\n");						
	print_adjlist(G, V);									// �׷��� ���
	printf("\nVisit order of Minimum Spanning Tree\n");
	PFS_adjlist(G, V);										// PFS Ž���� ���� ���� spanning Tree  ���
	print_tree(parent, V);									// �� ������ �θ��� ���	
	printf("\n\nMinimum Cost is ");									
	print_cost(check, V);									// Ž���� �� ��� ���
	fclose(fp);
}