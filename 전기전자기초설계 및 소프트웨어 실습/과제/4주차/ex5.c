#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int fi;
	int* ptr = NULL;
	int size = 0;

	srand((unsigned)time(NULL));

	while (size <= 0) {
		printf("size for memory : ");
		scanf("%d", &size);
		if (size <= 0) printf("\n size must be bigger than 0\n");
	}
	ptr = (int*)malloc(size * sizeof(int));
	if (ptr == NULL) {
		printf("FAIL : CANNOT ASSIGN!!\n");
		return -1;
	}
	for (fi = 0; fi < size; fi++) {
		ptr[fi] = rand() % 101;
		printf("ptr[%3d] = %3d\n", fi, ptr[fi]);
	}
	free(ptr);

	return 0;
}