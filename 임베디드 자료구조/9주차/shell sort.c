#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

void shell_sort(int* a, int N) {		// shell sort 함수, a는 데이터 주소, N은 데이터 길이
	int i, j, k, h, v;
	for (h = N / 2; h > 0; h /= 2) {		// h는 인터벌, 전체 데이터 길이를 반으로 나눠가며 0이 될때 까지 반복
		for (i = 0; i < h; i++) {			// 0부터 인터벌-1 까지 반복 (i)
			for (j = i + h; j < N; j += h) {		// i+h, 즉 인터벌만큼 벌려서 반복
				v = a[j];					// j번째 값을 v에 저장
				k = j;						// k를 인덱스로 사용해 insert sort 할거임
				while (k > h - 1 && a[k - h] > v) {		// k가 인터벌보다 크고 인터벌값중 a[j]보다 큰값이 있으면 인터벌값들 조사
					a[k] = a[k - h];		// 이전값들을 인터벌 칸수만큼 밀어넣는다.
					k -= h;					// k인덱스는 인터벌 값만큼 감소시킨다
				}
				a[k] = v;			// 반복문을 마치면 k인덱스가 a[j]가 들어가야할 위치가 된다.

			}
		}
	}
}

void main() {
	int number[SIZE] = { 5, 3, 2, 6, 12, 14, 4, 7, 0, 3 };

	shell_sort(number, 10);

	for (int i = 0; i < SIZE; i++) printf("%3d", number[i]);
}