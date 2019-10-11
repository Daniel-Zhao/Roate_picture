#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct a {
	WORD B;
	WORD G;
	WORD R;
}A;
void xuanzhuan(int flag, int h, int w, BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, FILE* fp)
{
	A** PH;
	int i, j, temp;
	unsigned char A1;
	PH = (A**)malloc(h * sizeof(A*));
	PH[0] = (A*)malloc(w * h * sizeof(A));
	for (i = 1; i < h; i++)
	{
		PH[i] = PH[i - 1] + w;
	}
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{

			fread(&A1, sizeof(unsigned char), 1, fp);
			PH[h - 1 - i][j].B = A1;
			fread(&A1, sizeof(unsigned char), 1, fp);
			PH[h - 1 - i][j].G = A1;
			fread(&A1, sizeof(unsigned char), 1, fp);
			PH[h - 1 - i][j].R = A1;
		}
		if (flag != 0)
		{
			for (j = 0; j < flag; j++)
				fread(&A1, sizeof(unsigned char), 1, fp);
		}
	}

	FILE* fp1;
	if ((fp1 = fopen("tr1.bmp", "wb")) == NULL)
	{
		printf("没有这张图片\n");
		exit(0);
	}

	flag = (3 * h) % 4;
	if (flag != 0)
	{
		flag = 4 - flag;
	}
	infoHeader.biSizeImage = w * 3 * (h + flag) * 1;
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp1);
	temp = infoHeader.biHeight;
	infoHeader.biHeight = infoHeader.biWidth;
	infoHeader.biWidth = temp;
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp1);
	BYTE p = 0;
	for (i = 0; i < w; i++)
	{
		for (j = 0; j < h; j++)
		{
			A1 = PH[h - j - 1][w - i - 1].B;
			fwrite(&A1, sizeof(unsigned char), 1, fp1);
			A1 = PH[h - j - 1][w - i - 1].G;
			fwrite(&A1, sizeof(unsigned char), 1, fp1);
			A1 = PH[h - j - 1][w - i - 1].R;
			fwrite(&A1, sizeof(unsigned char), 1, fp1);
		}
		if (flag != 0)
		{
			for (j = 0; j < flag; j++)
			{
				fwrite(&p, sizeof(BYTE), 1, fp1);
			}
		}
	}
	ShellExecute(0, "open", "tr.bmp", 0, 0, 0);
	ShellExecute(0, "open", "tr1.bmp", 0, 0, 0);
	fclose(fp1);
}
int main()
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	int w, h;
	FILE* fp;
	if ((fp = fopen("tr.bmp", "rb")) == NULL)
	{
		printf("无法打开图片文件\n");
	}
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (infoHeader.biBitCount == 24)
	{
		int flag;
		w = infoHeader.biWidth;
		h = infoHeader.biHeight;
		flag = (3 * w) % 4;
		if (flag != 0)
		{
			flag = 4 - flag;
		}
		xuanzhuan(flag, h, w, fileHeader, infoHeader, fp);
	}
	printf("文件旋转成功!\n");
	return 0;
}
