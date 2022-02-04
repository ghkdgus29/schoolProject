#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;


int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}

int GM[MAX_NODE][MAX_NODE];										// adjmatrix 를 나타내기 위한 행렬

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// directed adjmatrix를 구현하는 함수이다.
	char vertex[3];													// AB, AC 와 같은 선분을 저장하는 배열
	int i, j, k;
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);											// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM 배열을 0으로 전부 초기화한다.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 0;							// 대각 성분을 모두 0으로 초기화한다.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		fscanf(fp, "%s", vertex);										// 선분의 정보를 vertex배열에 저장한다. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// 입력받은 선분 정보를 정수화한다.
		a[i][j] = 1;

	}
}

void print_directed_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix를 출력하는 함수이다.
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%3c", int2name(i));				// 행렬에서 열에 해당하는 꼭지점 성분들을 출력한다. (맨위 가로줄)

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%3c", int2name(i));									// 행렬에서 행에 해당하는 꼭지점 성분들을 출력한다. (왼쪽 세로줄)
		for (j = 0; j < V; j++) printf("%3d", a[i][j]);				// 행렬 성분들을 해당하는 0과 1로 나타낸다.
		printf("\n");
	}
}

void warshall(int a[][MAX_NODE], int V) {		// 그래프간 reachability 를 찾는 함수
	int x, y, k;

	for (y = 0; y < V; y++) {		// 모든 노드에 대해 반복
		for (x = 0; x < V; x++) {		// 모든 노드에 대해 반복
			if (a[x][y] == 1) {				// x y 노드가 연결되어있고
				for (k = 0; k < V; k++) {		// 모든 k노드에 대해 조사 
					if (a[y][k] == 1) a[x][k] = 1;		// y k 노드가 연결되어있다면, x k 노드는 도달 가능하다.
				}
			}
		}
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");
	int V, E;

	input_directed_adjmatrix(GM, &V, &E);			// 그래프 만들기

	printf("\n\nOriginal graph\n");
	print_directed_adjmatrix(GM, V);				// 그래프 출력

	printf("\n\n\nReachability graph\n");
	warshall(GM, V);								// reachability 조사
	print_directed_adjmatrix(GM, V);

	fclose(fp);
}