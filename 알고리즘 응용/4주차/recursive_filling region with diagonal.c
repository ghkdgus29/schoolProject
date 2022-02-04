#include <stdio.h>

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,0,1,0,0,0,
					0,0,0,1,0,0,0,
					0,1,1,1,1,0,0,
					0,0,0,0,0,1,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0, };    // filling region을 나타낼 canvas를 2차원배열로 선언

void recur_fill(int x, int y) {        // canvas 내에 1인 지역(표시된 지역) 을 2로 바꾸는 (색칠하는) 함수이다.
	if (!(canvas[y][x] == 1)) return;   // 만약 canvas내에 1이 아닌 숫자 (0과 2)를 만나면 함수를 종료한다.
	else {								// 종료조건을 만족하지 않는 경우 
		canvas[y][x] = 2;				// 1을 2로 바꾼다. 색칠에 해당한다.
		recur_fill(x - 1, y);			
		recur_fill(x + 1, y);
		recur_fill(x, y - 1);			// 입력받은 seed에 상 하 좌 우 지역에 대한 recursive 함수호출로 만약 1이 있으면 2로 바꾸고 
		recur_fill(x, y + 1);			// 1이 없다면 recursive 함수를 빠져나간다.
		recur_fill(x - 1, y - 1);
		recur_fill(x - 1, y + 1);
		recur_fill(x + 1, y - 1);		// 입력받은 seed에 대각선 지역에 대한 recursive 함수호출로 만약 1이 있으면 2로 바꾸고 
		recur_fill(x + 1, y + 1);		// 1이 없다면 recursive 함수를 빠져나간다.
	}
}

void main() {
	int i, j;							// for문 반복을 위한 변수 선언
	recur_fill(3, 1);					// 3,1 지역을 seed로 하는 recur_fill 호출

	for (i = 0; i < 7; i++) {			// 수정된 canvas를 출력하기 위한 이중 for문이다.
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}

}