// ������������ ���� ������������ ��� ���� ����� ������� ����� ��������� �������������� � ������ .cpp

#pragma once		// ��������� ������� ������ �������� ���������� ��������� ������������� �����

void Buffer_Send();
void Buffer_SetPixel(HDC hdc, int x, int y);
void Buffer_RemovePixel(HDC hdc, int x, int y);
void Buffer_SetLine(HDC hdc, float xa, float ya, float xb, float yb);
void Buffer_SetTriangle(HDC hdc, float xa, float ya, float xb, float yb, float xc, float yc);


