#include <stdio.h>

void main()
{
	FILE* fp;
	fp = fopen("example.txt", "r");

	if (fp == NULL)
		printf("There is no file\n");
	else
		printf("Success !\n");
}