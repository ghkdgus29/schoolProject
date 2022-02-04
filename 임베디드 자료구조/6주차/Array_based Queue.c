#include <stdio.h>

#define MAX 10
int front, rear;
int queue[MAX];

void init_queue() {
	front = 0;
	rear = 0;
}

void clear_queue() {
	front = rear;
}

int put(int k) {
	if ((rear + 1) % MAX == front) {
		printf("\nQueue overflow !!!\n");
		return -1;
	}
	queue[rear] = k;
	rear = ++rear % MAX;

	return k;
}

int get() {
	int j;

	if (front == rear) {
		printf("Queue underflow !!!\n");
		return -1;
	}

	j = queue[front];
	front = ++front % MAX;

	return j;
}

void print_queue() {
	int k;
	if (front == rear) printf("queue empty!!\n");
	for (k = front; k != rear; k = ++k % MAX) printf("%-6d", queue[k]);
}

void main() {
	int k;
	init_queue();

	printf("1. Put 3, 6, 9, 1, 6, 3 and print front -> rear\n");
	put(3);
	put(6);
	put(9);
	put(1);
	put(6);
	put(3);
	print_queue();

	printf("\n\n2. Conduct get and print front->rear\n");
	k = get();
	printf("Get = %d\n", k);
	print_queue();

	printf("\n\n3. Put 4, 8, 7, 2, 0, 3\n");
	put(4);
	put(8);
	put(7);
	put(2);
	put(0);
	put(3);
	print_queue();

	printf("\n\n4. Initialize your queue\n");
	init_queue();
	print_queue();

	printf("\n\n5. Conduct get\n");
	get();
	print_queue();
}
