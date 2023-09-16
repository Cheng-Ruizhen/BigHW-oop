/* 2152046 大数据 程瑞真 */
#include <iostream>
#include <string>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：判断显示的语句最后是否为半个汉字
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void checkchinese(string& str, int menu_col)
{
	int len = str.size();
	if (len <= menu_col * 4) //一行中可以装下所有语句
		return;
	else {
		int charnum = 0;
		for (int i = 0; i < menu_col * 4; i++) {
			if (str[i] >= 33 && str[i] <= 126)
				charnum++;
		}
		if (charnum % 2 != 0) { //有奇数个字母
			str[menu_col * 4 - 1] = ' ';
		}
	}
	return;
}

/***************************************************************************
  函数名称：
  功    能：弹出式菜单
  输入参数：
  返 回 值：
  说    明：由于采用gmw，游戏界面会有额外行列产生，换算关系：实际行=menu_row * 2，实际列=menu_col * 4
***************************************************************************/
string pop_menu(string* opt, int num, int menu_row, int menu_col)
{
	CONSOLE_GRAPHICS_INFO Menu_CGI; //定义一个菜单CGI

	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&Menu_CGI, menu_row, menu_col);

	gmw_set_font(&Menu_CGI, "新宋体", 16);				//字体
	gmw_set_ext_rowcol(&Menu_CGI, 3, 3, 21, 21);       //额外行列
	Menu_CGI.is_need_mouse = 0;                         //不需要鼠标操作
	Menu_CGI.CFI.separator = 0;                         //色块之间不需要分割线

	gmw_draw_frame(&Menu_CGI);
	Menu_CGI.SLI.top_start_x++;
	gmw_status_line(&Menu_CGI, TOP_STATUS_LINE, "请选择文件");

	int pos = 0; //指针位置，范围[0 - menu_row-1]
	int first = 0; //菜单中第一个选项在opt中的下标
	int start_x = Menu_CGI.start_x + 2, start_y = Menu_CGI.start_y + 2; //选项开始位置
	while (1)
	{
		for (int i = 0; i < 2 * menu_row && i < num; i++) {
			string temp = opt[first + i];
			checkchinese(temp, menu_col);
			if (i == pos)
				cct_showstr(start_x, start_y + i, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
			else
				cct_showstr(start_x, start_y + i, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
		}

		int maction, mrow = 0, mcol = 0;
		int keycode1, keycode2;
		int ret;
		while (1) {
			int status = 0;
			ret = gmw_read_keyboard_and_mouse(&Menu_CGI, maction, mrow, mcol, keycode1, keycode2);
			switch (keycode1) {
			case 0xE0:
				switch (keycode2) {
				case KB_ARROW_UP:
					if (pos == 0) { //指向开头的选项
						if (first != 0) { //开头的选项不是第一个选项
							first--;
							status = 1;
						}
					}
					else { //指向中间的选项
						string temp = opt[first + pos];
						checkchinese(temp, menu_col);
						cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
						pos--;
						temp = opt[first + pos];
						checkchinese(temp, menu_col);
						cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
					}
					break;
				case KB_ARROW_DOWN:
					if (pos == 2 * menu_row - 1) { //指向结尾的选项
						if (first != (num - 2 * menu_row - 1)) { //还可以向下滚动
							first++;
							status = 1;
						}
					}
					else { //指向中间的选项
						if (pos != num - 1) { //还得保证不是指向最后一个元素
							string temp = opt[first + pos];
							checkchinese(temp, menu_col);
							cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
							pos++;
							temp = opt[first + pos];
							checkchinese(temp, menu_col);
							cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
						}
					}
					break;
				} //switch1
				break;
			case '\n':
			case '\r':
				return opt[first + pos]; //循环结束
			} //switch2

			if (status == 1)
				break;
		} //while1
	} //while2
}