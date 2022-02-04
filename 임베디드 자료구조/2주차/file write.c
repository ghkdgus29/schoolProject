#include <stdio.h>

void main()
{
	FILE* fp;
	fp = fopen("example.txt", "w");
	int score = 0;

	while (1) {
		printf("Input score: ");
		scanf("%d", &score);
		if (score < 0) break;
		else
			fprintf(fp, "%d\n", score);
	}
	fclose(fp);
}