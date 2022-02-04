#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // ��Ʈ�� ���� �԰ݿ���, ���ι��� ������ ũ��� 4�� ���(����Ʈ����)�� �Ѵ�.
// ��?? -> ������ ã�Ƽ� ����Ʈ�� ÷��
#define BYTE unsigned char

FILE* file;// ���� ������ ����
int rwsize, rwsize2;// ���� ���� �� ����Ʈ ��
BYTE* lpImg;// �Է� �����Ͱ� ����Ǵ� �޸� �ּҸ� �����ϴ� ������
BYTE* lpOutImg;// ��� �����Ͱ� ����Ǵ� �޸� �ּҸ� �����ϴ� ������
RGBQUAD* pRGB;// Lookup Table
BITMAPFILEHEADER hf;// ��Ʈ�� ���� �ش�
BITMAPINFOHEADER hinfo;// ��Ʈ�� ���� ���

void egdt(void); // ����!!

int main()
{

	egdt();

	return 0;
}


void egdt(void)
{
	int i, j;
	BYTE R, G, B, GRAY;

	BITMAPFILEHEADER ohf; // ��� ������ ���� ��Ʈ�� ���� ���
	BITMAPINFOHEADER ohinfo; // ��� ������ ���� ��Ʈ�� ���� ���� ���

	printf("RUN egdt()\n\n");

	file = fopen("ori.bmp", "rb");// ������ �б� ���� ��

	if (file == NULL)// ���� ���⿡ �����ϸ�
	{
		printf("Error At File open!!!\n");
		exit(1);
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // ������ ���� ������ BITMAPFILEHEADER��ŭ�� hf�� ����

	if (hf.bfType != 0x4D42) // 0X4D42 == BM(�ƽ�Ű �ڵ�), ��Ʈ�������� �� ó�� ������ BM�̴�. ���� ù �κ��� �а�, �� �κ��� �м��ؼ� ��Ʈ���� �ƴϸ�...
	{
		printf("File is NOT BMP\n");
		exit(1);
	}

	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, file); // ������ ���� ������ BITMAPINFOHEADER��ŭ�� hinfo�� ����

	printf("File size : %d\n", hf.bfSize); // �Է¹��� ��Ʈ�� ������ ũ�⸦ ȭ�鿡 �����
	printf("offset : %d\n", hf.bfOffBits); // �Է¹��� ��Ʈ�� ���Ͽ��� ���� �����Ͱ� �����ϴ� ������
	printf("Image Size : (%dX%d)\n", hinfo.biWidth, hinfo.biHeight); // �Է¹��� ��Ʈ�� ������ �ȼ� ������ ȭ�鿡 �����
	printf("%d BPP \n", hinfo.biBitCount); // �Է¹��� ��Ʈ�� ������ �ȼ��� ��Ʈ���� ���

	ohf = hf;
	ohinfo = hinfo;
	// ���� ���ϰ� ��� ������ ���� �����ϹǷ� ��������� �����ؼ� ����Ѵ�.
	// ��, �Ϻ� ���� �ٲ�� �͵��� �����Ƿ�, ���߿� �ݵ�� ��������� �Ѵ�.(bfOffBits, biBitCount ��...)

	rwsize = WIDTHBYTES(hinfo.biBitCount * hinfo.biWidth);
	rwsize2 = WIDTHBYTES(8 * ohinfo.biWidth);
	//�Է� ���ϰ� ��� ���Ͽ��� ���� �� �ٴ� �����Ͱ� ���Ʈ���� �����Ѵ�
	//�̶� �Է����Ͽ��� ��������(=�������� �ִ� �� �ȼ� ����)
	//�� �ȼ��� ����Ʈ ���� ���ϸ�, �� ���� �� ��Ʈ�� �����ϴ��� �� �� �ִ�.
	//rwsize2�� ����� ���� �Է� ���ϰ� ��� ������ �ȼ� ������� �����ϱ� ������ �Է� ������ �������� �״�� ����
	//8BPP ������ �ȼ��� ��Ʈ���� 8��Ʈ�̹Ƿ� 8�� ���Ѵ�.

	fseek(file, hf.bfOffBits, SEEK_SET);
	//��Ʈ�� ���Ͽ��� ���� ������ ����� �ִ� ��ġ�� �����͸� �̵��Ѵ�

	lpImg = (BYTE*)malloc(rwsize * hinfo.biHeight);
	//�Է� ���� ������ ���� �޸𸮸� �Ҵ��Ѵ�

	fread(lpImg, sizeof(char), rwsize * hinfo.biHeight, file);
	//�����ʹ� ���� ���� ������ ���� ����� ���� �� ó������ �̵��� ��Ȳ�̴�. �̶� �̹����� ��������(=�������� �ִ� �� �ȼ� ����)
	//�� �ٴ� ��Ʈ���� ���ϸ�, ��ü �������� ũ�Ⱑ ���´�. fread�Լ��� �̿��� �̹����� �����͸� char������ ũ�⸸ŭ
	//������ �����Ѵ�.

	fclose(file);
	//���ϻ���� �Ϸ�Ǿ����Ƿ� �ݴ´�.

	lpOutImg = (BYTE*)malloc(rwsize2 * ohinfo.biHeight);
	//����� ������ ���� �޸𸮸� �Ҵ��Ѵ�

	for (i = 0; i < ohinfo.biHeight; i++)
	{
		for (j = 0; j < ohinfo.biWidth; j++)
		{
			B = lpImg[i * rwsize + 3 * j + 0];
			G = lpImg[i * rwsize + 3 * j + 1];
			R = lpImg[i * rwsize + 3 * j + 2];
			//�ռ� ����ߵ� rwsize�� ���� ������ ������ ũ���. ���� ���⿡ i�� ���Ѵٴ°��� i+1��° ���� �����͸�
			//������� �ǹ��Ѵ�. ���� �÷����� �� �ȼ��� �����ʹ� 3����Ʈ�� �ʿ��ϹǷ� 3*j�� �ؼ� j+1��° �ȼ��� �̵��Ѵ�.
			//�� ���� ������ ����  i+1��° ���� j+1��°ĭ�� �ȼ��� �����͸� ������ �迭�� ���� Ȯ���ϰ� �ȴ�.
			//�̶� �迭�� ��ġ�� +0,+1,+2�� �ؼ� �������� B G R���� ���� �����Ѵ�.

			GRAY = (BYTE)(0.299 * R + 0.587 * G + 0.114 * B);
			//���� ������ R,G,B���� ������ ���� ���ϰ� ���ؼ� ��Ⱚ�� �����
			//�� ������ ��� ���������ΰ�? -> ã�Ƽ� ����Ʈ ÷��
			lpOutImg[i * rwsize2 + j] = GRAY;
			//���� ��Ⱚ�� lpOutImg�� i+1��° ���� j+1��°ĭ�� �ȼ��� �����͸� ������ �迭�� �����Ѵ�.
		}
	}

	int MaskSobelX[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };				// X���� ���������� �����ϱ� ���� �Һ�����ũ
	int MaskSobelY[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };				// Y���� ���������� �����ϱ� ���� �Һ�����ũ

	float* pImgSobelX, * pImgSobelY, * edgeRoot;						// X����, Y���� ����������, �� ���������� ���Ͱ��� �����ϱ� ���� float������Ÿ�� ��������
	float sum;															
	float min, max, value;
	float constVal1, constVal2;											// ���������� ó���� ���� ������ ����.

	pImgSobelX = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));			// plmgSobelX ������ lpOutImg ũ���� �����޸� ���� �Ҵ�, �迭ó�� ����ϸ� X ���⿡�������� ������ ������.
	pImgSobelY = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));			// plmgSobelY ������ lpOutImg ũ���� �����޸� ���� �Ҵ�, �迭ó�� ����ϸ� Y ���⿡�������� ������ ������.
	edgeRoot = (float*)malloc(rwsize2 * hinfo.biHeight*sizeof(float));				// edgeRoot ������ lpOutImg ũ���� �����޸� ���� �Ҵ�, �迭ó�� ����ϸ� ���������� �������� ������ ������.
	BYTE* lpImg2 = (BYTE*)malloc((rwsize2 + 2) * (hinfo.biHeight + 2));				// �Һ�����ŷ�� �ϱ����Ͽ� ���� �̹������� ���� ���η� 2�پ� �� �� �迭�� �ش��ϴ� �޸� ���� lpImg2�� �Ҵ�

	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {							// for���� ���� �Ҵ����� ���� �޸� ������ ��� 0���� �ʱ�ȭ
			pImgSobelX[i * rwsize2 + j] = 0;
			pImgSobelY[i * rwsize2 + j] = 0;
			edgeRoot[i * rwsize2 + j] = 0;
		}
	}

	for (i = 0; i < hinfo.biHeight + 2; i++) {							
		for (j = 0; j < hinfo.biWidth + 2; j++) {						// for���� ���� �Ҵ��� ���� �޸� ���� lpImg2�� ��� 0���� �ʱ�ȭ
			lpImg2[i * (rwsize2 + 2) + j] = (BYTE)0;
		}
	}

	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			lpImg2[i * (rwsize2 + 2) + j] = lpOutImg[(i - 1) * rwsize2 + j - 1];			// lpImg2�� ���μ��� ���κ��� ������ ����, �� �Һ�����ŷ�� ���� ������ ���ó���� lpOutImg������ �������ش�.
		}																					// i�� j���� ������ ���� lpOutImg�� [(i - 1) * rwsize2 + j - 1] �� ���� �����Ͽ���.
	}


	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			sum = 0;																		// sum ���� 0���� �ʱ�ȭ
			for (int ii = 0; ii < 3; ii++) {
				for (int jj = 0; jj < 3; jj++) {											// X�Һ�����ũ�� 3*3�迭�̹Ƿ� ������ ���� for�� �ۼ�
					sum += (MaskSobelX[ii][jj] * lpImg2[(i - 1 + ii) * (rwsize2 + 2) + (j - 1 + jj)]);		// �Һ�����ũ�� lpImg2�� ����, �� ����ũ*�������� sum�� �����Ų��.
				}
			}
			pImgSobelX[(i - 1) * rwsize2 + (j - 1)] = sum;									// �Һ�����ŷ�� ���� ���� �� sum�� X���� ���� ������ ���� �����޸� ������ �����Ѵ�.
		}
	}

	for (i = 1; i < hinfo.biHeight + 1; i++) {
		for (j = 1; j < hinfo.biWidth + 1; j++) {
			sum = 0;																		// sum���� 0���� �ٽ� �ʱ�ȭ
			for (int ii = 0; ii < 3; ii++) {
				for (int jj = 0; jj < 3; jj++) {											// Y�Һ� ����ũ ���� 3*3�迭�̹Ƿ� ������ ���� for�� �ۼ�
					sum += (MaskSobelY[ii][jj] * lpImg2[(i - 1 + ii) * (rwsize2 + 2) + (j - 1 + jj)]);		// X�Һ� ����ŷ�� �Ȱ��� ������ �����Ͽ� sum�� ����
				}
			}
			pImgSobelY[(i - 1) * rwsize2 + (j - 1)] = sum;									// �Һ�����ŷ�� ���� ���� �� sum�� Y���� ���� ������ ���� �����޸� ������ ����
		}
	}

	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			constVal1 =pImgSobelX[i * rwsize2 + j];					// X���� ������ constVal1 ������ ����
			constVal2 = pImgSobelY[i * rwsize2 + j];				// Y���� ������ constVal2 ������ ����
			value = sqrt((constVal1 * constVal1) + (constVal2 * constVal2));		// constVal ������ �̿��Ͽ� X���������� Y���������� ���Ͱ��� value ������ ����
			edgeRoot[i * rwsize2 + j] = value;						// value ������ �̿��Ͽ� ���Ͱ��� edgeRoot �޸� ������ ����
		}
	}

	min = 100000.0;									// �ּҰ��� ã�� ���� ���� min
	max = -100000.0;								// �ִ밪�� ã�� ���� ���� max
	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			value = edgeRoot[i * rwsize2 + j];		// value�� �Ʊ� ���� ���Ͱ��� ����
			if (value < min) min = value;			// value ���� min�� ũ�� min���� value�� ���� ,, ���Ͱ��� �� �ּҰ��� �����ϴ� ����
			if (value > max) max = value;			// value �� max���� ũ�� max���� value�� ���� ,, ���Ͱ��� �� �ִ밪�� �����ϴ� ����
		}
	}					// for���� ������ �� min���� �ּ� , max���� �ִ밡 �ȴ�.


	for (i = 0; i < hinfo.biHeight; i++) {
		for (j = 0; j < hinfo.biWidth; j++) {
			value = (edgeRoot[i * rwsize2 + j] - min) * 400.0 / (max - min);	// ���� �����Ͽ� ����� �� BYTE Ÿ���̱� ������ 255byte ���� ������ �����Ͽ��� �Ѵ�. 400������ �־��� �� ������ ������ �����
			lpOutImg[i * rwsize2 + j] = (BYTE)value;							// ���� �� value�� BYTE Ÿ������ ����ȯ �� ����� ���� lpOutImg �� �����Ѵ�.
		}
	}

	ohf.bfOffBits += 1024;
	//bfOffBits�� ���� ������ ȭ�� �����Ͱ� ���۵Ǵ� ���� ��ġ�̴�.
	//��Ʈ�����Ͽ��� ��Ʈ���� ������(1 ~ 8BPP)
	//��Ʈ���� ���� ����(8BPP �ʰ�) �޸� ColorTable�� ������ ȭ�� �����;տ� �ִ�.
	//���� bfOffBits�� 1024��ŭ �ڷ� �о�� �Ѵ�.
	//1024�� RGBQUAD�� ũ��(4����Ʈ) * 256�� �ȷ�Ʈ = 1024��.

	ohinfo.biBitCount = 8;
	//8BPP�̴ϱ� 8�̴�.

	pRGB = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);
	//�ռ� ���� ColorTable�� ���� �޸𸮸� �����Ѵ�.

	for (i = 0; i < 256; i++)
	{
		pRGB[i].rgbBlue = i;
		pRGB[i].rgbGreen = i;
		pRGB[i].rgbRed = i;
		pRGB[i].rgbReserved = i;
	}
	//����̴ϱ� �׳� 0���� 255���� ���������� �����Ѵ�.

	file = fopen("oriedge.bmp", "wb");
	//���� ����� ������ ������� ����.

	fwrite(&ohf, sizeof(char), sizeof(BITMAPFILEHEADER), file);
	fwrite(&ohinfo, sizeof(char), sizeof(BITMAPINFOHEADER), file);
	fwrite(pRGB, sizeof(RGBQUAD), 256, file);
	//��Ʈ�� ������ �����ϴ� BITMAPFILEHEADER, BITMAPINFOHEADER,RGBQUAD�� ����Ѵ�.
	fwrite(lpOutImg, sizeof(char), rwsize2 * hinfo.biHeight, file);
	//�ռ� �츮�� ���� ��� �����͸� ����Ѵ�.
	fclose(file);
	//������ �ݴ´�.

	free(lpImg);
	free(lpOutImg);
	free(pRGB);
	free(pImgSobelX);
	free(pImgSobelY);
	free(edgeRoot);
	free(lpImg2);
	//�޸𸮸� ��ȯ�Ѵ�.

	printf("Check oriedge.bmp!!\n\n");
}
