#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int G[MAX_NODE][MAX_NODE];		// �׷���
int distance[MAX_NODE];			// �Ÿ��� ������ �迭
int parent[MAX_NODE];			// �θ�
int check[MAX_NODE];			// �湮����
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
			a[i][j] = 1000;			// ��û ū���� �־������ν� ���� ������� �ʾ����� ǥ��, �ʱ�ȭ
		}
	}

	for (i = 0; i < *V; i++) a[i][i] = 0;			// �ڱ��ڽ��� ����ġ�� 0
	for (k = 0; k < *E; k++) {
		fscanf(fp, "%s %d", vertex, &w);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = w;
		a[j][i] = w;			// ���Ͽ��� �о�� ����ġ�� �Ҵ�
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {		// �׷��� ����Լ�
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

void print_distance(int d[], int start, int V) {		// start ��忡�� �� ������� ���� �ּҺ���� ����ϴ� �Լ�
	int i;
	for (i = 0; i < V; i++) printf("%5d", d[i]);
	printf("\n");
}

void dijkstra(int a[][MAX_NODE], int start, int V) {		// start��忡�� �� ������� ���� �ּҺ���� ���ϴ� �Լ�
	int x = 0, y, d;
	int i, checked = 0;		// checked �� �湮�� ��尳���� �ǹ�
	for (x = 0; x < V; x++) {
		distance[x] = a[start][x];		// distance �迭�� ���۳����� ��� ������� ���� ���� ����
		if (x != start) parent[x] = start;		// start��带 �����ϰ� ��� ������ �θ�� start��尡 �ȴ�.
	}

	check[start] = 1;		// start��� �湮���� üũ
	checked++;				// �湮�� ��尳�� ����

	print_distance(distance, start, V);

	while (checked < V) {		// ��� ������ �湮�� �� ���� �ݺ�
		x = 0;			// ������ �Ǵ� x��� �ʱ�ȭ
		while (check[x]) x++;		// ���� x��尡 �湮�� ����� �湮���� ���� ��尡 ���ö����� x����
		for (i = x; i < V; i++) {		// ��� ���鿡 ���Ͽ� ���� (i���)
			if (check[i] == 0 && distance[i] < distance[x]) x = i;		// i��尡 �湮x , ���س��(ó���� start)���� �Ÿ��� ���� ����� i��尪�� x���� ����
		}
		check[x] = 1;			// ���س��κ��� ���尡��� x��带 �湮
		checked++;				// �湮��尳�� ����

		for (y = 0; y < V; y++) {	// x��带 ������ ��� ���鿡 ���� �˻�
			if (x == y || a[x][y] >= 1000 || check[y]) continue;	// y��尡 x���ų�, �Ÿ��� ������ �ְų�, �̹� �湮�� ����� skip
			d = distance[x] + a[x][y];			// ���� d ��� =  x�������� ��� + x��忡�� y�������� ���
			if (d < distance[y]) {			// ���� < y�������� ���,,  y�������� �ִܰ�� ��߰��� �ǹ�
				distance[y] = d;			// y�������� ����� �������� ��ü
				parent[y] = x;				// �θ� x���� ����
			}
		}
		print_distance(distance, x, V);
	}
}

void print_parent(int V) {			// �θ��� ���
	for (int i = 0; i < V; i++) printf("\n%c", int2name(parent[i]));
}

void main() {
	int V, E;

	fp = fopen("dijkstra_shortest_path.txt", "rt");

	input_adjmatrix(G, &V, &E);
	printf("\nOriginal graph\n");			// adj matrix ���
	print_adjmatrix(G, V);

	printf("\n\n");
	dijkstra(G, name2int('F'), V);		// start ��� F �� ���� ��� ������ �ִܰ�κ�� ���ϱ�

	print_parent(V);					// �θ��� ���

	fclose(fp);
}