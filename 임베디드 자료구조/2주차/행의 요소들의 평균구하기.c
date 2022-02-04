#include <stdio.h>
#define ROWS 4
#define COLS 3

float get_row_avg(int m[][COLS], int r) {
	int* start, * end;
	float avg = 0;

	start = *(m + r);      
	end = *(m + r) + 2; 

	while (start <= end) {
		avg += *start;
		start++;
	}
	avg /= COLS;
	return avg;
}

int main()
{
	int r;
	int m[ROWS][COLS] = { {1,3,5},{9,7,3}, {3,3,9} ,{2,8,6} };

	printf("평균을 구하려는 행을 입력하세요 : ");
	scanf("%d", &r);
	
	float avg = get_row_avg(m, r-1);
	printf("3행의 평균값은 %2.2f입니다. \n", avg);
	return 0;
	
}
