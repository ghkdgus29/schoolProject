#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

FILE* fp;
#define MAX_NODE 100
#define SOURCE 'S'
#define SINK 'T'
#define NODE 6

int Capacity[MAX_NODE][MAX_NODE];		// 용량을 나타내는 matrix
int Flow[MAX_NODE][MAX_NODE];			// 유량을 나타내는 matrix
int Residual[MAX_NODE][MAX_NODE];		// 잔차를 나타내는 matrix (용량 - 유량)
int check[MAX_NODE];					// 방문을 나타내는 배열
int parent[MAX_NODE];					// 부모를 나타내는 배열
int path[MAX_NODE];						// augmenting path를 나타내는 배열

int name2int(char c) {
	if (c == SOURCE) return 0;
	if (c == SINK) return 1;
	return c - 'A' + 2;			// 'A' 노드는 2부터 시작
}

char int2name(int i) {
	if (i == 0) return SOURCE;
	if (i == 1) return SINK;
	return i + 'A' - 2;			// 2 는 'A'를 의미
}

int queue[MAX_NODE];			// queue를 사용하기 위한 배열
int front, rear;

void init_queue() {
	front = rear = 0;
}

int queue_empty() {
	if (front == rear) return 1;
	else return 0;
}

void put(int k) {
	queue[rear] = k;
	rear = ++rear % MAX_NODE;
}

int get() {
	int i;
	i = queue[front];
	front = ++front % MAX_NODE;
	return i;
}

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// 가중치와 방향성을 포함하는 adj matrix 그리기
	char vertex[3];
	int weight;
	int i, j, k;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {
			a[i][j] = 0;
		}
	}

	for (i = 0; i < *V; i++) a[i][i] = 0;			// 자기자신은 가중치가 0이다.
	for (k = 0; k < *E; k++) {
		fscanf(fp, "%s %d", vertex, &weight);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = weight;
	}
}

void print_directed_adjmatrix(int a[][MAX_NODE], int V) {			// adj matrix 출력하기
	int i, j;
	printf("    ");
	for (i = 0; i < V; i++) printf("%4c", int2name(i));
	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%4c", int2name(i));
		for (j = 0; j < V; j++) printf("%4d", a[i][j]);
		printf("\n");
	}
}

void set_path() {				// augmenting path 를 만드는 함수
	int* temp;
	int i, count = 0;
	temp = (int*)calloc(MAX_NODE, sizeof(int));		// temp 배열 생성
	i = name2int(SINK);			// SINK 노드부터 시작
	while (i >= 0) {
		temp[count] = i;
		i = parent[i];
		count++;				// temp 배열에 첫 값은 SINK, 이후 값은 부모들을 거슬러 올라가며 저장, 즉 augmenting path의 역순
	}
	for (i = 0; i < count; i++)
		path[i] = temp[count - i - 1];		// path에 augmenting path 의 역순을 다시 역으로 저장, 즉 augmenting path 저장
	path[i] = -1;				// 마지막엔 -1 저장 (-1 전까지가 노드 개수임, 즉 마침표를 의미)
	free(temp);
}

int get_augment_path(int a[][MAX_NODE], int V, int S, int T) {		// augmenting path 구하기 (BFS 수정)
	int i, j;
	init_queue();
	for (i = 0; i < V; i++) {
		check[i] = 0;				// 방문 초기화
		parent[i] = -1;				// 부모 초기화  (0이 Source이므로 -1로 초기화)
	}
	i = name2int(S);					// Source에서 시작
	if (check[i] == 0) {			// 만약 i노드를 방문하지 않았다면 
		put(i);				// i put
		check[i] = 1;				// i노드 방문 체크
		while (!queue_empty()) {				// queue에 뭐가 있으면 반복
			i = get();				// i로 꺼내오기
			if (i == name2int(T)) break;		// 만약 꺼내온게 Sink면 반복문 종료 (path 다 찾았음)
			for (j = 0; j < V; j++) {			// 모든 노드(j)에 대해 조사
				if (a[i][j] != 0) {				// 만약 j노드와 연결이 있으면
					if (check[j] == 0) {		// 또한 j노드와 방문하지 않았으면 
						put(j);					// j노드 put (방문)
						check[j] = 1;			// j노드 방문 체크
						parent[j] = i;			// i노드가 부모임을 입력
					}
				}
			}
		}
	}
	set_path();					// 위에서 구한 parent 배열을 통해 augmenting path를 찾는 함수
	if (i == name2int(T)) return 1;		// 반복문을 빠져나온 경우가 i노드가 SINK, 즉 경로를 찾아서 빠져나온 경우 1반환
	else return 0;						// queue가 비어서, 즉 더이상 경로가 없어서 빠져나온 경우 0반환
}

void construct_residual(int c[][MAX_NODE], int f[][MAX_NODE], int r[][MAX_NODE], int V) {		// residual network를 구하는 함수, V는 노드개수
	int i, j;
	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			r[i][j] = c[i][j] - f[i][j];		// residual network = capacity - flow
}

void clear_matrix(int m[][MAX_NODE], int V) {		// matrix 초기화 (0으로)
	int i, j;
	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			m[i][j] = 0;
}

void network_flow(int c[][MAX_NODE], int f[][MAX_NODE], int r[][MAX_NODE], int V, int S, int T) {			// flow , residual graph를 만드는 함수
	int i, min;
	clear_matrix(f, V);
	clear_matrix(r, V);					// flow, residual network 초기화
	construct_residual(c, f, r, V);		// 우선적으로 residual network 형성 (미완성)
	while (get_augment_path(r, V, S, T)) {		// 1이면 경로가 더 있으니까 반복문 반복, 0이면 경로가 없다는 뜻이니 반복문 탈출
		min = INT_MAX;					// 최소값 초기화 (가장 큰값으로)
		for (i = 1; path[i] >= 0; i++)		// path가 SOURCE부터 시작해 SINK에 다다르기 전까지 반복
			if (min > r[path[i - 1]][path[i]])
				min = r[path[i - 1]][path[i]];		// augment 경로 내 가장 용량이 작은값을 찾는다
		for (i = 1; path[i] >= 0; i++) {			// path가 SOURCE부터 시작해 SINK에 다다르기 전까지 반복
			f[path[i - 1]][path[i]] = f[path[i - 1]][path[i]] + min;	// 유량 = 이전 유량값 + min 용량값
			f[path[i]][path[i - 1]] = -f[path[i - 1]][path[i]];			// 유량의 역방향 = - 유량
		}
		construct_residual(c, f, r, V);				// 구한 flow, residual을 반복, augmenting path가 없을 때 까지
	}
}

// A DFS based function to find all reachable vertices from s.  The function
// marks visited[i] as true if i is reachable from s.  The initial values in
// visited[] must be false. We can also use BFS to find reachable vertices
void dfs(int rGraph[MAX_NODE][MAX_NODE], int s, int visited[])
{
	visited[s] = 1;
	for (int i = 0; i < NODE; i++)
		if (rGraph[s][i] && !visited[i])
		{
			dfs(rGraph, i, visited);
		}
}

// Prints the minimum s-t cut
void minCut(int graph[MAX_NODE][MAX_NODE], int rGraph[MAX_NODE][MAX_NODE], int s, int t)
{
	// Flow is maximum now, find vertices reachable from s
	int visited[NODE];
	for (int i = 0; i < NODE; i++)
		visited[i] = 0;
	dfs(rGraph, s, visited);

	// Print all edges that are from a reachable vertex to
	// non-reachable vertex in the original graph
	for (int i = 0; i < NODE; i++)
		for (int j = 0; j < NODE; j++)
			if (visited[i] && !visited[j] && graph[i][j])
				printf("\n %c - %c", int2name(i), int2name(j));
}

void main()
{
	int V, E;
	fp = fopen("capacity2.txt", "r");
	input_directed_adjmatrix(Capacity, &V, &E);

	network_flow(Capacity, Flow, Residual, V, SOURCE, SINK);

	printf("\nflow network\n");
	print_directed_adjmatrix(Flow, V);

	minCut(Capacity, Residual, 0, 5);
}