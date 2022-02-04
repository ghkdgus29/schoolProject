#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* fp;
	
	char* str = "The key to realizing a dream is to focus not on success but significance - and then even the small steps and little victories along your path will take on greater meaning.";

	char buf[200];
	int fi;

	fp = fopen("test_text.txt", "wt");
	if (fp == NULL) {
		printf("Fail to open file\n");
		return -1;
	}
	fputc('T', fp);
	fputc('E', fp);
	fputc('S', fp);
	fputc('T', fp);
	fputc('\n', fp);

	fputs(str, fp);
	fclose(fp);
	fp = fopen("test_text.txt", "rt");

	for (fi = 0; fi < 5; fi++) {
		printf("%c", fgetc(fp));
	}

	fgets(buf, 200, fp);
	printf("%s", buf);
	fclose(fp);
	return 0;			
}