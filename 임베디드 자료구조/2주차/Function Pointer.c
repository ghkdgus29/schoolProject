#include <stdio.h>

int add(int x, int y) {
	int result;
	result = x + y;
	return result;
}

int sub(int x, int y) {
	int result;
	result = x - y;
	return result;
}

void main() {
	int result;
	int(*fptr)(int, int);

	fptr = add;
	result = fptr(10, 20);
	printf("%d\n", result);

	fptr = sub;
	result = fptr(10, 20);
	printf("%d\n", result);
	
}