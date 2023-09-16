/* 2152046 ������ ������ */
#include<iostream>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include"../include/cmd_hdc_tools.h"
using namespace std;

static const int color = RGB(0,255,255);        //��ɫ
static const int width = 2;                     //ѩ���߽��ȶ�Ϊ2
const int   win_width = 800, win_high = 800;	//�趨��Ļ���
const int   win_bgcolor = RGB(204, 204, 204);   //��Ļ����ɫ,��wordȡɫ��ȡ��
static const double PI = 3.14159;
static int base_x = 400;
static int base_y = 300;

/* Koch���� */
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

/* kochѩ��(������koch����ƴ�Ӷ���) */
/*
	����˵��:
	      1.��Ϊ��֤�����������Χ����������
		  2.k           :koch���ߵݹ����
		  3.color       :ѩ�����ߵ���ɫ
		  4.width       :ѩ�����ߵĿ��
 */
static void draw_snowflake(int x1, int y1, int x2, int y2, int x3, int y3, int k, int color, int width)
{
	koch(x1, y1, x2, y2, k, color, width);
	koch(x2, y2, x3, y3, k, color, width);
	koch(x3, y3, x1, y1, k, color, width);
}

/* ģ��ѩ�������� */
/*
	����˵��:
		  1.x0,y0       :�������ε�����
		  2.k           :koch���ߵݹ����
		  3.color       :ѩ�����ߵ���ɫ
		  4.width       :ѩ�����ߵĿ��
		  5.length      :�������εı߳�
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
	int num = 3;   //ѩ��������
	int k = 3;

	/* ѩ�������� */
	while (num != 0) {
		int x0 = rand() % win_width, y0 = rand() % (win_high / 2);
		int length = rand() % 20 + 100;
		snowflake_down(x0, y0, k, color, width, length);
		num--;
	}

	/* ����Ļ�м仭һ��ѩ�� */
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