/* 2152046 程瑞真 大数据 */
//  game : 2048
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"../include/cmd_console_tools.h"
#include"../include/cmd_gmw_tools.h"
#include"90-02-b2.h"

using namespace std;

int main()
{
	int maxscore = 0;
	while (1) {
		int row, col, target, delay;
		input(row, col, target, delay);
		int g2048[max_row][max_col] = { 0 };

		/* 初始化方阵 */
		srand((unsigned int)(time(NULL)));
		int x1 = rand() % row, x2, y1 = rand() % col, y2;
		while (1) {
			x2 = rand() % row;
			if (x2 != x1)
				break;
		}
		while (1) {
			y2 = rand() % col;
			if (y2 != y1)
				break;
		}

		g2048[x1][y1] = 2, g2048[x2][y2] = 2;

		CONSOLE_GRAPHICS_INFO G2048_CGI; //声明一个CGI变量

		/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
		gmw_init(&G2048_CGI);

		gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
		gmw_set_font(&G2048_CGI, "新宋体", 16);						//字体
		gmw_set_frame_style(&G2048_CGI, 12, 5, true);					//游戏主区域风格：每个色块宽12高5，有分隔线【数字色块带边框，宽度为12(放最多6位数字)，高度为5(为了保持色块为方形)】
		gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
		gmw_set_block_border_switch(&G2048_CGI, true);				//小色块带边框
		gmw_set_delay(&G2048_CGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS + delay);   //设置色块移动延时
		gmw_set_status_line_switch(&G2048_CGI, TOP_STATUS_LINE, false);  //关闭上状态栏
		gmw_set_ext_rowcol(&G2048_CGI, 1);
		G2048_CGI.is_need_mouse = 0;                         //不需要鼠标操作

		bool remake = 0;
		step_of_2048(&G2048_CGI, row, col,target, g2048, remake, maxscore);

		if (remake == 0)
			break;
	}

	return 0;
}