#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 비트맵 파일 규격에선, 가로방향 데이터 크기는 4의 배수(바이트단위)로 한다.
// 왜?? -> 이유를 찾아서 레포트에 첨부
#define BYTE unsigned char

FILE* file;// 파일 포인터 선언
int rwsize, rwsize2;// 가로 라인 당 바이트 수
BYTE* lpImg;// 입력 데이터가 저장되는 메모리 주소를 지시하는 포인터
BYTE* lpOutImg;// 출력 데이터가 저장되는 메모리 주소를 지시하는 포인터
RGBQUAD* pRGB;// Lookup Table
BITMAPFILEHEADER hf;// 비트맵 파일 해더
BITMAPINFOHEADER hinfo;// 비트맵 정보 헤더

void egdt(void); // 과제!!

int main()
{

	egdt();

	return 0;
}


void egdt(void)
{
	int i, j;
	BYTE R, G, B, GRAY;

	BITMAPFILEHEADER ohf; // 출력 파일을 위한 비트맵 파일 헤더
	BITMAPINFOHEADER ohinfo; // 출력 파일을 위한 비트맵 파일 정보 헤더

	printf("RUN egdt()\n\n");

	file = fopen("ori.bmp", "rb");// 파일을 읽기 모드로 엶

	if (file == NULL)// 파일 열기에 실패하면
	{
		printf("Error At File open!!!\n");
		exit(1);
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // 파일을 열어 파일의 BITMAPFILEHEADER만큼을 hf에 저장

	if (hf.bfType != 0x4D42) // 0X4D42 == BM(아스키 코드), 비트맵파일은 맨 처음 시작이 BM이다. 따라서 첫 부분을 읽고, 그 부분을 분석해서 비트맵이 아니면...
	{
		printf("File is NOT BMP\n");
		exit(1);
	}

	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, file); // 파일을 열어 파일의 BITMAPINFOHEADER만큼을 hinfo에 저장

	printf("File size : %d\n", hf.bfSize); // 입력받은 비트맵 파일의 크기를 화면에 출력함
	printf("offset : %d\n", hf.bfOffBits); // 입력받은 비트맵 파일에서 실제 데이터가 시작하는 오프셋
	printf("Image Size : (%dX%d)\n", hinfo.biWidth, hinfo.biHeight); // 입력받은 비트맵 파일의 픽셀 개수를 화면에 출력함
	printf("%d BPP \n", hinfo.biBitCount); // 입력받은 비트맵 파일의 픽셀당 비트수를 출력

	ohf = hf;
	ohinfo = hinfo;
	// 원본 파일과 출력 파일은 거의 유사하므로 헤더파일을 복사해서 사용한다.
	// 단, 일부 값이 바뀌는 것들이 있으므로, 나중에 반드시 수정해줘야 한다.(bfOffBits, biBitCount 등...)

	rwsize = WIDTHBYTES(hinfo.biBitCount * hinfo.biWidth);
	rwsize2 = WIDTHBYTES(8 * ohinfo.biWidth);
	//입력 파일과 출력 파일에서 가로 한 줄당 데이터가 몇비트인지 저장한다
	//이때 입력파일에서 가로폭과(=가로폭에 있는 총 픽셀 갯수)
	//한 픽셀당 바이트 수를 곱하면, 한 줄이 몇 비트를 차지하는지 알 수 있다.
	//rwsize2를 계산할 때는 입력 파일과 출력 파일의 픽셀 사이즈는 동일하기 때문에 입력 파일의 가로폭을 그대로 쓰며
	//8BPP 파일의 픽셀당 비트수는 8비트이므로 8을 곱한다.

	fseek(file, hf.bfOffBits, SEEK_SET);
	//비트맵 파일에서 실제 값들이 저장돼 있는 위치로 포인터를 이동한다

	lpImg = (BYTE*)malloc(rwsize * hinfo.biHeight);
	//입력 받은 파일을 위한 메모리를 할당한다

	fread(lpImg, sizeof(char), rwsize * hinfo.biHeight, file);
	//포인터는 현재 실제 데이터 값이 저장된 곳의 맨 처음으로 이동한 상황이다. 이때 이미지의 세로폭과(=가로폭에 있는 총 픽셀 갯수)
	//한 줄당 비트수를 곱하면, 전체 데이터의 크기가 나온다. fread함수를 이용해 이미지의 데이터를 char변수의 크기만큼
	//나눠서 저장한다.

	fclose(file);
	//파일사용이 완료되었으므로 닫는다.

	lpOutImg = (BYTE*)malloc(rwsize2 * ohinfo.biHeight);
	//출력한 파일을 위한 메모리를 할당한다

	for (i = 0; i < ohinfo.biHeight; i++)
	{
		for (j = 0; j < ohinfo.biWidth; j++)
		{
			B = lpImg[i * rwsize + 3 * j + 0];
			G = lpImg[i * rwsize + 3 * j + 1];
			R = lpImg[i * rwsize + 3 * j + 2];
			//앞서 언급했듯 rwsize는 가로 한줄의 데이터 크기다. 따라서 여기에 i를 곱한다는것은 i+1번째 줄의 데이터를
			//사용함을 의미한다. 한편 컬러에서 한 픽셀당 데이터는 3바이트가 필요하므로 3*j를 해서 j+1번째 픽셀로 이동한다.
			//즉 앞의 과정을 통해  i+1번째 줄의 j+1번째칸의 픽셀의 데이터를 저장한 배열의 값을 확인하게 된다.
			//이때 배열의 위치를 +0,+1,+2를 해서 데이터의 B G R값을 각각 저장한다.

			GRAY = (BYTE)(0.299 * R + 0.587 * G + 0.114 * B);
			//각각 저장한 R,G,B값에 적당한 값을 곱하고 더해서 밝기값을 만든다
			//이 값들은 어떻게 정해진것인가? -> 찾아서 레포트 첨부
			lpOutImg[i * rwsize2 + j] = GRAY;
			//만든 밝기값을 lpOutImg의 i+1번째 줄의 j+1번째칸의 픽셀의 데이터를 저장한 배열에 저장한다.
		}
	}

	int MaskSobelX[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };				// X방향 에지강도를 측정하기 위한 소벨마스크
	int MaskSobelY[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };				// Y방향 에지강도를 측정하기 위한 소벨마스크

	float* pImgSobelX, * pImgSobelY, * edgeRoot;						// X방향, Y방향 에지강도와, 두 에지강도의 벡터값을 저장하기 위한 float포인터타입 변수선언
	float sum;															
	float min, max, value;
	float constVal1, constVal2;											// 에지강도의 처리를 위한 변수들 선언.

	pImgSobelX = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));			// plmgSobelX 변수에 lpOutImg 크기의 동적메모리 공간 할당, 배열처럼 사용하며 X 방향에지강도를 저장할 공간임.
	pImgSobelY = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));			// plmgSobelY 변수에 lpOutImg 크기의 동적메모리 공간 할당, 배열처럼 사용하며 Y 방향에지강도를 저장할 공간임.
	edgeRoot = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));				// edgeRoot 변수에 lpOutImg 크기의 동적메모리 공간 할당, 배열처럼 사용하며 에지강도의 벡터합을 저장할 공간임.
	BYTE* lpImg2 = (BYTE*)malloc((rwsize2 + 2) * (hinfo.biHeight + 2));				// 소벨마스킹을 하기위하여 원본 이미지보다 가로 세로로 2줄씩 더 긴 배열에 해당하는 메모리 공간 lpImg2에 할당

	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {							// for문을 통해 할당해준 동적 메모리 공간을 모두 0으로 초기화
			pImgSobelX[i * rwsize2 + j] = 0;
			pImgSobelY[i * rwsize2 + j] = 0;
			edgeRoot[i * rwsize2 + j] = 0;
		}
	}

	for (i = 0; i < hinfo.biHeight + 2; i++) {							
		for (j = 0; j < hinfo.biWidth + 2; j++) {						// for문을 통해 할당한 동적 메모리 공간 lpImg2를 모두 0으로 초기화
			lpImg2[i * (rwsize2 + 2) + j] = (BYTE)0;
		}
	}

	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			lpImg2[i * (rwsize2 + 2) + j] = lpOutImg[(i - 1) * rwsize2 + j - 1];			// lpImg2의 가로세로 끝부분을 제외한 공간, 즉 소벨마스킹을 위한 공간에 흑백처리된 lpOutImg값들을 저장해준다.
		}																					// i와 j값의 조정을 위해 lpOutImg는 [(i - 1) * rwsize2 + j - 1] 와 같이 설정하였다.
	}


	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			sum = 0;																		// sum 값을 0으로 초기화
			for (int ii = 0; ii < 3; ii++) {
				for (int jj = 0; jj < 3; jj++) {											// X소벨마스크가 3*3배열이므로 다음과 같은 for문 작성
					sum += (MaskSobelX[ii][jj] * lpImg2[(i - 1 + ii) * (rwsize2 + 2) + (j - 1 + jj)]);		// 소벨마스크와 lpImg2의 곱셈, 즉 마스크*원본값을 sum에 누계시킨다.
				}
			}
			pImgSobelX[(i - 1) * rwsize2 + (j - 1)] = sum;									// 소벨마스킹을 통해 얻은 값 sum을 X에지 강도 저장을 위한 동적메모리 공간에 저장한다.
		}
	}

	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			sum = 0;																		// sum값을 0으로 다시 초기화
			for (int ii = 0; ii < 3; ii++) {
				for (int jj = 0; jj < 3; jj++) {											// Y소벨 마스크 역시 3*3배열이므로 다음과 같이 for문 작성
					sum += (MaskSobelY[ii][jj] * lpImg2[(i - 1 + ii) * (rwsize2 + 2) + (j - 1 + jj)]);		// X소벨 마스킹과 똑같은 동작을 수행하여 sum에 누계
				}
			}
			pImgSobelY[(i - 1) * rwsize2 + (j - 1)] = sum;									// 소벨마스킹을 통해 얻은 값 sum을 Y에지 강도 저장을 위한 동적메모리 공간에 저장
		}
	}

	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			constVal1 =pImgSobelX[i * rwsize2 + j];					// X에지 강도를 constVal1 변수에 저장
			constVal2 = pImgSobelY[i * rwsize2 + j];				// Y에지 강도를 constVal2 변수에 저장
			value = sqrt((constVal1 * constVal1) + (constVal2 * constVal2));		// constVal 변수를 이용하여 X에지강도와 Y에지강도의 벡터값을 value 변수에 저장
			edgeRoot[i * rwsize2 + j] = value;						// value 변수를 이용하여 벡터값을 edgeRoot 메모리 공간에 저장
		}
	}

	min = 100000.0;									// 최소값을 찾기 위한 변수 min
	max = -100000.0;								// 최대값을 찾기 위한 변수 max
	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			value = edgeRoot[i * rwsize2 + j];		// value에 아까 구한 벡터값을 저장
			if (value < min) min = value;			// value 보다 min이 크면 min값을 value로 변경 ,, 벡터값들 중 최소값을 설정하는 과정
			if (value > max) max = value;			// value 가 max보다 크면 max값을 value로 변경 ,, 벡터값들 중 최대값을 설정하는 과정
		}
	}					// for문이 끝났을 때 min값은 최소 , max값은 최대가 된다.


	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			value = (edgeRoot[i * rwsize2 + j] - min) * 400.0 / (max - min);	// 실제 저장하여 출력할 때 BYTE 타입이기 때문에 255byte 내로 재계산을 진행하여야 한다. 400정도를 넣었을 때 선명한 에지가 저장됨
			lpOutImg[i * rwsize2 + j] = (BYTE)value;							// 재계산 한 value를 BYTE 타입으로 형변환 후 출력을 위한 lpOutImg 에 저장한다.
		}
	}

	ohf.bfOffBits += 1024;
	//bfOffBits는 실제 파일의 화면 데이터가 시작되는 곳의 위치이다.
	//비트맵파일에서 비트수가 낮으면(1 ~ 8BPP)
	//비트수가 높을 때와(8BPP 초과) 달리 ColorTable이 파일의 화면 데이터앞에 있다.
	//따라서 bfOffBits를 1024만큼 뒤로 밀어야 한다.
	//1024는 RGBQUAD의 크기(4바이트) * 256개 팔레트 = 1024다.

	ohinfo.biBitCount = 8;
	//8BPP이니까 8이다.

	pRGB = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);
	//앞서 말한 ColorTable을 위한 메모리를 설정한다.

	for (i = 0; i < 256; i++)
	{
		pRGB[i].rgbBlue = i;
		pRGB[i].rgbGreen = i;
		pRGB[i].rgbRed = i;
		pRGB[i].rgbReserved = i;
	}
	//흑백이니까 그냥 0부터 255까지 순차적으로 저장한다.

	file = fopen("oriedge.bmp", "wb");
	//이제 출력할 파일을 쓰기모드로 연다.

	fwrite(&ohf, sizeof(char), sizeof(BITMAPFILEHEADER), file);
	fwrite(&ohinfo, sizeof(char), sizeof(BITMAPINFOHEADER), file);
	fwrite(pRGB, sizeof(RGBQUAD), 256, file);
	//비트맵 파일을 구성하는 BITMAPFILEHEADER, BITMAPINFOHEADER,RGBQUAD를 기록한다.
	fwrite(lpOutImg, sizeof(char), rwsize2 * hinfo.biHeight, file);
	//앞서 우리가 만든 출력 데이터를 기록한다.
	fclose(file);
	//파일을 닫는다.

	free(lpImg);
	free(lpOutImg);
	free(pRGB);
	free(pImgSobelX);
	free(pImgSobelY);
	free(edgeRoot);
	free(lpImg2);
	//메모리를 반환한다.

	printf("Check oriedge.bmp!!\n\n");
}
