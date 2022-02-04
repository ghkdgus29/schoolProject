#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float insert_sort(float a, float b, float c, float d, float e, float f, float g, float h, float i) {			// 정보를 3x3으로 입력받고 이를 insert_sort를 통해 정렬, 중간값을 반환한다.
	int k, j;
	float t;
	float arr[] = { a, b, c, d, e, f, g, h, i };

	for (k = 1; k < 9; k++) {
		t = arr[k];
		j = k;
		while (j > 0 && arr[j - 1] > t) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = t;
	}
	return arr[4];
}

void read_file(float* input, int height, int width, char filename[]) {					// 원본 텍스트 파일 정보를 input 에 저장한다.
	int x, y;
	FILE* fp;

	fp = fopen(filename, "r");

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			fscanf(fp, "%f", &input[y * width + x]);
		}
		fscanf(fp, "\n");
	}
	fclose(fp);
}

void median_filtering(float* input, float* output, int height, int width) {
	int x, y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if ((y == 0) && (x == 0)) output[y * width + x] = insert_sort(0, 0, 0, 0, 0, input[x], input[x + 1], input[(y + 1) * width], input[(y + 1) * width + 1]);
			else if ((y == 0) && (x == width - 1)) output[y * width + x] = insert_sort(0, 0, 0, 0, 0, input[x], input[x - 1], input[(y + 1) * width + x - 1], input[(y + 1) * width + x]);
			else if ((y == height - 1) && (x == 0)) output[y * width + x] = insert_sort(0, 0, 0, 0, 0, input[(y - 1) * width], input[(y - 1) * width + 1], input[y * width], input[y * width + 1]);
			else if ((y == height - 1) && (x == width - 1))
				output[y * width + x] = insert_sort(0, 0, 0, 0, 0, input[(y - 1) * width + x - 1], input[(y - 1) * width + x], input[y * width + x - 1], input[y * width + x]);
			else if (y == 0)
				output[y * width + x] = insert_sort(0, 0, 0, input[x - 1], input[x], input[x + 1], input[(y + 1) * width + x - 1], input[(y + 1) * width + x], input[(y + 1) * width + x + 1]);
			else if (y == height - 1)
				output[y * width + x] = insert_sort(0, 0, 0, input[(y - 1) * width + x - 1], input[(y - 1) * width + x], input[(y - 1) * width + x + 1], input[y * width + x - 1], input[y * width + x], input[y * width + x + 1]);
			else if (x == 0)
				output[y * width + x] = insert_sort(0, 0, 0, input[(y - 1) * width + x], input[(y - 1) * width + x + 1], input[y * width + x], input[y * width + x + 1], input[(y + 1) * width + x], input[(y + 1) * width + x + 1]);
			else if (x == width - 1)
				output[y * width + x] = insert_sort(0, 0, 0, input[(y - 1) * width + x - 1], input[(y - 1) * width + x], input[y * width + x - 1], input[y * width + x], input[(y + 1) * width + x - 1], input[(y + 1) * width + x]);

			else output[y * width + x] = insert_sort(input[(y - 1) * width + x - 1], input[(y - 1) * width + x], input[(y - 1) * width + x + 1], input[y * width + x - 1], input[y * width + x], input[y * width + x + 1], input[(y + 1) * width + x - 1], input[(y + 1) * width + x], input[(y + 1) * width + x + 1]);
		}		
	}
}							// 모서리부분에서 3x3 마스크가 원본텍스트를 벗어난 경우 그 값은 0으로 설정하였다. 3x3의 원본을 정렬한후 중간값을 변환텍스트에 저장한다.

void main() {
	int height = 44, width = 45;

	int x, y;
	float* input, * output;
	char filename[] = "noisy_data.txt";
	input = (float*)malloc(width * height * sizeof(float));
	output = (float*)malloc(width * height * sizeof(float));

	read_file(input, height, width, filename);
	median_filtering(input, output, height, width);

	for (y = 0; y < height; y++) {						// 원본 텍스트 파일을 display
		for (x = 0; x < width; x++) {
			printf("%3.0f ", input[y * width + x]);
		}
		printf("\n");
	}
	printf("\n\n\n");

	for (y = 0; y < height; y++) {						// 변환한 텍스트 파일을 display , 잡음이 상대적으로 제거되었다.
		for (x = 0; x < width; x++) {
			printf("%3.0f ", output[y * width + x]);
		}
		printf("\n");
	}
	printf("\n\n");
	printf("done");

}
