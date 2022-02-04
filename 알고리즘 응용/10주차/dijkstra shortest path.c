#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int G[MAX_NODE][MAX_NODE];		// 그래프
int distance[MAX_NODE];			// 거리를 저장할 배열
int parent[MAX_NODE];			// 부모
int check[MAX_NODE];			// 방문여부
FILE* fp;

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E) {
	char vertex[3];
	int i, j, k, w;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {
			a[i][j] = 1000;			// 엄청 큰수를 넣어줌으로써 직접 연결되지 않았음을 표현, 초기화
		}
	}

	for (i = 0; i < *V; i++) a[i][i] = 0;			// 자기자신의 가중치는 0
	for (k = 0; k < *E; k++) {
		fscanf(fp, "%s %d", vertex, &w);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = w;
		a[j][i] = w;			// 파일에서 읽어온 가중치값 할당
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {		// 그래프 출력함수
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%5c", int2name(i));

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%5c", int2name(i));
		for (j = 0; j < V; j++) printf("%5d", a[i][j]);
		printf("\n");
	}
}

void print_distance(int d[], int start, int V) {		// start 노드에서 각 노드들까지 가는 최소비용을 출력하는 함수
	int i;
	for (i = 0; i < V; i++) printf("%5d", d[i]);
	printf("\n");
}

void dijkstra(int a[][MAX_NODE], int start, int V) {		// start노드에서 각 노드들까지 가는 최소비용을 구하는 함수
	int x = 0, y, d;
	int i, checked = 0;		// checked 는 방문한 노드개수를 의미
	for (x = 0; x < V; x++) {
		distance[x] = a[start][x];		// distance 배열에 시작노드부터 모든 노드들까지 가는 비용들 저장
		if (x != start) parent[x] = start;		// start노드를 제외하고 모든 노드들의 부모는 start노드가 된다.
	}

	check[start] = 1;		// start노드 방문함을 체크
	checked++;				// 방문한 노드개수 증가

	print_distance(distance, start, V);

	while (checked < V) {		// 모든 노드들을 방문할 때 까지 반복
		x = 0;			// 기준이 되는 x노드 초기화
		while (check[x]) x++;		// 만약 x노드가 방문한 노드라면 방문하지 않은 노드가 나올때까지 x증가
		for (i = x; i < V; i++) {		// 모든 노드들에 대하여 조사 (i노드)
			if (check[i] == 0 && distance[i] < distance[x]) x = i;		// i노드가 방문x , 기준노드(처음엔 start)에서 거리가 가장 가까운 i노드값을 x노드로 설정
		}
		check[x] = 1;			// 기준노드로부터 가장가까운 x노드를 방문
		checked++;				// 방문노드개수 증가

		for (y = 0; y < V; y++) {	// x노드를 제외한 모든 노드들에 대해 검사
			if (x == y || a[x][y] >= 1000 || check[y]) continue;	// y노드가 x노드거나, 거리가 무한히 멀거나, 이미 방문한 노드라면 skip
			d = distance[x] + a[x][y];			// 간선 d 비용 =  x노드까지의 비용 + x노드에서 y노드까지의 비용
			if (d < distance[y]) {			// 간선 < y노드까지의 비용,,  y노드까지의 최단경로 재발견을 의미
				distance[y] = d;			// y노드까지의 비용을 간선으로 교체
				parent[y] = x;				// 부모를 x노드로 갱신
			}
		}
		print_distance(distance, x, V);
	}
}

void print_parent(int V) {			// 부모노드 출력
	for (int i = 0; i < V; i++) printf("\n%c", int2name(parent[i]));
}

void main() {
	int V, E;

	fp = fopen("dijkstra_shortest_path.txt", "rt");

	input_adjmatrix(G, &V, &E);
	printf("\nOriginal graph\n");			// adj matrix 출력
	print_adjmatrix(G, V);

	printf("\n\n");
	dijkstra(G, name2int('F'), V);		// start 노드 F 로 부터 모든 노드들의 최단경로비용 구하기

	print_parent(V);					// 부모노드 출력

	fclose(fp);
}