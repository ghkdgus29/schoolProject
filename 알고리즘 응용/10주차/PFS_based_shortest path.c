#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100
#define UNSEEN (-INT_MAX)

typedef struct node {
	int vertex;
	int weight;
	struct node* next;
}node;

node* G[MAX_NODE];		// �׷��� ������ ����
int check[MAX_NODE];	// �湮 ����, ����ġ���� ����
int parent[MAX_NODE];	// �� ������ �θ��� ����
FILE* fp;
int nheap = 0;			// �� ����
int heap[MAX_NODE];		// heap , �ε��� �� ����

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

void visit(int k) {
	printf("--> %c", int2name(k));		// ��� �湮
}

void input_adjlist(node* g[], int* V, int* E) {		// file�κ��� �׷��� ���� ���� (adj list)
	char vertex[3];
	int i, j, w;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) g[i] = NULL;		// ��� ���� �ʱ�ȭ
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

void print_adjlist(node* g[], int V) {		// �׷��� ǥ��
	int i;
	node* t;
	for (i = 0; i < V; i++) {
		printf("\n%c : ", int2name(i));
		for (t = g[i]; t != NULL; t = t->next) {
			printf("-> %c:%d", int2name(t->vertex), t->weight);
		}
	}
}

void pq_init() {
	nheap = 0;			// heap �ʱ�ȭ
}

int pq_empty() {
	return (nheap == 0);
}

void upheap(int h[], int k) {		// k�� ���� ���� ���� (�ǳ�)
	int v;
	v = h[k];		// v = ���� �ǳ���
	while (check[h[k / 2]] <= check[v] && k / 2 > 0) {		// �θ� �ε��� ����ġ <= v(�ڽ�)�ε��� ����ġ , �θ� ��尡 0�� �ƴϸ� �ݺ�	
		h[k] = h[k / 2];		// �θ��带 �������
		k /= 2;					// ������ �Ǵ� k��带 ���� �ø���
	}
	h[k] = v;		// ��� ���� ������ �� k��ġ�� v(ó�� ���� �ǳ���)�� ��ġ�̴�.
}

void downheap(int h[], int k) {			// k�� ���� ���� �� ó�� (�Ѹ�)
	int i, v;
	v = h[k];
	while (k <= nheap / 2) {			// internal node�� ���� �ݺ�
		i = k << 1;		// i=2*k, i�� �ڽĳ��, k�� �θ���
		if (i < nheap && check[h[i]] < check[h[i + 1]]) i++;	// �ڽĳ���� �ε��� ����ġ�� ū ���� i��尡 �ȴ�
		if (check[v] >= check[h[i]]) break;		// v���(�θ�) �ε��� ����ġ >= i���(�ڽİ�) �ε��� ����ġ �̸� ����
		h[k] = h[i];		// i��� (�ڽİ�)�� ���� �ø��� 
		k = i;				// �ε����� �Ǵ� k��带 �Ʒ��� ������.
	}
	h[k] = v;			// k��ġ�� v(root��)�� ��ġ�̴�.
}

int pq_extract(int h[]) {		// root �ε��� ����
	int v = h[1];		// v = root�ε���
	h[1] = h[nheap--];		// �� ������ root�� �̵�
	downheap(h, 1);
	return v;
}

void adjust_heap(int h[], int n) {
	int k;
	for (k = n / 2; k >= 1; k--) downheap(h, k);	// ��� internal node�� ���Ͽ� downheap�� �Ͽ� �ٽñ� ����
}

void print_heap(int h[]) {		// �� �ε����� ���� node, weight�� �ε��� ���ʴ�� ���
	int i;
	printf("\n");
	for (i = 1; i <= nheap; i++)
		printf("%c:%d ", int2name(h[i]), check[h[i]]);	// ������ �ε����� �ش��ϴ� node, ����ġ���� ���
}

int pq_update(int h[], int v, int p) {		// node�� ����, ����ġ���� �����ϴ� �Լ�
	if (check[v] == UNSEEN) {		// v�� ���� �湮���� ���� �����
		h[++nheap] = v;				// ���� �ǳ��� v ����
		check[v] = p;				// ����ġ�� p���� �־��� (���۳��� 0, ������ ���� ����ġ�� ������)
		upheap(h, nheap);
		return -1;
	}
	else {
		if (check[v] < p) {			// v����� �ٸ� ������ ������ ��κ��� ����ġ���� ũ�ٸ� (cost�� ���ٸ�) 
			check[v] = p;			// ����ġ�� ����
			adjust_heap(h, nheap);
			return 1;
		}
		else return 0;				// v����� �ٸ� ������ ���� ��κ��� ȿ�������� ����
	}
}

void shortest_adjlist(node* g[], int start, int V) {		// start node -> �ٸ� node �� ���� �ִܰ�����
	int i;
	node* t;
	pq_init();
	for (i = 0; i < V; i++) {
		check[i] = UNSEEN;			// ����ġ�ʱ�ȭ
		parent[i] = 0;				// �θ��� �ʱ�ȭ
	}
	i = start;		// i��� = ���۳��
	if (check[i] == UNSEEN) {
		parent[i] = -1;				// ���۳�� �θ�� -1�� ����
		pq_update(heap, i, 0);		// ���۳��� ����ġ 0���� ����
		while (!pq_empty()) {		// ���� �������� �ݺ�
			print_heap(heap);		// ���� ������ ������ �ε����� �ϴ� ���� ����ġ�� ���
			i = pq_extract(heap);			// �� ���� �ε����� ������.
			check[i] = -check[i];		// ������ �� �ε������� ���� �湮�� ����̹Ƿ� ����ġ���� ���ȭ�Ѵ�.
			visit(i);			// i��� �湮
			for (t = g[i]; t != NULL; t = t->next) {	// i����� �ֺ� ���� Ž��
				if (check[t->vertex] < 0) {				// ���� �湮���� ���� �ֺ� ����� ���
					if (pq_update(heap, t->vertex, -t->weight - check[i]))	// �ش� ����� ����ġ�� + i�������� ����ġ�� �� ������ ������Ʈ
						parent[t->vertex] = i;			// ����尡 ������Ʈ�� �Ǿ��ٸ� �ش����� �θ�� i����̴�.
				}
			}
		}
	}
}

void print_tree(int p[], int V) {	// ��� �� �θ� - �ڽ� ���踦 �����ִ� ǥ
	int i;
	printf("\n\nTree structure \nson :    ");
	for (i = 0; i < V; i++)
		printf("%c ", int2name(i));			// �ڽ�

	printf("\n----------------------------------------\nparent :  ");
	for (i = 0; i < V; i++) {
		printf("%c ", int2name(parent[i]));		// �θ�
	}
}

void main() {
	int V, E;

	fp = fopen("PFS_shortest_path.txt", "rt");

	input_adjlist(G, &V, &E);
	printf("\nOriginal graph\n");
	print_adjlist(G, V);

	printf("\n\nshortest path cost");
	shortest_adjlist(G, name2int('A'), V);

	printf("\n\nshortest path cost A->K is %d\n", check[name2int('K')]);

	print_tree(parent, V);
	fclose(fp);
}