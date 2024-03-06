#include <Windows.h>
#include <iostream>
#include <math.h>
#include "ssd1306.h"

using namespace std;

void Buffer_Send()
{
	Sleep(10);
}


void Buffer_SetPixel(HDC hdc, int x, int y)
{
    const int N = 6;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            SetPixel(hdc, (x * N) + i, (y * N) + j, RGB(174, 255, 255));
        }
    }
}

void Buffer_RemovePixel(HDC hdc, int x, int y)
{
    const int N = 6;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            SetPixel(hdc, (x * N) + i, (y * N) + j, RGB(12, 12, 12));
        }
    }
}

void Buffer_SetLine(HDC hdc, float xa, float ya, float xb, float yb)
{

	//////////////////////////////////////////////////////////////////////////
	// ������ ������ ���� ����� ������������
	if (xa == xb)
	{
		if (yb <= ya)
		{
			int ytemp = ya;
			ya = yb;
			yb = ytemp;
		}

		int x = xa;

		for (int y = ya; y < yb; y++)
		{
			Buffer_SetPixel(hdc, x, y);
		}

		return;
	}


	//////////////////////////////////////////////////////////////////////////
	// ������ ������ ���� xa > xb - ����� ���� ����������� �� �������
	if (xb < xa)
	{
		float xtemp;
		float ytemp;

		xtemp = xa;
		xa = xb;
		xb = xtemp;

		ytemp = ya;
		ya = yb;
		yb = ytemp;
	}

	// ���������� �������
	for (float x = xa; x <= xb; x += 1)
	{
		float temp = (x - xa) * (yb - ya) + ya * (xb - xa);		// ��� temp ������-�� �� ��������. �� ���������� ������� ���� �������, �������� ������� �� 2
		int y = temp / (xb - xa);								// -1 � +0.9 - ������������ ��������, � ���� �������� ������ ����������� ������������ ������
		Buffer_SetPixel(hdc, x, y);
	}
}

void Buffer_SetTriangle(HDC hdc, float xa, float ya, float xb, float yb, float xc, float yc)
{
	Buffer_SetLine(hdc, xa, ya, xb, yb);
	Buffer_SetLine(hdc, xb, yb, xc, yc);
	Buffer_SetLine(hdc, xc, yc, xa, ya);
}

