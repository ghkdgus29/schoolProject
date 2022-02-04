#include <stdio.h>

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,1,1,1,0,0,
					0,1,0,0,0,1,0,
					0,1,0,0,0,1,0,
					0,1,0,0,1,0,0,
					0,0,1,1,1,0,0,
					0,0,0,0,0,0,0, };

void recur_fill(int x, int y) {
	if (canvas[y][x] == 1 || canvas[y][x] == 2) return;
	else {
		canvas[y][x] = 2;
		recur_fill(x - 1, y);
		recur_fill(x + 1, y);
		recur_fill(x, y - 1);
		recur_fill(x, y + 1);
	}
}

void main() {
	int i, j;
	recur_fill(4, 3);
	
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}
	
}