#include <stdio.h>

#define MAX_NODE 50
#define MAX_EDGE 100
#define ONLY_FIND 0
#define UNION 1

int parent[MAX_NODE];				// 부모노드 저장
int cost = 0;						// 탐색 비용
int nheap = 0;						// 힙의 크기
int heap[MAX_NODE];					// 힙, Edge 배열의 인덱스값들이 저장되어있다.
FILE* fp;

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}


typedef struct edge {					// edge 를 나타내는 구조체로 두 노드와 가중치를 저장
	int v1, v2;
	int weight;
}edge;

edge Edge[MAX_EDGE];					// edge 구조체를 저장하는 Edge배열

void find_init(int elem) {				// 노드의 부모 초기화
	int i;
	for (i = 0; i < elem; i++) parent[i] = -1;		// 모두 -1로 초기화한다 --> find_set에서 사용하기 위함
}

void union_set(int elem, int asso) {		// elem 노드의 부모노드를 asso 로 저장하는 함수
	parent[elem] = asso;
}

int find_set(int elem, int asso, int flag) {				// 입력받은 edge, 즉 두 노드를 조사하여 탐색경로가 circle을 이루면 0을 반환하고, circle을 이루지 않으면 1을 반환한다.
	int i = elem, j = asso;
	while (parent[i] >= 0) i = parent[i];				// elem 노드의 가장 상위 부모노드를 i에 저장 (-1이 나올때까지)
	while (parent[j] >= 0) j = parent[j];				// asso 노드의 가장 상위 부모노드를 j에 저장 (-1이 나올때까지)
	if (flag == UNION && i != j) union_set(i, j);		// i노드와 j노드가 다르다면 union_set을 통해 parent[i] = j로 묶어준다.
	return (i != j);								// 만약 i, j가 다른 노드라면 1,   i,j가 같은 노드라면 0을 반환한다.
}

void input_edge(edge e[], int* V, int* E) {					// 그래프 정보를 파일로부터 읽어들이기 위한 함수, 가중치, 두 노드를 Edge배열에 저장
	char vertex[3];
	int i, j, w;
	printf("\nInput number of nodes and edges\n");
	fscanf(fp, "%d %d", V, E);
	for (j = 0; j < *E; j++) {
		printf("\nInput two nodes consisting of edge and its weight ->");
		fscanf(fp, "%s %d", vertex, &w);

		vertex[2] = NULL;					// vertex[2]는 사용하지 않음

		e[j].v1 = name2int(vertex[0]);			// edge 배열 구조체의 첫번째 멤버(노드1)에 vertex[0] 저장
		e[j].v2 = name2int(vertex[1]);			// edge 배열 구조체의 두번째 멤버(노드2)에 vertex[1] 저장
		e[j].weight = w;						// edge 배열 구조체의 세번째 멤버, 가중치에 w값 저장
	}
}

void upheap(int h[], int k) {										// 힙공간에 새로운 노드를 맨 아래에 넣어 위로 정렬한다. 이때 노드들은 edge배열의 인덱스들이다. k는 힙의 길이이다.(맨끝)
	int v;
	v = h[k];				// 힙의 맨끝값을 v에 복사
	while (Edge[h[k / 2]].weight >= Edge[v].weight && k / 2 > 0) {		// 부모 인덱스값의 가중치 >= 현재 인덱스값의 가중치 , 부모 인덱스가 0이 아니면 반복
		h[k] = h[k / 2];				// 부모값을 아래로 내린다.
		k /= 2;							// 기준이 되는 k도 위로 올린다
	}
	h[k] = v;					// 전부 끝마쳤을 때 k값에 힙의 맨끝값, v를 넣는다.
}

void downheap(int h[], int k) {										// 힙공간에서 첫 번째 성분을 추출하고 나서 다시 정렬을 위한 함수로  (보통 k는 1)
	int i, v;														// 가장 낮은 노드를 뿌리로 옮겨와 다시 가중치를 비교해 정렬한다.
	v = h[k];			// 힙의 뿌리값을 v에 복사
	while (k <= nheap / 2) {		// internal node에 대해 조사
		i = k << 1;				// i = 2*k , 즉 i 노드는 자식노드
		if (i < nheap && Edge[h[i]].weight > Edge[h[i + 1]].weight) i++;	// 왼쪽 오른쪽 둘중 가중치가 더 작은인덱스값을 i로 설정
		if (Edge[v].weight <= Edge[h[i]].weight) break;						// 자식노드(i) 인덱스값의 가중치 >  부모노드(k) 인덱스값의 가중치  이면 정렬 종료
		h[k] = h[i];														// 아니라면 자식노드(i)를 위로 올림
		k = i;														// 기준이 되는 k는 아래로 내린다
	}
	h[k] = v;				// 전부 끝마쳤을 때 k값에 힙의 뿌리값, v를 넣는다.
}

int pq_empty() {							// 힙이 비었는지 검사하는 함수로 비었을 경우 1, 아닐경우 0을 리턴
	return (nheap == 0);
}

void pq_insert(int h[], int v) {			// 힙에 값을 집어넣기 위한 함수, 아래서 부터 집어넣는다.
	h[++nheap] = v;				// 힙의 맨 끝에 인덱스값을 집어넣음
	upheap(h, nheap);			// 아래 -> 위 정렬 수행
}

int pq_extract(int h[]) {					// 힙에 값을 추출하기 위한 함수, 위에서 부터 빼낸다.
	int v = h[1];				// v에 힙의 뿌리값을 추출
	h[1] = h[nheap--];			// 힙의 뿌리값에 힙의 맨 끝값을 저장
	downheap(h, 1);				// 위 -> 아래 정렬 수행
	return v;					// 추출한 값은 반환
}

void pq_init() {								// 힙을 초기화시키기 위한 함수
	nheap = 0;
}

void visit(int e) {								// 출력함수, Edge배열 속 edge구조체의 두 노드를 출력하고 cost값을 저장한다.
	printf("%c%c ", int2name(Edge[e].v1), int2name(Edge[e].v2));
	cost += Edge[e].weight;				// 방문한 edge의 비용들을 모두 cost에 더한다.
}


void kruskal(edge e[], int V, int E) {			// kruskal 탐색을 하기 위한 함수,	e는 Edge 배열을 의미한다.
	int n, val = 0;
	find_init(V);				// 모든 노드 부모 -1로 초기화
	pq_init();					// 힙 초기화
	for (n = 0; n < E; n++) pq_insert(heap, n);			// edge 개수만큼 힙에 인덱스들 넣고 정렬

	n = 0;						// n은 방문한 edge개수를 의미, 0으로 초기화
	while (!pq_empty()) {				// heap 이 빌 때까지 반복
		val = pq_extract(heap);			// val에 heap의 뿌리값 추출 ( 인덱스를 통한 가중치값(비용)이 가장 작은edge)
		if (find_set(e[val].v1, e[val].v2, UNION)) {		// circle 인지 검사 , circle이면 0을 반환해서 방문안함
			visit(val);
			n++;				// 방문했으면 방문한 edge개수 증가
		}
		if (n == V - 1) break;		// 모든 노드 개수 -1 만큼의 edge는 모든 노드들을 방문하였음을 의미한다.
	}
}

void main() {
	int V, E;

	fp = fopen("kruskal.txt", "rt");

	input_edge(Edge, &V, &E);									// 그래프 형성
	printf("\n\nVisited edge of minimum spanning tree\n");
	kruskal(Edge, V, E);										// kruskal , 최소비용 경로들을 출력
	printf("\n\nMinimum cost is \n %d\n", cost);				// 최소비용을 출력
	fclose(fp);
}