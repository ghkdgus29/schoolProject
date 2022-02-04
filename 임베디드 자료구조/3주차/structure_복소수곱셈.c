#include <stdio.h>

typedef struct _complex {
	float re;
	float im;
}CNum;

CNum multiply(CNum a, CNum b) {
	CNum c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.re * b.im + a.im * b.re;
	return c;
}

void main()
{
	CNum a = { 1.3, 3.9 };
	CNum b = { -3.3, 8.4 };
	CNum result;

	result = multiply(a, b);

	printf("result = %f + %fj", result.re, result.im);
}