#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
	char s1[20];								// putty�� ���� ���콺��ġ�� ������ֱ� ���� �迭 s1�� �����Ѵ�.
	char xpos[20] = "Xpos : ";					// x��ǥ ��ġ�� ��¹ޱ� ���� �ȳ��� ���� xpos �迭�� �����ϰ� Xpos : ���ڿ��� �����Ѵ�.
	char ypos[20] = "Ypos : ";					// y��ǥ ��ġ�� ��¹ޱ� ���� �ȳ��� ���� ypos �迭�� �����ϰ� Ypos : ���ڿ��� �����Ѵ�.
	char c;										// ���ڸ� �Է¹��� c ������ �����Ѵ�.
	char x[5];									// �Է¹��� x��ǥ�� �����ϱ� ���� x�迭�� �����Ѵ�.
	char y[5];									// �Է¹��� y��ǥ�� �����ϱ� ���� y�迭�� �����Ѵ�.
	unsigned long bytes_read;					// putty�� ������ ����� ���� bytes_read ������ �����Ѵ�.

	HANDLE hSerial;								// serial port�� ����� hSerial�� �����Ѵ�.

	unsigned long bytes_written;				// putty�κ��� ������ �Է¹ޱ� ���� bytes written ������ �����Ѵ�.
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-dcb ����
	DCB dcbSerialParams = { 0 };
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-commtimeouts ����
	COMMTIMEOUTS timeouts = { 0 };

	// https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-createfilea ����
	hSerial = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// VSPE ������ �������� �ұ��ϰ�, COM port�� ���� ������ ������ ����ٸ�, L"COM2"���� L�� ����� �ٽ� �����ϼ���.)
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("ERROR!! COM2 PORT NOT FOUND\n");					// COM2 ��Ʈ�� ã�� ������ ���
		}
		printf("ERROR!! FAIIED TO ACCESS TO COM2 SUCCESSFULLY\n");		// ���� ���� ���
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcommstate ����
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR!! FAIIED TO GET SERIAL PARAMETERS\n");		// ���� ����� ����� �ȵǾ����� �������� ���
	}

	dcbSerialParams.BaudRate = CBR_57600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.StopBits = ONESTOPBIT;									// serial ��� �Ķ���� ����
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommstate ����
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR, Setting serial port state");
	}

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	POINT a;										// ���콺�� ���� ��ġ�� �޾ƿ��� ���� �����ͺ��� ����
	INPUT_RECORD rec;								// ���콺 �̺�Ʈ�� �Է¹ޱ� ���� rec ����
	DWORD dw;										// ���콺 ���¸� ��Ÿ���� ����
	HANDLE hIn, hOut;								// ���콺 �Է��� �ޱ� ���� ����

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD Mode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hIn, Mode))
		printf("ERROR at SetConsoleMode\n");
	Mode = ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hIn, Mode))
		printf("ERROR at SetConsoleMode\n");		// putty�� cmd â ������ ������ �����ֱ� ���� �ڵ�

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommtimeouts ����
	if (!SetCommTimeouts(hSerial, &timeouts))
	{
		printf("ERROR!! Time Out\n");			// ���� putty���� ����� �ð��ʰ� �߻��� ���� ���
		return 1;
	}
	else										// putty���� ����� �������� �̷������ ��
	{
		while (1)								// ���ѷ���
		{
			ReadConsoleInput(hIn, &rec, 1, &dw);				// ���콺�� ���¸� �Է¹޴´�.
			GetCursorPos(&a);									// ���콺�� ���� ��ġ�� �Է¹޴´�.
			sprintf(s1, "X = %d, Y = %d\n\r", a.x, a.y);			// �Է¹��� ���콺�� ���� ��ġ�� sprintf�� ����� s1�迭�� �����Ѵ�.
			WriteFile(hSerial, s1, 20, &bytes_written, NULL);		// s1�迭, �� �Է¹��� ���콺�� ���� ��ġ�� ����ؼ� putty�� ����Ѵ�.
			Sleep(30);											// �����ð��� ���� Sleep �Լ�

			if (rec.EventType == MOUSE_EVENT) {				
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK) {					// ���� ���콺�� ����Ŭ���� �߻��ϸ� if���� �����Ѵ�.
						WriteFile(hSerial, xpos, 6, &bytes_written, NULL);					// Xpos : �� putty�� ����Ѵ�.
						int i = 0;														// x�迭�� �����Ͽ� ������ �ֱ� ���� �ε��̺��� i�� 0���� �ʱ�ȭ
						while (i < 5) {												// ���� �ε��̺��� i�� x�迭�� ũ�⸦ �ʰ��ϸ� while���� ����
							ReadFile(hSerial, &c, 1, &bytes_read, NULL);				// putty�� ���� ���ڸ� �Է¹ް� �ϳ��� c������ �����Ѵ�.
							if (bytes_read == 1) {										// c�� �� ���ڰ� ����Ǿ��� ��
								x[i] = c;										// x[i]��° �迭�� putty�κ��� �Է¹��� ���ڸ� �����Ѵ�.
								if (x[i] == 13) break;							// ���� putty�� ���� �Է¹��� ���ڰ� ������ ��� x�迭�� ������ �����Ѵ�.
								i++;											// i�� �������� putty�� ���� �Է¹��� ������ �������� x�迭�� �����Ѵ�.
							}
						}
						
						WriteFile(hSerial, "\n\r", 2, &bytes_written, NULL);			// �ٹٲ��� ���Ͽ� WriteFile�Լ��� ����Ͽ���.		
						WriteFile(hSerial, ypos, 6, &bytes_written, NULL);				// Ypos : �� putty�� ����Ѵ�.
						i = 0;														// �ε����� ó������ �ϱ����Ͽ� i�� 0���� �ʱ�ȭ
						while (i < 5) {												// ���� �ε��̺��� i�� y�迭�� ũ�⸦ �ʰ��ϸ� while���� ����
							ReadFile(hSerial, &c, 1, &bytes_read, NULL);				// putty�� ���� ���ڸ� �Է¹ް� �ϳ��� c������ �����Ѵ�.
							if (bytes_read == 1) {									// c�� �� ���ڰ� ����Ǿ��� ��
								y[i] = c;										// y[i]��° �迭�� putty�κ��� �Է¹��� ���ڸ� �����Ѵ�.
								if (y[i] == 13) break;							// ���� putty�� ���� �Է¹��� ���ڰ� ������ ��� y�迭�� ������ �����Ѵ�.
								i++;											// i�� �������� putty�� ���� �Է¹��� ������ �������� y�迭�� �����Ѵ�.
							}
						}
						

						SetCursorPos(atoi(x), atoi(y));								// putty�� ���� �Է¹��� x, y�迭�� ���ڿ����� atoi�� ���� ����ȭ�ϰ� �� ��ǥ�� Ŀ���� �̵���Ų��.
						WriteFile(hSerial, "\n\r", 2, &bytes_written, NULL);		// �ٹٲ��� ���� WriteFile�Լ� ���
					}
				}
			}
		}


		CloseHandle(hSerial);					// hSerial�� ����� �����Ѵ�.
		return 0;								
	}
}
