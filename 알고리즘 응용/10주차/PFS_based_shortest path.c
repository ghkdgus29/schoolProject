#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100
#define UNSEEN (-INT_MAX)

typedef struct node {
	int vertex;
	int weight;
	struct node* next;
}node;

node* G[MAX_NODE];		// 그래프 성분을 저장
int check[MAX_NODE];	// 방문 여부, 가중치값을 저장
int parent[MAX_NODE];	// 각 노드들의 부모노드 저장
FILE* fp;
int nheap = 0;			// 힙 길이
int heap[MAX_NODE];		// heap , 인덱스 값 저장

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

void visit(int k) {
	printf("--> %c", int2name(k));		// 노드 방문
}

void input_adjlist(node* g[], int* V, int* E) {		// file로부터 그래프 성분 추출 (adj list)
	char vertex[3];
	int i, j, w;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) g[i] = NULL;		// 모든 노드들 초기화
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

void print_adjlist(node* g[], int V) {		// 그래프 표현
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
	nheap = 0;			// heap 초기화
}

int pq_empty() {
	return (nheap == 0);
}

void upheap(int h[], int k) {		// k는 보통 힙의 길이 (맨끝)
	int v;
	v = h[k];		// v = 힙의 맨끝값
	while (check[h[k / 2]] <= check[v] && k / 2 > 0) {		// 부모 인덱스 가중치 <= v(자식)인덱스 가중치 , 부모 노드가 0이 아니면 반복	
		h[k] = h[k / 2];		// 부모노드를 끌어내린다
		k /= 2;					// 기준이 되는 k노드를 위로 올린다
	}
	h[k] = v;		// 모든 동작 마쳤을 때 k위치가 v(처음 힙의 맨끝값)의 위치이다.
}

void downheap(int h[], int k) {			// k는 보통 힙의 맨 처음 (뿌리)
	int i, v;
	v = h[k];
	while (k <= nheap / 2) {			// internal node에 대해 반복
		i = k << 1;		// i=2*k, i는 자식노드, k는 부모노드
		if (i < nheap && check[h[i]] < check[h[i + 1]]) i++;	// 자식노드중 인덱스 가중치가 큰 값이 i노드가 된다
		if (check[v] >= check[h[i]]) break;		// v노드(부모값) 인덱스 가중치 >= i노드(자식값) 인덱스 가중치 이면 중지
		h[k] = h[i];		// i노드 (자식값)를 위로 올린다 
		k = i;				// 인덱스가 되는 k노드를 아래로 내린다.
	}
	h[k] = v;			// k위치가 v(root값)의 위치이다.
}

int pq_extract(int h[]) {		// root 인덱스 추출
	int v = h[1];		// v = root인덱스
	h[1] = h[nheap--];		// 맨 끝값을 root로 이동
	downheap(h, 1);
	return v;
}

void adjust_heap(int h[], int n) {
	int k;
	for (k = n / 2; k >= 1; k--) downheap(h, k);	// 모든 internal node에 대하여 downheap을 하여 다시금 정렬
}

void print_heap(int h[]) {		// 힙 인덱스를 통한 node, weight를 인덱스 차례대로 출력
	int i;
	printf("\n");
	for (i = 1; i <= nheap; i++)
		printf("%c:%d ", int2name(h[i]), check[h[i]]);	// 힙안의 인덱스에 해당하는 node, 가중치값을 출력
}

int pq_update(int h[], int v, int p) {		// node의 상태, 가중치값을 갱신하는 함수
	if (check[v] == UNSEEN) {		// v가 만약 방문하지 않은 노드라면
		h[++nheap] = v;				// 힙의 맨끝에 v 넣음
		check[v] = p;				// 가중치는 p값을 넣어줌 (시작노드는 0, 나머지 노드는 가중치의 누적합)
		upheap(h, nheap);
		return -1;
	}
	else {
		if (check[v] < p) {			// v노드의 다른 간선이 이전의 경로보다 가중치값이 크다면 (cost가 적다면) 
			check[v] = p;			// 가중치값 갱신
			adjust_heap(h, nheap);
			return 1;
		}
		else return 0;				// v노드의 다른 간선이 이전 경로보다 효율적이지 않음
	}
}

void shortest_adjlist(node* g[], int start, int V) {		// start node -> 다른 node 로 가는 최단경로출력
	int i;
	node* t;
	pq_init();
	for (i = 0; i < V; i++) {
		check[i] = UNSEEN;			// 가중치초기화
		parent[i] = 0;				// 부모노드 초기화
	}
	i = start;		// i노드 = 시작노드
	if (check[i] == UNSEEN) {
		parent[i] = -1;				// 시작노드 부모는 -1로 설정
		pq_update(heap, i, 0);		// 시작노드는 가중치 0으로 설정
		while (!pq_empty()) {		// 힙이 빌때까지 반복
			print_heap(heap);		// 현재 힙안의 값들을 인덱스로 하는 노드와 가중치값 출력
			i = pq_extract(heap);			// 힙 안의 인덱스를 빼낸다.
			check[i] = -check[i];		// 힙에서 뺀 인덱스값의 노드는 방문한 노드이므로 가중치값을 양수화한다.
			visit(i);			// i노드 방문
			for (t = g[i]; t != NULL; t = t->next) {	// i노드의 주변 노드들 탐색
				if (check[t->vertex] < 0) {				// 아직 방문하지 않은 주변 노드일 경우
					if (pq_update(heap, t->vertex, -t->weight - check[i]))	// 해당 노드의 가중치값 + i노드까지의 가중치값 을 음수로 업데이트
						parent[t->vertex] = i;			// 헤당노드가 업데이트가 되었다면 해당노드의 부모는 i노드이다.
				}
			}
		}
	}
}

void print_tree(int p[], int V) {	// 노드 간 부모 - 자식 관계를 보여주는 표
	int i;
	printf("\n\nTree structure \nson :    ");
	for (i = 0; i < V; i++)
		printf("%c ", int2name(i));			// 자식

	printf("\n----------------------------------------\nparent :  ");
	for (i = 0; i < V; i++) {
		printf("%c ", int2name(parent[i]));		// 부모
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