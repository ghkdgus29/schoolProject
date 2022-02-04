#include <Windows.h>
#include <stdio.h>

int main() {
	int i;

	for (i = 0; i < 500; i++) {
		SetCursorPos(i, i);
		Sleep(5);
	}
	return 0;
}