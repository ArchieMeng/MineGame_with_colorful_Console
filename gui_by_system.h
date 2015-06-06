/*1452286数-朱可仁*/
/*
*绘图接口：
drawMainMenu:绘制主界面
drawSingleBoard:绘制单人界面
drawNum:绘制单个数字
*/
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <cctype>
#include <cmath>
#pragma comment(lib,"winmm.lib")
using namespace std;

//#define DEBUG
#define CONTINUE 0
#define WIN 1
#define FAIL 2
#define OVER 3
#define WHITE 15
#define YELLOW 14
#define BLACK 0
#define RED 12
#define GRAY 8
#define NUM_WIDTH 6
#define NUM_HEIGHT 5
#define getx(x) (2*(NUM_WIDTH+1)*(x)+2)
#define gety(y) ((NUM_HEIGHT+1)*(y)+2)
#define isincellx(sx) !(((sx)-2)%(2*(NUM_WIDTH+1)))
#define isincelly(sy) !(((sy)-2)%(NUM_HEIGHT+1))
#define NUMBER_0 0x30
#define NUMBER_9 0x39
#define MAX_WIDTH 10
#define MAX_HEIGHT 8
#define max(a,b) ((a)>(b)?(a):(b))
#define MAX_RAND 32768
#define DOUBLE_WIDTH 4
#define DOUBLE_HEIGHT 4
#define BOTH_OVER -1
#define INF 1e7

void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

void setcolor(HANDLE hout, const int bg_color, const int fg_color)
{
	static int crt_bg_color = BLACK, crt_fg_color = WHITE;
	if (bg_color != crt_bg_color || fg_color != crt_fg_color)
	{
		crt_bg_color = bg_color;
		crt_fg_color = fg_color;
		SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
	}
}

void showstr(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color, const char s[], bool fDouble = false)
{
	gotoxy(hout, X + fDouble * 2 * DOUBLE_WIDTH * (NUM_WIDTH + 1), Y);
	setcolor(hout, bg_color, fg_color);

	printf("%s", s);
}

void shownum(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color, int num)
{
	gotoxy(hout, X, Y);
	setcolor(hout, bg_color, fg_color);
	printf("%d", num);
}

void setConsoleSize(int cols, int lines)
{
	char s[255];
	sprintf(s, "mode con cols=%d lines=%d", cols, lines);
	system(s);
}

/*
*打印格子(单人)
*宽:6个双字节,高:5字节
*边框1字节
*第i列(i从0始),j行格子左上角屏幕坐标
*x = 14*i+2, y = 5*j+2
*/
void drawSingleBoard(HANDLE hout, int width, int height)
{
	int Cols = width * 14 + 2, Lines = height * 6 + 2;

	setConsoleSize(Cols, Lines + 10);

	showstr(hout, 0, 1, WHITE, BLACK, "╔");
	for (int i = 2; i < Cols - 2; i += 2)
		if (i % 14 == 0)
			showstr(hout, i, 1, WHITE, BLACK, "╤");
		else
			showstr(hout, i, 1, WHITE, BLACK, "═");
	showstr(hout, Cols - 2, 1, WHITE, BLACK, "╗");

	for (int j = 1; j < Lines - 2; j++)
		if (j % 6 == 0)
		{
			showstr(hout, 0, j + 1, WHITE, BLACK, "╟");
			for (int i = 2; i < Cols - 2; i += 2)
				if (i % 14 == 0)
					showstr(hout, i, j + 1, WHITE, BLACK, "┼");
				else
					showstr(hout, i, j + 1, WHITE, BLACK, "─");
			showstr(hout, Cols - 2, j + 1, WHITE, BLACK, "╢");
		}
		else
		{
			showstr(hout, 0, j + 1, WHITE, BLACK, "║");
			for (int i = 2; i < Cols - 2; i += 2)
				if (i % 14 == 0)
					showstr(hout, i, j + 1, WHITE, BLACK, "│");
			showstr(hout, Cols - 2, j + 1, WHITE, BLACK, "║");
		}

	showstr(hout, 0, Lines - 1, WHITE, BLACK, "╚");
	for (int i = 2; i < Cols - 2; i += 2)
		if (i % 14 == 0)
			showstr(hout, i, Lines - 1, WHITE, BLACK, "╧");
		else
			showstr(hout, i, Lines - 1, WHITE, BLACK, "═");
	showstr(hout, Cols - 2, Lines - 1, WHITE, BLACK, "╝");
}

void drawMainMenu(HANDLE hout)
{
	const int Lines = 25, Cols = 80;

	setcolor(hout, WHITE, BLACK);
	system("cls");
	setConsoleSize(Cols, Lines + 1);

	showstr(hout, 0, 0, WHITE, BLACK, "╔");
	for (int i = 2; i < Cols - 2; i += 2)
		showstr(hout, i, 0, WHITE, BLACK, "═");
	showstr(hout, Cols - 2, 0, WHITE, BLACK, "╗");

	for (int j = 1; j < Lines - 2; j++)
	{
		showstr(hout, 0, j, WHITE, BLACK, "║");
		/*for (int i = 2; i < Cols - 2; i++)
			showstr(hout, i, j, WHITE, BLACK, " ");*/
		showstr(hout, Cols - 2, j, WHITE, BLACK, "║");
	}

	showstr(hout, 0, Lines - 2, WHITE, BLACK, "╚");
	for (int i = 2; i < Cols - 2; i += 2)
		showstr(hout, i, Lines - 2, WHITE, BLACK, "═");
	showstr(hout, Cols - 2, Lines - 2, WHITE, BLACK, "╝");
}


void drawNum(HANDLE hout, int x, int y, int num, int color)
{
	int sx, sy;//左上角坐标

		sx = getx(x);
		sy = gety(y);

	int bg_color = color;
	//if(bg_color ==)

	showstr(hout, sx, sy, bg_color, BLACK, "╔");
	for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
		showstr(hout, sx + i, sy, bg_color, BLACK, "═");
	showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy, bg_color, BLACK, "╗");

	for (int j = 1; j < NUM_HEIGHT; j++)
	{
		showstr(hout, sx, sy + j, bg_color, BLACK, "║");
		for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
			showstr(hout, sx + i, sy + j, bg_color, BLACK, "　");
		showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy + j, bg_color, BLACK, "║");
	}

	showstr(hout, sx, sy + NUM_HEIGHT - 1, bg_color, BLACK, "╚");
	for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
		showstr(hout, sx + i, sy + NUM_HEIGHT - 1, bg_color, BLACK, "═");
	showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy + NUM_HEIGHT - 1, bg_color, BLACK, "╝");

	gotoxy(hout, sx + NUM_WIDTH / 2 - log((double)num) / 2 + 2, sy + NUM_HEIGHT / 2);
	printf("%d", num);
}

void drawNum(HANDLE hout, int x, int y, char sign, int color )
{
	int sx, sy;//左上角坐标

		sx = getx(x);
		sy = gety(y);

	int bg_color = color;
	//if(bg_color ==)

	showstr(hout, sx, sy, bg_color, BLACK, "╔");
	for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
		showstr(hout, sx + i, sy, bg_color, BLACK, "═");
	showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy, bg_color, BLACK, "╗");

	for (int j = 1; j < NUM_HEIGHT; j++)
	{
		showstr(hout, sx, sy + j, bg_color, BLACK, "║");
		for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
			showstr(hout, sx + i, sy + j, bg_color, BLACK, "　");
		showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy + j, bg_color, BLACK, "║");
	}

	showstr(hout, sx, sy + NUM_HEIGHT - 1, bg_color, BLACK, "╚");
	for (int i = 2; i < 2 * (NUM_WIDTH - 1); i += 2)
		showstr(hout, sx + i, sy + NUM_HEIGHT - 1, bg_color, BLACK, "═");
	showstr(hout, sx + 2 * (NUM_WIDTH - 1), sy + NUM_HEIGHT - 1, bg_color, BLACK, "╝");

	gotoxy(hout, sx + NUM_WIDTH / 2 - log((double)sign) / 2 + 2, sy + NUM_HEIGHT / 2);
	printf("%c", sign);
}

/*
int getNum()
{
	int val = 0;

	while (1)
	{
		int key = getKey();
		if (key >= NUMBER_0 && key <= NUMBER_9 && val <= 1e6)
		{
			val = val * 10 + key - NUMBER_0;
			printf("%d", key - NUMBER_0);
		}
		else if (key == VK_BACK && val > 0)
		{
			val /= 10;
			printf("\b \b");
		}
		else if (key == VK_RETURN)
			return val;
	}
}*/




