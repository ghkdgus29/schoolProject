#include <Windows.h>
#include <stdio.h>

int main() {
	POINT a;
	while (1) {
		GetCursorPos(&a);
		printf("X = %d, Y = %d\n", a.x, a.y);
	}
	return 0;
}