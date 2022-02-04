#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
	char s1[20];								// putty에 현재 마우스위치를 출력해주기 위한 배열 s1을 선언한다.
	char xpos[20] = "Xpos : ";					// x좌표 위치를 출력받기 위한 안내를 위한 xpos 배열을 선언하고 Xpos : 문자열을 저장한다.
	char ypos[20] = "Ypos : ";					// y좌표 위치를 출력받기 위한 안내를 위한 ypos 배열을 선언하고 Ypos : 문자열을 저장한다.
	char c;										// 문자를 입력받을 c 변수를 선언한다.
	char x[5];									// 입력받은 x좌표를 저장하기 위한 x배열을 선언한다.
	char y[5];									// 입력받은 y좌표를 저장하기 위한 y배열을 선언한다.
	unsigned long bytes_read;					// putty에 값들의 출력을 위한 bytes_read 변수를 선언한다.

	HANDLE hSerial;								// serial port로 사용할 hSerial을 선언한다.

	unsigned long bytes_written;				// putty로부터 값들을 입력받기 위한 bytes written 변수를 선언한다.
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-dcb 참고
	DCB dcbSerialParams = { 0 };
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-commtimeouts 참고
	COMMTIMEOUTS timeouts = { 0 };

	// https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-createfilea 참고
	hSerial = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// VSPE 세팅을 했음에도 불구하고, COM port를 여는 과정에 문제가 생긴다면, L"COM2"에서 L을 지우고 다시 빌드하세요.)
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("ERROR!! COM2 PORT NOT FOUND\n");					// COM2 포트를 찾지 못했을 경우
		}
		printf("ERROR!! FAIIED TO ACCESS TO COM2 SUCCESSFULLY\n");		// 에러 문구 출력
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcommstate 참고
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR!! FAIIED TO GET SERIAL PARAMETERS\n");		// 만약 통신이 제대로 안되었을시 에러임을 출력
	}

	dcbSerialParams.BaudRate = CBR_57600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.StopBits = ONESTOPBIT;									// serial 통신 파라미터 설정
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommstate 참고
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR, Setting serial port state");
	}

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	POINT a;										// 마우스의 현재 위치를 받아오기 위한 포인터변수 선언
	INPUT_RECORD rec;								// 마우스 이벤트를 입력받기 위한 rec 선언
	DWORD dw;										// 마우스 상태를 나타내는 변수
	HANDLE hIn, hOut;								// 마우스 입력을 받기 위한 변수

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD Mode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hIn, Mode))
		printf("ERROR at SetConsoleMode\n");
	Mode = ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hIn, Mode))
		printf("ERROR at SetConsoleMode\n");		// putty와 cmd 창 에서의 에러를 없애주기 위한 코드

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommtimeouts 참고
	if (!SetCommTimeouts(hSerial, &timeouts))
	{
		printf("ERROR!! Time Out\n");			// 만약 putty와의 통신의 시간초과 발생시 에러 출력
		return 1;
	}
	else										// putty와의 통신이 에러없이 이루어졌을 때
	{
		while (1)								// 무한루프
		{
			ReadConsoleInput(hIn, &rec, 1, &dw);				// 마우스의 상태를 입력받는다.
			GetCursorPos(&a);									// 마우스의 현재 위치를 입력받는다.
			sprintf(s1, "X = %d, Y = %d\n\r", a.x, a.y);			// 입력받은 마우스의 현재 위치를 sprintf를 사용해 s1배열에 저장한다.
			WriteFile(hSerial, s1, 20, &bytes_written, NULL);		// s1배열, 즉 입력받은 마우스의 현재 위치를 계속해서 putty에 출력한다.
			Sleep(30);											// 지연시간을 위한 Sleep 함수

			if (rec.EventType == MOUSE_EVENT) {				
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK) {					// 만약 마우스의 더블클릭이 발생하면 if문을 실행한다.
						WriteFile(hSerial, xpos, 6, &bytes_written, NULL);					// Xpos : 를 putty에 출력한다.
						int i = 0;														// x배열에 접근하여 값들을 넣기 위한 인덱싱변수 i를 0으로 초기화
						while (i < 5) {												// 만약 인덱싱변수 i가 x배열의 크기를 초과하면 while문을 종료
							ReadFile(hSerial, &c, 1, &bytes_read, NULL);				// putty를 통해 문자를 입력받고 하나씩 c변수에 저장한다.
							if (bytes_read == 1) {										// c에 한 문자가 저장되었을 때
								x[i] = c;										// x[i]번째 배열에 putty로부터 입력받은 문자를 저장한다.
								if (x[i] == 13) break;							// 만약 putty로 부터 입력받은 문자가 엔터일 경우 x배열의 저장을 종료한다.
								i++;											// i를 증가시켜 putty로 부터 입력받은 값들을 차례차례 x배열에 저장한다.
							}
						}
						
						WriteFile(hSerial, "\n\r", 2, &bytes_written, NULL);			// 줄바꿈을 위하여 WriteFile함수를 사용하였다.		
						WriteFile(hSerial, ypos, 6, &bytes_written, NULL);				// Ypos : 를 putty에 출력한다.
						i = 0;														// 인덱싱을 처음부터 하기위하여 i를 0으로 초기화
						while (i < 5) {												// 만약 인덱싱변수 i가 y배열의 크기를 초과하면 while문을 종료
							ReadFile(hSerial, &c, 1, &bytes_read, NULL);				// putty를 통해 문자를 입력받고 하나씩 c변수에 저장한다.
							if (bytes_read == 1) {									// c에 한 문자가 저장되었을 때
								y[i] = c;										// y[i]번째 배열에 putty로부터 입력받은 문자를 저장한다.
								if (y[i] == 13) break;							// 만약 putty로 부터 입력받은 문자가 엔터일 경우 y배열의 저장을 종료한다.
								i++;											// i를 증가시켜 putty로 부터 입력받은 값들을 차례차례 y배열에 저장한다.
							}
						}
						

						SetCursorPos(atoi(x), atoi(y));								// putty로 부터 입력받은 x, y배열의 문자열값을 atoi를 통해 정수화하고 그 좌표로 커서를 이동시킨다.
						WriteFile(hSerial, "\n\r", 2, &bytes_written, NULL);		// 줄바꿈을 위한 WriteFile함수 사용
					}
				}
			}
		}


		CloseHandle(hSerial);					// hSerial의 사용을 종료한다.
		return 0;								
	}
}
