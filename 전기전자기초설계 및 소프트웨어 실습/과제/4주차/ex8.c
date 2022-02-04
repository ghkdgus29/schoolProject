#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 100

int main() {
	FILE* ori;
	FILE* cp;
	char buffer[BUF_SIZE];
	int count;

	ori = fopen("C:\\Users\\user\\Desktop\\image.bmp", "rb");
	cp = fopen("C:\\Users\\user\\Desktop\\copy.bmp", "wb");

	if (ori == NULL || cp == NULL) {
		printf("Fail to open file\n");
		return -1;
	}

	while (1) {
		count = fread(buffer, 1, BUF_SIZE, ori);
		if (count < BUF_SIZE) {
			if (feof(ori) != 0) {
				fwrite(buffer, 1, count, cp);
				printf("Copy success!!\n");
				break;
			}
			else {
				printf("Copy FAIL!!\n");
				break;
			}
		}
		else {
			fwrite(buffer, 1, BUF_SIZE, cp);
		}
	}
	fclose(ori);
	fclose(cp);
	return 0;
}