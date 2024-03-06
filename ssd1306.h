// Заголовочный файл предназначен для того чтобы хранить здесь интерфейс взаимодействия с файлом .cpp

#pragma once		// Директива которая решает проблему повторного включения заголовочного файла

void Buffer_Send();
void Buffer_SetPixel(HDC hdc, int x, int y);
void Buffer_RemovePixel(HDC hdc, int x, int y);
void Buffer_SetLine(HDC hdc, float xa, float ya, float xb, float yb);
void Buffer_SetTriangle(HDC hdc, float xa, float ya, float xb, float yb, float xc, float yc);


