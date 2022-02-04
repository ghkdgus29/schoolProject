#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

typedef struct node {
	struct node* left;
	struct node* right;
}node;

typedef int(*FCMP)(const void*, const void*);

void init_tree(node** p);
void* btv_search(void* key, node* base, int* num, int width, FCMP fcmp);
void* btv_insert(void* key, node* base, int* num, int width, FCMP fcmp);
void btv_list(node* p, void (*fptr)(void*));

void _sort(node* p, void* a, int width);
node* _balance(int n, void* a, int width);
void btv_balance(node* base, int* num, int width);
void _deletall(node* t);
void* btv_deleteall(node* base, int* num);