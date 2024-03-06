#include <Windows.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "ssd1306.h"
using namespace std;




struct vec3d					// Структура точек углов треугольника
{
	float x, y, z;
};

struct triangle					// Структура треугольника
{
	vec3d p[3];
};

struct mesh						// Структура конечного объекта состоящего из треугольников
{
	vector<triangle> tris;
};

struct mat4x4					// Структура матрицы для проекции
{
	float m[4][4] = { 0 };
};

// Функция перемножения матриц
void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
{
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}
};


float fTheta;


int main()
{
    system("mode con cols=96 lines=24");        // Определение размера окошка
    system("title Имитатор ssd1306");
    CONSOLE_CURSOR_INFO inf = { 0 };            // Отключение курсора
    inf.dwSize = 1;                             
    inf.bVisible = false;                        
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    HWND myConsole = GetConsoleWindow();        // 3:30 "its gonna get your main console .. this variable is your console"
    HDC mdc = GetDC(myConsole);                 // 4:05 "this is basicaly like a display variable or display function"
	for (int i = 0; i < 100; i++)				// Это костыль. Без него почему-то первые ~10-20 точек удаляются
	{
		Buffer_RemovePixel(mdc, 0, 0);
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////   Собственно код   /////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	// Инициализация куба и матрицы для проекции
	mesh meshCube;
	mat4x4 matProj;


	// Pаполнение куба треугольниками
	meshCube.tris = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
	};

	// Projection matrix
	float fzNear = 0.1f;												// znear
	float fzFar = 1000.0f;												// zfar
	float fFov = 90.0f;													// theta (FOV)
	float fAspectRatio = (float)64 / (float)128;						// a = h/w
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);		// f = 1/(tan(theta/2))
		// Заполнение матрицы
	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fzFar / (fzFar - fzNear);
	matProj.m[3][2] = (-fzFar * fzNear) / (fzFar - fzNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;


	// Set up rotation matrices
	mat4x4 matRotZ, matRotX;
	fTheta++;

	// Rotation Z
	matRotZ.m[0][0] = cosf(fTheta);
	matRotZ.m[0][1] = sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] = cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(fTheta * 0.5f);
	matRotX.m[1][2] = sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] = cosf(fTheta * 0.5f);
	matRotX.m[3][3] = 1;



	// Нарисовать треугольники
	for (auto tri : meshCube.tris)
	{
		triangle triProjected, triTranslated;

		// Offset into the screen
		triTranslated = tri;
		triTranslated.p[0].z = tri.p[0].z + 1.0f;
		triTranslated.p[1].z = tri.p[1].z + 1.0f;
		triTranslated.p[2].z = tri.p[2].z + 1.0f;

		// Project triangles from 3D --> 2D
		MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
		MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
		MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
		
		// Scale into view
		triProjected.p[0].x += 1.0f;
		triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f;
		triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f;
		triProjected.p[2].y += 1.0f;
		triProjected.p[0].x *= 0.5f * (float)128.0;
		triProjected.p[0].y *= 0.5f * (float)64.0;
		triProjected.p[1].x *= 0.5f * (float)128.0;
		triProjected.p[1].y *= 0.5f * (float)64.0;
		triProjected.p[2].x *= 0.5f * (float)128.0;
		triProjected.p[2].y *= 0.5f * (float)64.0;

		// Отрисовка проекции
		Buffer_SetTriangle(mdc, triProjected.p[0].x, triProjected.p[0].y,
			triProjected.p[1].x, triProjected.p[1].y,
			triProjected.p[2].x, triProjected.p[2].y);
	}





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////   Конец   ///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cin.ignore();
	ReleaseDC(myConsole, mdc);

    return 0;
}





