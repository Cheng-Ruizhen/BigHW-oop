/* 2152046 程瑞真 大数据 */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

int main()
{
	int row, col,target;
	input(row, col, target);
	int map[max_row][max_col] = { 0 };
	int v[max_row][max_col] = { 0 };

	/*   初始化矩阵   */
	srand((unsigned int)(time(NULL)));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j] = rand() % 3 + 1;
		}
	}

	CONSOLE_GRAPHICS_INFO Mto10_CGI; //声明一个CGI变量

	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&Mto10_CGI);

	gmw_set_color(&Mto10_CGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_font(&Mto10_CGI, "Terminal", 16, 8);
	gmw_set_frame_style(&Mto10_CGI, 6, 3, false);	//色块带边框，宽度为6(放最多2位数字)，高度为3(最多放一行)
	gmw_set_frame_color(&Mto10_CGI, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_border_switch(&Mto10_CGI, true);
	gmw_set_rowno_switch(&Mto10_CGI, true);//显示行号
	gmw_set_colno_switch(&Mto10_CGI, true);//显示列标
	gmw_set_delay(&Mto10_CGI, DELAY_OF_BLOCK_MOVED, 10);

	step_of_merge_to_10(&Mto10_CGI, row, col, map, v, target);

	return 0;
}