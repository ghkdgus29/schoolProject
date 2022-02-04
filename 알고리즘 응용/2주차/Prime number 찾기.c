#include <stdio.h>
#include <Windows.h>

int main()
{
	int i, N;
	int* arr;

	printf("Input :");
	scanf("%d", &N);

	arr = (int*)calloc(N + 1, sizeof(int));
	for (i = 2; i < N; i++) {
		if (arr[i] == 0) {
			for (int j = i + i; j <= N; j += i) arr[j] = 1;
		}
	}
	for (i = 2; i < N; i++) {
		if (arr[i] == 0) printf("%d ", i);
	}

	free(arr);

	return 0;
}