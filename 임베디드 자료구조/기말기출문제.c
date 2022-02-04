#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define BASE(i) ((char*)base + (i)*width)
typedef int(*FCMP)(const void*, const void*);

typedef struct student {
	char id[20];
	char name[20];
}student;

int id_cmp(const void* a, const void* b) {
	return (strcmp(((student*)a)->id, ((student*)b)->id));
}

int lv_search(void* key, void* base, int* num, int width, FCMP fcmp) {
	int i = 0;
	while (fcmp(BASE(i), key) != 0 && i < *num) i++;

	return(i < *num ? i : -1);
}

int lv_insert(void* key, void* base, int* num, int width, FCMP fcmp) {
	memcpy(BASE((*num)++), key, width);

	return *num - 1;
}

int lv_delete(void* key, void* base, int* num, int width, FCMP fcmp) {
	int p, i;
	if (*num > 0) {
		if ((p = lv_search(key, base, num, width, fcmp)) < 0) return -1;

		for (i = p + 1; i < *num; i++)
			memcpy(BASE(i - 1), BASE(i), width);
		(*num)--;
		return p;
	}
	return -1;
}

int lfv_search(void* key, void* base, int* num, int width, FCMP fcmp) {
	int i = 0, j;
	void* v;
	while (fcmp(BASE(i), key) != 0 && i < *num) i++;
	if (i >= *num) return -1;

	v = malloc(width);
	memcpy(v, BASE(i), width);
	for (j = i - 1; j >= 0; j--)
		memcpy(BASE(j + 1), BASE(j), width);

	memcpy(BASE(0), v, width);
	return 0;
}

void main() {
	student table[MAX], data, key;
	int nitem = 0, pos;

	strcpy(data.id, "111");
	strcpy(data.name, "Jackson");

	lv_insert(&data, table, &nitem, sizeof(student), id_cmp);
	
	strcpy(data.id, "222");
	strcpy(data.name, "Chris");


	lv_insert(&data, table, &nitem, sizeof(student), id_cmp);

	strcpy(key.id, "111");

	pos = lv_search(&key, table, &nitem, sizeof(student), id_cmp);
	printf("ID = %s -> Name = %s\n", key.id, table[pos].name);

	strcpy(key.id, "222");

	pos = lv_search(&key, table, &nitem, sizeof(student), id_cmp);
	printf("ID = %s -> Name = %s\n", key.id, table[pos].name);
}