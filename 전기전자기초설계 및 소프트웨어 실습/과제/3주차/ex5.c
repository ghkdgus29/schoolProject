#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int arr[100];
	int* ptr_arr;
	int fi;

	srand((unsigned)time(NULL));

	ptr_arr = &arr[0];

	printf("address of arr[0] is %p\n", &arr[0]);
	printf("arr is pointing %p\n", arr);
	printf("ptr_arr is pointing %p\n", ptr_arr);
	printf("\n");

	printf("address of arr[1] is %p\n", &arr[1]);
	printf("arr+1 is pointing %p\n", arr + 1);
	printf("ptr_arr + 1 is pointing %p\n", ptr_arr + 1);
	printf("\n");

	for (fi = 0; fi < 100; fi++) {
		arr[fi] = rand() % 101;
	}

	printf("arr[0] = %d\n", arr[0]);
	printf("*arr = %d\n", *arr);
	printf("*ptr_arr = %d\n", *ptr_arr);
	printf("\n");

	printf("arr[20] = %d\n", arr[20]);
	printf("*(arr+20) = %d\n", *(arr + 20));
	printf("*(ptr_arr+20) = %d\n", *(ptr_arr + 20));
	printf("*arr+20 = %d\n", *arr + 20);
	printf("*ptr_arr+20 = %d\n", *ptr_arr + 20);
}