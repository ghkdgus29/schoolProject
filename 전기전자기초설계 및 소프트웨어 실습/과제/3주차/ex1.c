#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STDNUM 96

int main() {
	int fi;
	int std[STDNUM] = { 0, };
	srand((unsigned)time(NULL));

	printf("sizeof(std[0]) = %d\n", sizeof(std[0]));
	printf("sizeof(std) = %d\n", sizeof(std));
	printf("sizeof(std)/sizeof(std[0]) = %d\n", sizeof(std) / sizeof(std[0]));

	for (fi = 0; fi < STDNUM; fi++) {
		std[fi] = rand() % 101;
		printf("student %2d's grade is %3d\n", fi, std[fi]);
	}
}