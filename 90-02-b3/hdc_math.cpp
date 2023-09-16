/* 2152046 程瑞真 大数据 */
#include<iostream>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include"../include/cmd_hdc_tools.h"
using namespace std;

static const int color = RGB(0,255,255);        //青色
static const int width = 2;                     //雪花边界宽度定为2
const int   win_width = 800, win_high = 800;	//设定屏幕宽度
const int   win_bgcolor = RGB(204, 204, 204);   //屏幕背景色,由word取色器取得
static const double PI = 3.14159;
static int base_x = 400;
static int base_y = 300;

/* Koch曲线 */
static void koch(int x0, int y0, int x1, int y1, int k, int color, int width)
{
	int x2, y2, x3, y3, x4, y4;
	x2 = (int)(2.0 / 3 * x0 + 1.0 / 3 * x1);
	y2 = (int)(2.0 / 3 * y0 + 1.0 / 3 * y1);
	x3 = (int)(1.0 / 3 * x0 + 2.0 / 3 * x1);
	y3 = (int)(1.0 / 3 * y0 + 2.0 / 3 * y1);
	x4 = (int)(1.0 / 2 * (x0 + x1) - sqrt(3.0) / 6 * (y1 - y0));
	y4 = (int)(1.0 / 2 * (y0 + y1) + sqrt(3.0) / 6 * (x1 - x0));
	if (k > 1)
	{
		koch(x0, y0, x2, y2, k - 1, color, width);
		koch(x2, y2, x4, y4, k - 1, color, width);
		koch(x4, y4, x3, y3, k - 1, color, width);
		koch(x3, y3, x1, y1, k - 1, color, width);
	}
	else
	{
		hdc_line(x2, y2, x4, y4, width, color);
		hdc_line(x4, y4, x3, y3, width, color);
		hdc_line(x3, y3, x1, y1, width, color);
		hdc_line(x0, y0, x2, y2, width, color);
	}
}

/* koch雪花(由三条koch曲线拼接而成) */
/*
	参数说明:
	      1.人为保证给定三点可以围成正三角形
		  2.k           :koch曲线递归次数
		  3.color       :雪花边线的颜色
		  4.width       :雪花边线的宽度
 */
static void draw_snowflake(int x1, int y1, int x2, int y2, int x3, int y3, int k, int color, int width)
{
	koch(x1, y1, x2, y2, k, color, width);
	koch(x2, y2, x3, y3, k, color, width);
	koch(x3, y3, x1, y1, k, color, width);
}

/* 模拟雪花的下落 */
/*
	参数说明:
		  1.x0,y0       :正三角形的中心
		  2.k           :koch曲线递归次数
		  3.color       :雪花边线的颜色
		  4.width       :雪花边线的宽度
		  5.length      :正三角形的边长
 */
static void snowflake_down(int x0, int y0, int k, int color, int width, int length)
{
	int x1, y1, x2, y2, x3, y3;
	int angle = 0;
	for (; y0 <= win_high; y0 += 20, angle += 10) {
		x1 = x0 + (int)(length / sqrt(3.0) * sin(angle * PI / 180));
		y1 = y0 - (int)(length / sqrt(3.0) * cos(angle * PI / 180));
		x2 = x0 + (int)(length / sqrt(3.0) * sin((angle - 120) * PI / 180));
		y2 = y0 - (int)(length / sqrt(3.0) * cos((angle - 120) * PI / 180));
		x3 = x0 + (int)(length / sqrt(3.0) * sin((angle + 120) * PI / 180));
		y3 = y0 - (int)(length / sqrt(3.0) * cos((angle + 120) * PI / 180));

		draw_snowflake(x1, y1, x2, y2, x3, y3, k, color, width);
		Sleep(50);
		draw_snowflake(x1, y1, x2, y2, x3, y3, k, win_bgcolor, width);
	}
}

void hdc_draw_Mathematical_curve_2152046()
{
	hdc_cls();

	srand((unsigned)time(NULL));
	int num = 3;   //雪花的数量
	int k = 3;

	/* 雪花的下落 */
	while (num != 0) {
		int x0 = rand() % win_width, y0 = rand() % (win_high / 2);
		int length = rand() % 20 + 100;
		snowflake_down(x0, y0, k, color, width, length);
		num--;
	}

	/* 在屏幕中间画一个雪花 */
	int x1, y1, x2, y2, x3, y3;
	int length = 400, angle = 0;
	x1 = base_x + (int)(length / sqrt(3.0) * sin(angle * PI / 180));
	y1 = base_y - (int)(length / sqrt(3.0) * cos(angle * PI / 180));
	x2 = base_x + (int)(length / sqrt(3.0) * sin((angle - 120) * PI / 180));
	y2 = base_y - (int)(length / sqrt(3.0) * cos((angle - 120) * PI / 180));
	x3 = base_x + (int)(length / sqrt(3.0) * sin((angle + 120) * PI / 180));
	y3 = base_y - (int)(length / sqrt(3.0) * cos((angle + 120) * PI / 180));
	draw_snowflake(x1, y1, x2, y2, x3, y3, k + 1, color, width);

}