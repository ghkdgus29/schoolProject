#include <stdio.h>

void main()
{
	FILE* fp;
	fp = fopen("example.txt", "r");
	int score, num = 0;
	float avg=0;

	while (!feof(fp)) {
		fscanf(fp, " %d\n", &score);
		avg += score;
		num++;
	}
	avg /= (float)num;
	printf("average = %f\n", avg);
	fclose(fp);
}