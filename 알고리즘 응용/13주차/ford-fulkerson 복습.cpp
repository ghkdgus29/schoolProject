#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define SINK 'T'
#define SOURCE 'S'

FILE* fp;

int char2int(char c) {
	if (c == SOURCE) return 0;
	else if (c == SINK) return 1;
	else
		return c - 'A' + 2;
}

char int2char(int i) {
	if (i == 0) return SOURCE;
	else if (i == 1) return SINK;
	else
		return i + 'A' - 2;
}

void clear_matrix(int m[][MAX], int V) {
	int i, j;
	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			m[i][j] = 0;
}

void input_matrix(int a[][MAX], int* V, int* E) {
	int i, j, k, w;
	char vertex[3];
	fscanf(fp, "%d %d", V, E);
	clear_matrix(a, *V);
	
	for (i = 0; i < *E; i++) {
		fscanf(fp, "%s %d", vertex, &w);
		
		j = char2int(vertex[0]);
		k = char2int(vertex[1]);
		a[j][k] = w;
	}
}

void print_matrix(int c[][MAX], int f[][MAX], int r[][MAX], int V) {
	int i, j;

	printf("\n\ncapacity\n");
	printf("   ");
	for (i = 0; i < V; i++)
		printf(" %c  ", int2char(i));

	for (i = 0; i < V; i++) {
		printf("\n%c", int2char(i));
		for (j = 0; j < V; j++) {
			printf("%4d", c[i][j]);
		}
	}

	printf("\n\nflow\n");
	printf("   ");
	for (i = 0; i < V; i++)
		printf(" %c  ", int2char(i));

	for (i = 0; i < V; i++) {
		printf("\n%c", int2char(i));
		for (j = 0; j < V; j++) {
			printf("%4d", f[i][j]);
		}
	}

	printf("\n\nresidual\n");
	printf("   ");
	for (i = 0; i < V; i++)
		printf(" %c  ", int2char(i));

	for (i = 0; i < V; i++) {
		printf("\n%c", int2char(i));
		for (j = 0; j < V; j++) {
			printf("%4d", r[i][j]);
		}
	}
	printf("\n\n");
}

int front, rear;
int queue[MAX];
#define init_queue() front=rear=0
#define queue_empty() (front==rear)

int put(int k) {
	if (rear + 1 % MAX == front) {
		printf("ovflow!");
		return -1;
	}

	queue[rear] = k;
	rear = ++rear % MAX;
	return 0;
}

int get() {
	if (queue_empty()) {
		printf("empty!");
		return -1;
	}

	int v = queue[front];
	front = ++front % MAX;
	return v;
}

int parent[MAX];
int check[MAX];
int path[MAX];


void set_path(int V) {
	int i, cnt = 0;
	int* temp;
	temp = (int*)malloc(sizeof(int) * V);

	i = char2int(SINK);	
	while (i>=0) {
		temp[cnt++] = i;
		i = parent[i];
	}
	

	for (i = 0; i < cnt; i++) {
		path[i] = temp[cnt - i - 1];
	}
	path[i] = -1;

	printf("\npath check\n");										//
	for (i = 0; path[i] >= 0; i++)										//
		printf("%c ", int2char(path[i]));							//


	free(temp);
}

int get_augment_path(int a[][MAX], int V, int S, int T) {
	int i, j;
	init_queue();
	for (i = 0; i < V; i++) {
		parent[i] = -1;
		check[i] = 0;
	}

	i = S;
	if (check[i] == 0) {
		put(i);
		check[i] = 1;
		printf("\naug Path check\n");						//
		while (!queue_empty()) {
			i = get();
			printf("%c", int2char(i));					//
			if (i == T) break;
			for (j = 0; j < V; j++) {
				if ((a[i][j] != 0) && (check[j] == 0)) {
					put(j);
					check[j] = 1;
					parent[j] = i;
				}
			}
		}

		set_path(V);
		if (i == T) return 1;
		else return 0;
	}

}

void construct_residual(int c[][MAX], int f[][MAX], int r[][MAX], int V) {
	int i, j;

	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			r[i][j] = c[i][j] - f[i][j];
}

void network_flow(int c[][MAX], int f[][MAX], int r[][MAX], int V, int S, int T) {
	int i, min;
	clear_matrix(f, V);
	clear_matrix(r, V);
	construct_residual(c, f, r, V);

	while (get_augment_path(r, V, S, T)) {
		min = MAX;
		
		for (i = 1; path[i] >= 0; i++) {
			if (min > r[path[i - 1]][path[i]])
				min = r[path[i - 1]][path[i]];
		}
		printf("\nmin check %d\n", min);			//

		for (i = 1; path[i] >= 0; i++) {
			f[path[i - 1]][path[i]] = f[path[i - 1]][path[i]] + min;
			f[path[i]][path[i - 1]] = -f[path[i - 1]][path[i]];
		}

		construct_residual(c, f, r, V);
	}

}


int main() {
	int V, E;
	int Capacity[MAX][MAX];
	int Flow[MAX][MAX];
	int Residual[MAX][MAX];
	fp = fopen("capacity.txt", "rt");

	input_matrix(Capacity, &V, &E);

	network_flow(Capacity, Flow, Residual, V, char2int(SOURCE), char2int(SINK));

	print_matrix(Capacity, Flow, Residual, V);

	fclose(fp);
}