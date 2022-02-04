#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int random, guess;
	srand(time(NULL));
	random = rand() % 100 + 1;
	printf("=======Up & down prediction =========\n");
	printf("type your guess(1~100) : ");
	scanf("%d", &guess);

	while (guess != random)
	{
		printf("wrong answer       ");
		while (guess > random) { printf("Down\n"); break; }
		while (random > guess) { printf("Up\n"); break; }

		printf("type your guess(1~100) ; ");
		scanf("%d", &guess);
	}

	printf("correct! Game Over");
}