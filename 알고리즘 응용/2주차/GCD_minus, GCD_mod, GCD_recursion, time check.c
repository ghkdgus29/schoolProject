#include <stdio.h>
#include <Windows.h>

int GCD_minus(int a, int b)
{
	
	if (b == 0) return a;
	else if (a >= b) a = a - b;
	else {
		int temp = a;
		a = b;
		b = temp;
	}
	
	GCD_minus(a, b);
}

int GCD_mod(int a, int b)
{
	if (b == 0) return a;
	else if (a >= b) a = a % b;
	else {
		int temp = a;
		a = b;
		b = temp;
	}

	GCD_mod(a, b);
}

int GCD_recursion(int a, int b)
{
	if (b == 0) return a;
	else return GCD_recursion(b, a % b);
}

void main()
{
	LARGE_INTEGER freq, start, stop;
	double diff1, diff2, diff3;

	int a, b, val1, val2, val3;
	printf("input two values \n");
	scanf("%d %d", &a, &b);

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	for (int i = 0; i < 1000; i++) val1 = GCD_minus(a, b);

	QueryPerformanceCounter(&stop);
	diff1 = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	for(int i = 0; i < 1000;i++) val2 = GCD_mod(a, b);

	QueryPerformanceCounter(&stop);
	diff2 = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	for (int i = 0; i < 1000; i++) val3 = GCD_recursion(a, b);

	QueryPerformanceCounter(&stop);
	diff3 = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;


	printf("val1 = %d  running time = %f \nval2 = %d  running time = %f \nval3 = %d  running time = %f\n", val1, diff1, val2, diff2, val3, diff3);
	return 0;
}