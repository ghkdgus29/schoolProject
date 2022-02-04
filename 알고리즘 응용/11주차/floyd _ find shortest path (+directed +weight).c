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

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// ����ġ�� ���⼺�� �����ϴ� adj matrix �׸���
	char vertex[3];
	int weight;
	int i, j, k;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {
			a[i][j] = 1000;							// ���ѿ� �ش��ϴ� �� (���� �հŸ�)
		}
	}

	for (i = 0; i < *V; i++) a[i][i] = 0;			// �ڱ��ڽ��� ����ġ�� 0�̴�.
	for (k = 0; k < *E; k++) {
		fscanf(fp, "%s %d", vertex, &weight);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = weight;
	}
}

void print_directed_adjmatrix(int a[][MAX_NODE], int V) {			// adj matrix ����ϱ�
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

void floyd(int d[][MAX_NODE], int V) {							// floyed warshall �Լ�, �� ������ �ּҺ���� ���
	int x, y, k;
	for (y = 0; y < V; y++) {
		for (x = 0; x < V; x++) {
			for (k = 0; k < V; k++) {
				if (d[x][y] + d[y][k] < d[x][k]) d[x][k] = d[x][y] + d[y][k];		// ���� ������ ��κ��� ������ �������, ����ġ�� ���������� ������Ʈ �Ѵ�.
			}
		}
	}
}

void main() {
	fp = fopen("floyd.txt", "rt");

	int V, E;

	input_directed_adjmatrix(GM, &V, &E);			// ���Ϸ� ���� ���� �׷��� ���
	printf("\n\nOriginal graph\n");
	print_directed_adjmatrix(GM, V);

	printf("\n\n\nshortest path graph\n");			// �� ������ �ּҺ�� ���
	floyd(GM, V);										// floyd warshall �Լ�
	print_directed_adjmatrix(GM, V);

	fclose(fp);
}