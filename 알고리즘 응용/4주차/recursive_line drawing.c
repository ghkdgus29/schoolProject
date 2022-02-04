#include <stdio.h>
#include <stdlib.h>
#define BLK 20

int recursive_line(int* canvas, int x1, int y1, int x2, int y2) {
	canvas[y1 * BLK + x1] = 1;
	canvas[y2 * BLK + x2] = 1;
	if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1) return 1;
	else {
		int cx = (x1 + x2) / 2;
		int cy = (y1 + y2) / 2;
		recursive_line(canvas, x1, y1, cx, cy);
		recursive_line(canvas, cx, cy, x2, y2);
	}
}

void main() {
	int i, j;
	int* canvas = (int*)calloc(BLK * BLK, sizeof(int));

	recursive_line(canvas, 1, 1, 15, 15);

	for (i = 0; i < BLK; i++) {
		for (j = 0; j < BLK; j++) {
			printf("%-2d", canvas[i * BLK + j]);
		}
		printf("\n");
	}

}