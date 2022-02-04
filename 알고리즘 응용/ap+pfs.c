#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100				// 힙의 최대크기를 나타낸다.
#define UNSEEN (-INT_MAX)			// 접근하지 않은 노드를 나타낸다.

typedef struct node {				// 그래프를 그리기 위한 노드
	int vertex;
	int weight;
	struct node* next;
}node;

node* G[MAX_NODE];					// 그래프를 위한 배열, 그래프를 adj list 형태로 저장
int check[MAX_NODE];				// 그래프 선분의 가중치를 저장하기 위한 배열
int parent[MAX_NODE];				// 그래프 노드의 부모를 저장하기 위한 배열
FILE* fp;
int nheap = 0;						// 힙공간의 크기
int heap[MAX_NODE];					// 힙공간 생성

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}

void input_adjlist(node* g[], int* V, int* E) {		// 파일로부터 노드의 개수와, 선분 개수, 가중치를 입력받아
	char vertex[3];									// 그래프를 형성하는 함수
	int i, j, w;
	node* t;
	printf("Input number of nodes and edges  -->");
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) g[i] = NULL;
	printf("\nInput two nodes consisting of edges and its weight -->");
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

void print_adjlist(node* g[], int V) {								// 형성된 그래프를 출력하는 함수, 노드와 가중치를 출력한다.
	int i;
	node* t;
	for (i = 0; i < V; i++) {
		printf("\n%c : ", int2name(i));
		for (t = g[i]; t != NULL; t = t->next) {
			printf("-> %c:%d ", int2name(t->vertex), t->weight);
		}
	}
}

void print_heap(int h[]) {											// 힙공간에 있는 노드와 그 노드에 해당하는 가중치를 출력하는 함수
	int i;
	printf("\n");
	for (i = 1; i <= nheap; i++) printf("%c:%d ", int2name(h[i]), check[h[i]]);
}

int visit(int k) {													// 노드의 방문을 나타내는 함수
	printf("--> %c", int2name(k));
}

void upheap(int h[], int k) {										// 힙공간에 새로운 노드를 집어넣어 정렬한다. 선분의 cost(가중치)가 작은 값이 위로 올라간다.
	int v;
	v = h[k];
	while (check[h[k / 2]] <= check[v] && k / 2 > 0) {
		h[k] = h[k / 2];
		k /= 2;
	}
	h[k] = v;
}

void downheap(int h[], int k) {										// 힙공간에서 k번째 성분을 추출하고 나서 다시 정렬을 위한 함수로 
	int i, v;														// 가장 낮은 노드를 뿌리로 옮겨와 다시 가중치를 비교해 정렬한다.
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

int pq_empty() {							// 힙이 비었는지 검사하는 함수로 비었을 경우 1, 아닐경우 0을 리턴
	if (nheap == 0) return 1;
	return 0;
}

int pq_extract(int h[]) {					// 힙에서 뿌리에 해당하는 성분을 추출하고 , 다시정렬하는 함수
	int v = h[1];
	h[1] = h[nheap--];
	downheap(h, 1);
	return v;
}

void adjust_heap(int h[], int n) {						// 힙성분들을 다시 정렬하기 위한 함수
	int k;
	for (k = n / 2; k >= 1; k--) downheap(h, k);
}

int pq_update(int h[], int v, int p) {				// 힙에 노드들을 집어넣을때 노드가 처음집어넣는 노드이면 if(check[v] == UNSEEN)문을
	if (check[v] == UNSEEN) {						// 수행하여 입력받은 cost 값을 v번째 힙공간에 집어넣고 정렬
		h[++nheap] = v;
		check[v] = p;
		upheap(h, nheap);
		return 1;
	}
	else {											// 만약 한번 이상 집어넣었던 노드일경우, cost값의 비교를 수행하여 		
		if (check[v] < p) {							// cost가 작으면 갱신, cost가 크면 그대로 둔다.
			check[v] = p;
			adjust_heap(h, nheap);
			return 1;
		}
		else return 0;
	}
}

void pq_init() {								// 힙을 초기화시키기 위한 함수
	nheap = 0;
}

void PFS_adjlist(node* g[], int V) {			// 위에 서술한 함수들을 모두 포함한 함수로 PFS탐색을 수행하는 함수이다.
	int i;										// 수행중에 parent 와 check 배열을 채워넣으며 parent 노드와 cost를 기록한다.
	node* t;									// 또한 visit함수를 통해 방문한 노드를 출력함으로써 탐색순서를 확인할 수 있다.
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

void print_tree(int p[], int V) {							// 노드들의 부모를 출력하는 함수.
	int i;
	printf("\n\nTree structure\nson :    ");
	for (i = 0; i < V; i++) {
		printf("%c ", int2name(i));
	}
	printf("\n------------------------------------\nparent :   ");
	for (i = 1; i < V; i++) {
		printf("%c ", int2name(parent[i]));
	}
}

void print_cost(int c[], int V) {							// 노드들의 가중치(cost)를 모두 더한 값으로 경로를 탐색하는 
	int i, sum = 0;											// 최소비용을 출력한다.
	for (i = 1; i < V; i++) sum += check[i];
	printf("%d\n", sum);
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
	printf("\n\n단절점 찾기\n");

	for (i = 0; i < V; i++) check[i] = 0;
	order = son_of_root = 0;
	AP_recur(a, 0);

	if (son_of_root > 1) printf("*");
	else printf(" ");
	printf("%c son : %d\n", int2name(0), son_of_root);
}

void main() {
	int V, E;
	fp = fopen("graph.txt", "rt");							// 그래프의 대한 정보가 담겨있는 텍스트파일

	input_adjlist(G, &V, &E);
	printf("\nOrigianl graph\n");
	print_adjlist(G, V);									// 그래프 출력

	AP_search(G, V);

	printf("\nVisit order of Minimum Spanning Tree\n");
	PFS_adjlist(G, V);										// PFS 탐색을 통해 얻은 spanning Tree  출력
	print_tree(parent, V);									// 각 노드들의 부모노드 출력	
	printf("\n\nMinimum Cost is ");
	print_cost(check, V);									// 탐색의 들어간 비용 출력

	for (int i = 0; i < V; i++) free(G[i]);

	fclose(fp);
}