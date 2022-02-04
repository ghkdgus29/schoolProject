#include <stdio.h>
#include <string.h>
#define MAX 100

int stack[MAX];
int top;

void init_stack() {
	top = -1;
}

int push(int t) {
	if (top >= MAX - 1) {
		printf("\nStack overflow !!!\n");
		return -1;
	}

	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf("\nStack underflow !!!\n");
		return -1;
	}
	return stack[top--];
}

int get_stack_top() {
	return (top < 0) ? -1 : stack[top];
}

int is_op(int k) {
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

int precedence(int op) {
	if (op == '(') return 0;
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	else return 3;
}

int is_stack_empty() {
	if (top < 0) return 1;
	return 0;
}

void postfix(char* dst, char* src) {
	char c;
	init_stack();
	while (*src) {
		if (*src == '(') {
			push(*src);
			src++;
		}
		else if (*src == ')') {
			while (get_stack_top() != '(') {
				*dst++ = pop();
				*dst++ = ' ';
			}
			pop();
			src++;
		}
		else if (is_op(*src)) {
			while (!is_stack_empty() && precedence(get_stack_top()) >= precedence(*src)) {
				*dst++ = pop();
				*dst++ = ' ';
			}
			push(*src);
			src++;
		}
		else if (*src >= '0' && *src <= '9') {
			do {
				*dst++ = *src++;
			} while (*src >= '0' && *src <= '9');
			*dst++ = ' ';
		}
		else src++;
	}
	while (!is_stack_empty()) {
		*dst++ = pop();
		*dst++ = ' ';
	}
	dst--;
	*dst = 0;
}

void main() {
	char exp[256];
	char src[256] = "(1*(2+3/4)+5)/6+7";

	postfix(exp, src);
	printf("Postfix representation : %s\n", exp);
}
