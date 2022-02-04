#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define MAZE_SIZE 19
#define MOUSE 2

#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

int* rec;

void delay(int);

void gotoxy(int x, int y)
{
	COORD Pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void record(int x, int y)
{
	static int index = 0;
	rec[index++] = x;
	rec[index++] = y;
}

int del_path(int i, int j)
{
	while (rec[j] >= 0) rec[i++] = rec[j++];
	rec[i] = -1;
}

void shortest_path()    // 최단 거리로 미로를 찾는 함수
{
	int x, y, temp;    // 그림을 그리기 위한 변수 x,y 
	int i = 0;         // i인덱스를 0으로 초기화
	
	while (rec[i] >= 0)   // i인덱스 값이 -1이면 배열의 끝부분을 의미하므로 무한루프를 나간다. (미로의 출구)
	{
		int j = i + 2;          // i인덱스와 비교할 j인덱스값을 설정해준다. 
		while (rec[j] >= 0)     // j 인덱스 값 역시 -1이면 미로의 끝부분을 의미한다. 
		{
			if ((rec[i] == rec[j]) && (rec[i + 1] == rec[j + 1])) {    // 배열에 저장된 (x, y) 순서쌍이 같다면 if 분기문을 실행
				del_path(i, j);											 // del_path() 함수
				j = i + 2;                                             // 인덱스 j는 인덱스i의 다음번 순서쌍을 의미한다.
			}
			else j += 2;       // 인덱스i, j를 통해 비교한 (x, y) 순서쌍이 다르다면 j를 증가시킨다.
		}
		i = i + 2;           // 다음번 (x, y) 순서쌍에도 똑같은 연산을 진행해준다. (인덱스 i)
	}

	i = 0;                                    // 그림 그리기
	while (rec[i] >= 0) {
		x = rec[i++];
		y = rec[i++];
		gotoxy(x + 1, y + 1);
		_putch(MOUSE);
		delay(100);
		gotoxy(x + 1, y + 1);
		_putch(' ');
	}
}

void delay(int n)
{
	for (int i = 0; i < 50000000; i++) {

	}
}

void forward(int* x, int* y, int dir)
{
	gotoxy(*x + 1, *y + 1);
	_putch(' ');

	*x = (dir == LEFT) ? --(*x) : (dir == RIGHT) ? ++(*x) : *x;
	*y = (dir == UP) ? --(*y) : (dir == DOWN) ? ++(*y) : *y;

	record(*x, *y);
	gotoxy(*x + 1, *y + 1);
	_putch(MOUSE);

}

int still_in_maze(int x, int y) {
	if (x > 0 && x < MAZE_SIZE - 1 && y > 0 && y < MAZE_SIZE - 1)
		return 1;
	else
		return 0;
}

int wall_ahead(int m[][MAZE_SIZE], int x, int y, int dir)
{
	x = (dir == LEFT) ? --x : (dir == RIGHT) ? ++x : x;
	y = (dir == UP) ? --y : (dir == DOWN) ? ++y : y;

	return m[y][x];
}

void right(int* dir)
{
	*dir <<= 1;
	*dir = (*dir > LEFT) ? UP : *dir;
}

void left(int* dir)
{
	*dir >>= 1;
	*dir = (*dir == 0) ? LEFT : *dir;
}

void right_hand_on_wall(int m[][MAZE_SIZE], int x, int y, int dir)
{
	gotoxy(x + 1, y + 1);
	_putch(MOUSE);

	forward(&x, &y, dir);
	while (still_in_maze(x, y))
	{
		
		right(&dir);
		while (wall_ahead(m, x, y, dir)) left(&dir);
		forward(&x, &y, dir);

	}
	record(-1, -1);
}


void main()
{
	rec = (int*)calloc(MAZE_SIZE * MAZE_SIZE, sizeof(int));
	int sx = MAZE_SIZE - 1;
	int sy = MAZE_SIZE - 2;

	int maze[MAZE_SIZE][MAZE_SIZE] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
									  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
									  {1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1},
									  {1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1},
									  {1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1},
									  {1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
									  {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1},
									  {1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
									  {1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
									  {1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
									  {1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1},
									  {1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1},
									  {1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1},
									  {1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1},
									  {1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
									  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
									  {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1},
									  {1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
									  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };

	int x, y;

	for (y = 0; y < MAZE_SIZE; y++)
	{
		if (y == 0) gotoxy(0, 1);
		for (x = 0; x < MAZE_SIZE; x++)
		{
			if (x == 0) printf(" ");

			if (maze[y][x] == 1) printf("%%");
			else printf(" ");
		}
		printf("\n");
	}
	right_hand_on_wall(maze, sx, sy, LEFT);
	shortest_path();

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}