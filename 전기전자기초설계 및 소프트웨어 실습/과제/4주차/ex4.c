#include <stdio.h>

int main(int argc, char* argv[]) {
	int fi;

	printf("main function get %d argv\n", argc);
	for (fi = 0; fi < argc; fi++) {
		printf("argv[%d] : %s\n", fi, argv[fi]);
	}
}