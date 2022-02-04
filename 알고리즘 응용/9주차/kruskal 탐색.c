#include <stdio.h>

#define MAX_NODE 50
#define MAX_EDGE 100
#define ONLY_FIND 0
#define UNION 1

int parent[MAX_NODE];				// �θ��� ����
int cost = 0;						// Ž�� ���
int nheap = 0;						// ���� ũ��
int heap[MAX_NODE];					// ��, Edge �迭�� �ε��������� ����Ǿ��ִ�.
FILE* fp;

int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}


typedef struct edge {					// edge �� ��Ÿ���� ����ü�� �� ���� ����ġ�� ����
	int v1, v2;
	int weight;
}edge;

edge Edge[MAX_EDGE];					// edge ����ü�� �����ϴ� Edge�迭

void find_init(int elem) {				// ����� �θ� �ʱ�ȭ
	int i;
	for (i = 0; i < elem; i++) parent[i] = -1;		// ��� -1�� �ʱ�ȭ�Ѵ� --> find_set���� ����ϱ� ����
}

void union_set(int elem, int asso) {		// elem ����� �θ��带 asso �� �����ϴ� �Լ�
	parent[elem] = asso;
}

int find_set(int elem, int asso, int flag) {				// �Է¹��� edge, �� �� ��带 �����Ͽ� Ž����ΰ� circle�� �̷�� 0�� ��ȯ�ϰ�, circle�� �̷��� ������ 1�� ��ȯ�Ѵ�.
	int i = elem, j = asso;
	while (parent[i] >= 0) i = parent[i];				// elem ����� ���� ���� �θ��带 i�� ���� (-1�� ���ö�����)
	while (parent[j] >= 0) j = parent[j];				// asso ����� ���� ���� �θ��带 j�� ���� (-1�� ���ö�����)
	if (flag == UNION && i != j) union_set(i, j);		// i���� j��尡 �ٸ��ٸ� union_set�� ���� parent[i] = j�� �����ش�.
	return (i != j);								// ���� i, j�� �ٸ� ����� 1,   i,j�� ���� ����� 0�� ��ȯ�Ѵ�.
}

void input_edge(edge e[], int* V, int* E) {					// �׷��� ������ ���Ϸκ��� �о���̱� ���� �Լ�, ����ġ, �� ��带 Edge�迭�� ����
	char vertex[3];
	int i, j, w;
	printf("\nInput number of nodes and edges\n");
	fscanf(fp, "%d %d", V, E);
	for (j = 0; j < *E; j++) {
		printf("\nInput two nodes consisting of edge and its weight ->");
		fscanf(fp, "%s %d", vertex, &w);

		vertex[2] = NULL;					// vertex[2]�� ������� ����

		e[j].v1 = name2int(vertex[0]);			// edge �迭 ����ü�� ù��° ���(���1)�� vertex[0] ����
		e[j].v2 = name2int(vertex[1]);			// edge �迭 ����ü�� �ι�° ���(���2)�� vertex[1] ����
		e[j].weight = w;						// edge �迭 ����ü�� ����° ���, ����ġ�� w�� ����
	}
}

void upheap(int h[], int k) {										// �������� ���ο� ��带 �� �Ʒ��� �־� ���� �����Ѵ�. �̶� ������ edge�迭�� �ε������̴�. k�� ���� �����̴�.(�ǳ�)
	int v;
	v = h[k];				// ���� �ǳ����� v�� ����
	while (Edge[h[k / 2]].weight >= Edge[v].weight && k / 2 > 0) {		// �θ� �ε������� ����ġ >= ���� �ε������� ����ġ , �θ� �ε����� 0�� �ƴϸ� �ݺ�
		h[k] = h[k / 2];				// �θ��� �Ʒ��� ������.
		k /= 2;							// ������ �Ǵ� k�� ���� �ø���
	}
	h[k] = v;					// ���� �������� �� k���� ���� �ǳ���, v�� �ִ´�.
}

void downheap(int h[], int k) {										// ���������� ù ��° ������ �����ϰ� ���� �ٽ� ������ ���� �Լ���  (���� k�� 1)
	int i, v;														// ���� ���� ��带 �Ѹ��� �Űܿ� �ٽ� ����ġ�� ���� �����Ѵ�.
	v = h[k];			// ���� �Ѹ����� v�� ����
	while (k <= nheap / 2) {		// internal node�� ���� ����
		i = k << 1;				// i = 2*k , �� i ���� �ڽĳ��
		if (i < nheap && Edge[h[i]].weight > Edge[h[i + 1]].weight) i++;	// ���� ������ ���� ����ġ�� �� �����ε������� i�� ����
		if (Edge[v].weight <= Edge[h[i]].weight) break;						// �ڽĳ��(i) �ε������� ����ġ >  �θ���(k) �ε������� ����ġ  �̸� ���� ����
		h[k] = h[i];														// �ƴ϶�� �ڽĳ��(i)�� ���� �ø�
		k = i;														// ������ �Ǵ� k�� �Ʒ��� ������
	}
	h[k] = v;				// ���� �������� �� k���� ���� �Ѹ���, v�� �ִ´�.
}

int pq_empty() {							// ���� ������� �˻��ϴ� �Լ��� ����� ��� 1, �ƴҰ�� 0�� ����
	return (nheap == 0);
}

void pq_insert(int h[], int v) {			// ���� ���� ����ֱ� ���� �Լ�, �Ʒ��� ���� ����ִ´�.
	h[++nheap] = v;				// ���� �� ���� �ε������� �������
	upheap(h, nheap);			// �Ʒ� -> �� ���� ����
}

int pq_extract(int h[]) {					// ���� ���� �����ϱ� ���� �Լ�, ������ ���� ������.
	int v = h[1];				// v�� ���� �Ѹ����� ����
	h[1] = h[nheap--];			// ���� �Ѹ����� ���� �� ������ ����
	downheap(h, 1);				// �� -> �Ʒ� ���� ����
	return v;					// ������ ���� ��ȯ
}

void pq_init() {								// ���� �ʱ�ȭ��Ű�� ���� �Լ�
	nheap = 0;
}

void visit(int e) {								// ����Լ�, Edge�迭 �� edge����ü�� �� ��带 ����ϰ� cost���� �����Ѵ�.
	printf("%c%c ", int2name(Edge[e].v1), int2name(Edge[e].v2));
	cost += Edge[e].weight;				// �湮�� edge�� ������ ��� cost�� ���Ѵ�.
}


void kruskal(edge e[], int V, int E) {			// kruskal Ž���� �ϱ� ���� �Լ�,	e�� Edge �迭�� �ǹ��Ѵ�.
	int n, val = 0;
	find_init(V);				// ��� ��� �θ� -1�� �ʱ�ȭ
	pq_init();					// �� �ʱ�ȭ
	for (n = 0; n < E; n++) pq_insert(heap, n);			// edge ������ŭ ���� �ε����� �ְ� ����

	n = 0;						// n�� �湮�� edge������ �ǹ�, 0���� �ʱ�ȭ
	while (!pq_empty()) {				// heap �� �� ������ �ݺ�
		val = pq_extract(heap);			// val�� heap�� �Ѹ��� ���� ( �ε����� ���� ����ġ��(���)�� ���� ����edge)
		if (find_set(e[val].v1, e[val].v2, UNION)) {		// circle ���� �˻� , circle�̸� 0�� ��ȯ�ؼ� �湮����
			visit(val);
			n++;				// �湮������ �湮�� edge���� ����
		}
		if (n == V - 1) break;		// ��� ��� ���� -1 ��ŭ�� edge�� ��� ������ �湮�Ͽ����� �ǹ��Ѵ�.
	}
}

void main() {
	int V, E;

	fp = fopen("kruskal.txt", "rt");

	input_edge(Edge, &V, &E);									// �׷��� ����
	printf("\n\nVisited edge of minimum spanning tree\n");
	kruskal(Edge, V, E);										// kruskal , �ּҺ�� ��ε��� ���
	printf("\n\nMinimum cost is \n %d\n", cost);				// �ּҺ���� ���
	fclose(fp);
}