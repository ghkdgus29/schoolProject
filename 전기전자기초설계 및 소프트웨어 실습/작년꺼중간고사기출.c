#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sort(char string[], int word) {
	int i = 0, j = 0, k = 0;
	char temp[20];
	char temp2[50][20] = { 0, };
	while (string[i] != '\0') {
		if (string[i] == ' ' || string[i] == '\n' || string[i] == ','|| string[i] == '.') {
			if (string[i + 1] >= 'A' && string[i + 1] <= 'z') {
				temp2[j][k] = '\0';
				k = 0;
				j++;
			}
		}
		else temp2[j][k++] = string[i];
		i++;
	}
	
	for (int i = 0; i < word - 1; i++) {
		for (int j = 0; j < word - 1 - i; j++) {
			if (strcmp(temp2[j], temp2[j+1]) > 0) {
				strcpy(temp, temp2[j]);
				strcpy(temp2[j], temp2[j + 1]);
				strcpy(temp2[j + 1], temp);
			}
		}
	}
	for (i = 0; i < word; i++) {
		printf("\n%s", temp2[i]);
	}
	
	
}


int count(char string[]) {
	int len = 0, w = 1, s = 0;
	while (string[len] != '\0') len++;
	for (int i = 0; i < len; i++) {
		if (string[i] == ' ' || string[i] == '\n')
			w++;
		if (string[i] == '.' || string[i] == '\n') s++;
		if (string[i] == '.' && string[i + 1] == '\n') s--;

	}
	printf("단어수 : %d  문장수 : %d ", w, s);
	return w;
}


void main() {
	char source[300] = { 0 };
	int word;
	FILE* fp;
	fp = fopen("input.txt", "rt");
	fread(source, sizeof(source), 1, fp);
	printf("%s\n", source);

	word = count(source);

	

	sort(source, word);


	fclose(fp);
}