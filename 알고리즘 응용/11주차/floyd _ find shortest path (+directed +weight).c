#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

int GM[MAX_NODE][MAX_NODE];

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// 가중치와 방향성을 포함하는 adj matrix 그리기
	char vertex[3];
	int weight;
	int i, j, k;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {
			a[i][j] = 1000;							// 무한에 해당하는 값 (아주 먼거리)
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
	printf("   ");
	for (i = 0; i < V; i++) printf("%5c", int2name(i));
	printf("\n");
	
	for (i = 0; i < V; i++) {
		printf("%5c", int2name(i));
		for (j = 0; j < V; j++) printf("%5d", a[i][j]);
		printf("\n");
	}
}

void floyd(int d[][MAX_NODE], int V) {							// floyed warshall 함수, 각 노드로의 최소비용을 출력
	int x, y, k;
	for (y = 0; y < V; y++) {
		for (x = 0; x < V; x++) {
			for (k = 0; k < V; k++) {
				if (d[x][y] + d[y][k] < d[x][k]) d[x][k] = d[x][y] + d[y][k];		// 만약 기존의 경로보다 간선이 작을경우, 가중치를 간선값으로 업데이트 한다.
			}
		}
	}
}

void main() {
	fp = fopen("floyd.txt", "rt");

	int V, E;

	input_directed_adjmatrix(GM, &V, &E);			// 파일로 부터 읽은 그래프 출력
	printf("\n\nOriginal graph\n");
	print_directed_adjmatrix(GM, V);

	printf("\n\n\nshortest path graph\n");			// 각 노드로의 최소비용 출력
	floyd(GM, V);										// floyd warshall 함수
	print_directed_adjmatrix(GM, V);

	fclose(fp);
}