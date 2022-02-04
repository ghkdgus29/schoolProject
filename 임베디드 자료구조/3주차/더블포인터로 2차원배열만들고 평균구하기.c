#include <stdio.h>
#include <stdlib.h>

void make_matrix(int** m, int len) {
	int x, y;
	for (y = 0; y < len; y++)
		for (x = 0; x < len; x++) {
			scanf("%d", m[y] + x);

		}
}

void main() {
	int len;
	float avg = 0;
	printf("Input your square matrix size : ");
	scanf("%d", &len);

	int** matrix = (int**)malloc(len * sizeof(int*));
	for (int i = 0; i < len; i++) matrix[i] = (int*)malloc(len * sizeof(int));
	make_matrix(matrix, len);

	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			avg += matrix[i][j];

		}
		free(matrix[i]);
	}
	free(matrix);
	avg /= (len * len);
	printf("ЦђБе : %2.2f", avg);

}