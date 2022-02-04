#include <stdio.h>

int main() {
	char str[100];
	printf("Plz input your text : ");
	scanf("%[^\n]s", str);
	printf("your text is : %s", str);
}