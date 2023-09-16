/* 2152046 程瑞真 大数据 */
#include <iostream>
#include <Windows.h>
#include <cmath>
#include "../include/cmd_hdc_tools.h"
using namespace std;

static const double PI = 3.14159;
const int SIZE_WIDTH = 750, SIZE_HIGH = 750;

const int MyCOLOR_OUTLINE = RGB(0, 0, 0);   //黑色
const int MyCOLOR_BACKGROUND = RGB(253, 251, 144);  //浅黄色
const int MyCOLOR_CAP = RGB(247, 209, 126);   //橙黄色
const int MyCOLOR_CLOTH = RGB(253, 123, 104);  //红色
const int MyCOLOR_BODY = RGB(255, 237, 223);  //浅橙色
const int MyCOLOR_HAIR = RGB(0, 0, 0);    //黑色
const int MyCOLOR_EYEBALL = RGB(248, 248, 247);  //白色
const int MyCOLOR_PUPILS = RGB(2, 3, 4);   //黑色

//顺时针给定三点，通过引用返回圆弧的圆心、半径、起始角、终止角
static void GETPARA(int x1, int y1, int x2, int y2, int x3, int y3, int& x0, int& y0, int& r, int& start_angle, int& end_angle)
{
	double t = -(((x3 - x1) * (x3 - x2) + (y3 - y1) * (y3 - y2)) * 1.0 / ((x3 - x1) * (y2 - y1) - (x2 - x1) * (y3 - y1)));

	x0 = (int)((x1 + x2 + (y1 - y2) * t) / 2);
	y0 = (int)((y1 + y2 + (x2 - x1) * t) / 2);
	r  = (int)(sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));

	double v01[2] = { (double)(x1 - x0),(double)(y1 - y0) };
	double L01 = sqrt(v01[0] * v01[0] + v01[1] * v01[1]);
	double v03[2] = { (double)(x3 - x0),(double)(y3 - y0) };
	double L03 = sqrt(v03[0] * v03[0] + v03[1] * v03[1]);
	double Angle1_cos = (v01[0] * 0 + v01[1] * (-1)) / (L01 * 1);
	double Angle1 = acos(Angle1_cos) * 180 / PI;
	double Angle3_cos = (v03[0] * 0 + v03[1] * (-1)) / (L03 * 1);
	double Angle3 = acos(Angle3_cos) * 180 / PI;

	start_angle = (int)(Angle1 * v01[0] / abs(v01[0]));
	end_angle = (int)(Angle3 * v03[0] / abs(v03[0]));
}

static void background(const int base_x, const int base_y)
{
	int left_up_x = base_x - SIZE_WIDTH / 2, left_up_y = base_y - SIZE_HIGH / 2;
	hdc_rectangle(left_up_x, left_up_y, SIZE_WIDTH, SIZE_HIGH, 0, true, 2, MyCOLOR_BACKGROUND);
	hdc_rectangle(left_up_x, left_up_y, SIZE_WIDTH, SIZE_HIGH, 0, false, 5, MyCOLOR_OUTLINE);
}

static void cap(const int base_x, const int base_y)
{
	hdc_ellipse(base_x, base_y - 100, 222, 197, 0, true, 2, MyCOLOR_CAP);
	hdc_ellipse(base_x, base_y - 100, 225, 200, 0, false, 4, MyCOLOR_OUTLINE);
}

static void lower_part(const int base_x, const int base_y)
{
	int x0, y0, r, start_angle, end_angle;
	int x1, y1, x2, y2, x3, y3;

	/*-------------------- 左半边 ------------------------*/
	x1 = base_x - 96, y1 = base_y + 136;
	x2 = base_x - 75, y2 = base_y + 108;
	x3 = base_x - 30, y3 = base_y + 95;
	
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_sector(x0, y0, r, start_angle, end_angle, true, 2, MyCOLOR_CLOTH);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_OUTLINE);
	hdc_line(x1 + 20, y1, x1 + 15, y1 - 25, 2, MyCOLOR_OUTLINE);

	/*-------------------- 右半边 ------------------------*/
	x1 = base_x + 96, y1 = base_y + 136;
	x2 = base_x + 75, y2 = base_y + 108;
	x3 = base_x + 30, y3 = base_y + 95;

	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_sector(x0, y0, r, end_angle, start_angle, true, 2, MyCOLOR_CLOTH);
	hdc_arc(x0, y0, r, end_angle, start_angle, 4, MyCOLOR_OUTLINE);
	hdc_line(x1 - 20, y1, x1 - 15, y1 - 25, 2, MyCOLOR_OUTLINE);

	/*------------------- 中间部分 -----------------------*/
	hdc_ellipse(base_x, base_y + 100, 40, 52, 0, true, 2, MyCOLOR_BODY);
	hdc_ellipse(base_x, base_y + 100, 42, 54, 0, false, 4, MyCOLOR_OUTLINE);

	hdc_rectangle(base_x - 96, base_y + 136, 200, 100, 0, true, 1, MyCOLOR_BACKGROUND);
}

static void neck(const int base_x, const int base_y)
{
	hdc_rectangle(base_x - 38, base_y + 75, 76, 25, 0, true, 2, MyCOLOR_BODY);
	hdc_rectangle(base_x - 24, base_y + 55, 48, 20, 0, true, 2, MyCOLOR_BODY);
	hdc_line(base_x - 24, base_y + 55, base_x - 24, base_y + 75, 3, MyCOLOR_OUTLINE);
	hdc_line(base_x + 24, base_y + 55, base_x + 24, base_y + 75, 3, MyCOLOR_OUTLINE);

	int x0, y0, r, start_angle, end_angle;
	int x1, y1, x2, y2, x3, y3;

	x1 = base_x - 23, y1 = base_y + 72;
	x2 = base_x - 30, y2 = base_y + 92;
	x3 = base_x - 40, y3 = base_y + 97;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_sector(x0, y0, r, start_angle, end_angle, true, 2, MyCOLOR_CAP);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_OUTLINE);

	x1 = base_x + 25, y1 = base_y + 72;
	x2 = base_x + 30, y2 = base_y + 92;
	x3 = base_x + 41, y3 = base_y + 97;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_sector(x0, y0, r, end_angle, start_angle, true, 2, MyCOLOR_CAP);
	hdc_arc(x0, y0, r, end_angle, start_angle, 4, MyCOLOR_OUTLINE);
}

static void ears(const int base_x, const int base_y)
{
	hdc_ellipse(base_x - 127, base_y - 50, 32, 36, -25, true, 2, MyCOLOR_BODY);
	hdc_ellipse(base_x - 127, base_y - 50, 32, 39, -25, false, 4, MyCOLOR_OUTLINE);

	hdc_ellipse(base_x + 127, base_y - 50, 32, 36, 25, true, 2, MyCOLOR_BODY);
	hdc_ellipse(base_x + 127, base_y - 50, 32, 39, 25, false, 4, MyCOLOR_OUTLINE);
}

static void head(const int base_x, const int base_y) {
	int x0, y0, r, start_angle, end_angle;
	int x1, y1, x2, y2, x3, y3;

	hdc_ellipse(base_x, base_y - 77, 144, 150, 0, false, 4, MyCOLOR_OUTLINE);
	hdc_ellipse(base_x, base_y - 109, 156, 143, 0, true, 2, MyCOLOR_HAIR);
	ears(base_x, base_y);
	hdc_ellipse(base_x, base_y - 77, 142, 148, 0, true, 1, MyCOLOR_BODY);
	x1 = base_x - 150, y1 = base_y - 60;
	x2 = base_x - 135, y2 = base_y - 51;
	x3 = base_x - 132, y3 = base_y - 37;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 3, MyCOLOR_OUTLINE);
	x1 = base_x + 150, y1 = base_y - 60;
	x2 = base_x + 135, y2 = base_y - 51;
	x3 = base_x + 132, y3 = base_y - 37;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 3, MyCOLOR_OUTLINE);

	x1 = base_x - 126, y1 = base_y - 5;
	x2 = base_x - 134, y2 = base_y - 38;
	x3 = base_x - 133, y3 = base_y - 67;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_OUTLINE);
	x1 = base_x + 126, y1 = base_y - 5;
	x2 = base_x + 134, y2 = base_y - 38;
	x3 = base_x + 133, y3 = base_y - 67;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 4, MyCOLOR_OUTLINE);
	hdc_triangle(base_x - 148, base_y - 77, base_x - 120, base_y - 53, base_x - 90, base_y - 210, true, 2, MyCOLOR_HAIR);
	hdc_triangle(base_x - 148, base_y - 77, base_x - 70, base_y - 210, base_x - 130, base_y - 190, true, 2, MyCOLOR_HAIR);
	x1 = base_x - 119, y1 = base_y - 53;
	x2 = base_x - 110, y2 = base_y - 101;
	x3 = base_x - 93, y3 = base_y - 150;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_OUTLINE);
	hdc_triangle(base_x - 93, base_y - 150, base_x - 88, base_y - 91, base_x - 12, base_y - 245, true, 3, MyCOLOR_HAIR);
	hdc_triangle(base_x - 93, base_y - 150, base_x - 12, base_y - 245, base_x - 100, base_y - 200, true, 2, MyCOLOR_HAIR);
	hdc_triangle(base_x - 93, base_y - 150, base_x - 108, base_y - 120, base_x - 106, base_y - 190, true, 2, MyCOLOR_HAIR);
	x1 = base_x - 88, y1 = base_y - 91;
	x2 = base_x - 92, y2 = base_y - 118;
	x3 = base_x - 93, y3 = base_y - 150;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_OUTLINE);
	x1 = base_x - 88, y1 = base_y - 91;
	x2 = base_x - 48, y2 = base_y - 172;
	x3 = base_x - 15, y3 = base_y - 224;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_HAIR);
	hdc_triangle(x3, y3, x3 - 16, y3 + 22, x3 - 8, y3 - 10, true, 1, MyCOLOR_HAIR);
	hdc_triangle(x3, y3, x3 + 50, y3 + 90, x3 + 40, y3 , true, 2, MyCOLOR_HAIR);
	hdc_triangle(base_x - 15 + 39, base_y - 224 + 17, base_x - 15 + 39, base_y - 224 - 6, base_x - 15 + 114, base_y - 224 + 33, true, 2, MyCOLOR_HAIR);
	hdc_triangle(base_x + 103, base_y - 125, base_x + 124, base_y - 50, base_x + 115, base_y - 205, true, 2, MyCOLOR_HAIR);
	x1 = base_x + 103, y1 = base_y - 120;
	x2 = base_x + 113, y2 = base_y - 95;
	x3 = base_x + 124, y3 = base_y - 50;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_BODY);
	hdc_triangle(base_x - 15 + 39, base_y - 224 + 17, base_x - 15 + 119, base_y - 224 + 135, base_x - 15 + 114, base_y - 224 + 33, true, 2, MyCOLOR_HAIR);
	x1 = base_x + 24, y1 = base_y - 207;
	x2 = base_x + 64, y2 = base_y - 153;
	x3 = base_x + 104, y3 = base_y - 80;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 6, MyCOLOR_BODY);
	hdc_triangle(base_x - 15, base_y - 224 - 2, base_x - 15 + 50, base_y - 224 + 90, base_x - 15 + 40, base_y - 224 - 2, true, 2, MyCOLOR_HAIR);
	x1 = base_x + 35, y1 = base_y - 134;
	x2 = base_x + 20, y2 = base_y - 152;
	x3 = base_x - 15, y3 = base_y - 224;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_HAIR);
	hdc_triangle(x1, y1, x2, y2, x3, y3, true, 2, MyCOLOR_HAIR);
	hdc_triangle(x3, y3, x2, y2, (x3 + x2) / 2 - 5, (y3 + y2) / 2, true, 2, MyCOLOR_HAIR);
	hdc_triangle(base_x + 103, base_y - 125, base_x + 115, base_y - 205, base_x + 97, base_y -220, true, 4, MyCOLOR_HAIR);
	hdc_triangle(base_x + 115, base_y - 205, base_x + 124, base_y - 50, base_x + 150, base_y - 147, true, 3, MyCOLOR_HAIR);
	x1 = base_x + 126, y1 = base_y - 51;
	x2 = base_x + 150, y2 = base_y - 75;
	x3 = base_x + 160, y3 = base_y - 63;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_HAIR);
	hdc_triangle(x2, y2, x2 - 10, y2 - 50, x2 - 20, y2, true, 2, MyCOLOR_HAIR);
	hdc_triangle(x3 + 3, y3 + 5, x3 - 9, y3 - 80, x3 - 13, y3 - 18, true, 4, MyCOLOR_HAIR);
	x1 = base_x + 165, y1 = base_y - 60;
	x2 = base_x + 159, y2 = base_y - 89;
	x3 = base_x + 153, y3 = base_y - 143;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_CAP);
	hdc_line(x3 - 2, y3, (x2 + x3) / 2 + 1, (y2 + y3) / 2 + 23, 3, MyCOLOR_HAIR);
	hdc_triangle(base_x + 53, base_y - 243, base_x + 60, base_y - 202, base_x + 150, base_y - 214, true, 2, MyCOLOR_HAIR);
	x1 = base_x + 67, y1 = base_y - 241;
	x2 = base_x + 106, y2 = base_y - 225;
	x3 = base_x + 154, y3 = base_y - 214;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 3, MyCOLOR_CAP);
	x1 = base_x + 60, y1 = base_y - 203;
	x2 = base_x + 107, y2 = base_y - 204;
	x3 = base_x + 150, y3 = base_y - 212;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 3, MyCOLOR_HAIR);
	hdc_triangle(base_x - 180, base_y - 87, base_x - 150, base_y - 111, base_x - 147, base_y - 160, true, 2, MyCOLOR_HAIR);
	x1 = base_x - 184, y1 = base_y - 82;
	x2 = base_x - 167, y2 = base_y - 110;
	x3 = base_x - 143, y3 = base_y - 172;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 2, MyCOLOR_CAP);
	hdc_arc(x0, y0, r - 1, end_angle, start_angle, 3, MyCOLOR_CAP);
	x1 = base_x - 144, y1 = base_y - 114;
	x2 = base_x - 165, y2 = base_y - 93;
	x3 = base_x - 180, y3 = base_y - 83;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 2, MyCOLOR_HAIR);
} 

static void eye(const int base_x, const int base_y)
{
	int a0 = 43, b0 = 40;
	int a1 = 14, b1 = 16;
	int x1 = base_x - 75, y1 = base_y - 52;
	hdc_ellipse(x1, y1, a0, b0, 0, true, 0, MyCOLOR_EYEBALL);
	hdc_ellipse(x1 + 3, y1 + 1, a1, b1, 0, true, 2, MyCOLOR_PUPILS);
	hdc_ellipse(x1 + 3, y1 + 1, a1 - 1, b1 - 1, 0, false, 7, MyCOLOR_PUPILS);
	hdc_arc(x1, y1, a0, -70, 60, 4, MyCOLOR_OUTLINE);
	hdc_arc(x1, y1, a0, 157, 200, 5, MyCOLOR_OUTLINE);

	int x2 = base_x + 75, y2 = base_y - 52;
	hdc_ellipse(x2, y2, a0, b0, 0, true, 0, MyCOLOR_EYEBALL);
	hdc_ellipse(x2 - 3, y2 + 1, a1, b1, 0, true, 2, MyCOLOR_PUPILS);
	hdc_ellipse(x2 - 3, y2 + 1, a1 - 1, b1 - 1, 0, false, 7, MyCOLOR_PUPILS);
	hdc_arc(x2, y2, a0, -60, 90, 4, MyCOLOR_OUTLINE);
	hdc_arc(x2, y2, a0, 165, 208, 5, MyCOLOR_OUTLINE);
}

static void other(const int base_x, const int base_y)
{
	int x0, y0, r, start_angle, end_angle;
	int x1, y1, x2, y2, x3, y3;

	/* 眉毛 */
	x1 = base_x - 73, y1 = base_y - 137;
	x2 = base_x - 52, y2 = base_y - 138;
	x3 = base_x - 30, y3 = base_y - 128;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, start_angle, end_angle, 4, MyCOLOR_HAIR);
	x1 = base_x + 78, y1 = base_y - 137;
	x2 = base_x + 57, y2 = base_y - 138;
	x3 = base_x + 35, y3 = base_y - 128;
	GETPARA(x1, y1, x2, y2, x3, y3, x0, y0, r, start_angle, end_angle);
	hdc_arc(x0, y0, r, end_angle, start_angle, 4, MyCOLOR_HAIR);

	/* 鼻子 */
	hdc_point(base_x, base_y - 5, 6, MyCOLOR_OUTLINE);
	hdc_point(base_x, base_y - 3, 5, MyCOLOR_OUTLINE);

	/* 嘴巴 */
	hdc_line(base_x - 10, base_y + 35, base_x + 10, base_y + 35, 4, MyCOLOR_OUTLINE);

	/* 刀疤 */
	hdc_line(base_x + 65, base_y + 10, base_x + 83, base_y + 7, 4, MyCOLOR_OUTLINE);
	hdc_arc(base_x + 63, base_y - 7, 20, 174, 207, 4, MyCOLOR_OUTLINE);
	hdc_arc(base_x + 63, base_y - 7, 20, 174, 213, 3, MyCOLOR_OUTLINE);
	hdc_line(base_x + 65, base_y + 1, base_x + 64, base_y + 17, 3, MyCOLOR_OUTLINE);
	hdc_line(base_x + 77, base_y + 1, base_x + 78, base_y + 15, 3, MyCOLOR_OUTLINE);
}

void hdc_draw_cartoon_2152046(const int base_x, int base_y)
{
	hdc_cls();

	background(base_x, base_y);
	lower_part(base_x, base_y);
	cap(base_x, base_y);
	neck(base_x, base_y);
	head(base_x, base_y);
	eye(base_x, base_y);
	other(base_x, base_y);
}