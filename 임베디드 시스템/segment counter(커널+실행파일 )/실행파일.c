#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static struct termios init_setting, new_setting;
char seg_num[10] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90 };
char seg_dnum[10] = { 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x58, 0x00, 0x10 };

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08

void init_keyboard()
{
	tcgetattr(STDIN_FILENO, &init_setting);
	new_setting = init_setting;
	new_setting.c_lflag &= ~ICANON;
	new_setting.c_lflag &= ~ECHO;
	new_setting.c_cc[VMIN] = 0;
	new_setting.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_setting);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &init_setting);
}

char get_key()
{
	char ch = -1;

	if (read(STDIN_FILENO, &ch, 1) != 1)
		ch = -1;
	return ch;
}

void print_menu()
{
	printf("\n----------menu----------\n");
	printf("[u] : up count\n");
	printf("[d] : down count\n");
	printf("[p] : set count\n");
	printf("[q] : exit count\n");
	printf("------------------------\n\n");
}

int main(int argc, char** argv)
{
	short data[4];
	char key;
	int tmp_n;
	int delay_time;

	char buff;
	char tmp;
	char prev = 'r';
	char updown;

	int dev = open("/dev/my_segment", O_RDWR); // if you want read='O_RDONLY' write='O_WRONLY', read&write='O_RDWR'
	int dev2 = open("/dev/my_gpio", O_RDWR);

	if (dev == -1 || dev2 == -1) {
		printf("Opening was not possible!\n");
		return -1;
	}
	printf("device opening was successfull!\n");

	init_keyboard();
	print_menu();
	tmp_n = 0;
	delay_time = 1000;

	data[0] = (seg_num[0] << 4) | D1;
	data[1] = (seg_num[0] << 4) | D2;
	data[2] = (seg_num[0] << 4) | D3;
	data[3] = (seg_num[0] << 4) | D4;

	int idx1 = 0;
	int idx10 = 0;
	int idx100 = 0;
	int idx1000 = 0;

	while (1) {
		read(dev2, &buff, 1);
		updown = 0;
		prev = tmp;
		tmp = buff;
		if (prev != tmp) {
			if (tmp == '1') {
				updown = 'U';
				printf("\nUP!\n");
			}
			else if (tmp == '2') {
				updown = 'D';
				printf("\nDOWN!\n");
			}
		}



		key = get_key();
		if (key == 'q') {
			printf("exit this program.\n");
			break;
		}
		else if (key == 'u' || updown == 'U') {
			idx1++;
			if (idx1 > 9) {
				idx1 = 0;
				idx10++;
				if (idx10 > 9) {
					idx10 = 0;
					idx100++;
					if (idx100 > 9) {
						idx100 = 0;
						idx1000++;
						if (idx1000 > 9) {
							idx1000 = 0;
						}
					}
				}
			}

			data[0] = (seg_num[idx1000] << 4) | D1;
			data[1] = (seg_num[idx100] << 4) | D2;
			data[2] = (seg_num[idx10] << 4) | D3;
			data[3] = (seg_num[idx1] << 4) | D4;

		}

		else if (key == 'd' || updown == 'D') {
			idx1--;

			if (idx1 < 0 && idx10 == 0 & idx100 == 0 && idx1000 == 0) {
				idx1 = 9;
				idx10 = 9;
				idx100 = 9;
				idx1000 = 9;
			}

			else if (idx1 < 0 && idx10 == 0 && idx100 == 0 && idx1000 > 0) {
				idx1 = 9;
				idx10 = 9;
				idx100 = 9;
				idx1000--;
			}

			else if (idx1 < 0 && idx10 == 0 && idx100 > 0) {
				idx1 = 9;
				idx10 = 9;
				idx100--;
			}

			else if (idx1 < 0 && idx10 > 0) {
				idx1 = 9;
				idx10--;
			}

			data[0] = (seg_num[idx1000] << 4) | D1;
			data[1] = (seg_num[idx100] << 4) | D2;
			data[2] = (seg_num[idx10] << 4) | D3;
			data[3] = (seg_num[idx1] << 4) | D4;

		}

		else if (key == 'p') {
			printf("\n");
			char arr[4] = { 0, };
			char set;
			printf("count set\n");
			for (int i = 0; i < 4; i++) {
				set = get_key();
				while (1) {
					if (set >= '0' && set <= '9') {
						arr[i] = set;
						break;
					}
					else {
						set = get_key();
					}
					write(dev, &data[tmp_n], 2);
					usleep(delay_time);

					tmp_n++;
					if (tmp_n > 3) {
						tmp_n = 0;
					}
				}
				printf("%d", arr[i] - '0');
			}
			idx1000 = arr[0] - '0';
			idx100 = arr[1] - '0';
			idx10 = arr[2] - '0';
			idx1 = arr[3] - '0';

			printf("\nend input\n");

			data[0] = (seg_num[idx1000] << 4) | D1;
			data[1] = (seg_num[idx100] << 4) | D2;
			data[2] = (seg_num[idx10] << 4) | D3;
			data[3] = (seg_num[idx1] << 4) | D4;
		}

		write(dev, &data[tmp_n], 2);
		usleep(delay_time);

		tmp_n++;
		if (tmp_n > 3) {
			tmp_n = 0;
		}
	}
	close_keyboard();
	write(dev, 0x0000, 2);
	close(dev);
	close(dev2);
	return 0;
}

