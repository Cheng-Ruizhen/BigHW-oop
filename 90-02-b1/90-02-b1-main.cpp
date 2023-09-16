/* 2152046 大数据 程瑞真 */
#include <iostream>
#include<Windows.h>
#include "../include/cmd_console_tools.h"
#include"../include/menu.h"
#include"../include/消除类游戏函数集.h"
#include"90-02-b1.h"
using namespace std;

int main()
{
	const char option[][100] = {
		"A.命令行找出可消除项并标识",
		"B.命令行完成一次消除（分步骤显示）",
		"C.命令行完成一关（分步骤显示）",
		"D.伪图形界面下用鼠标选择一个色块（无分隔线）",
		"E.伪图形界面下用鼠标选择一个色块（有分隔线）",
		"F.伪图形界面完成一次消除（分步骤）",
		"G.伪图形界面完整版",
		"Q.退出"
	};

	while (1) {
		char choice;
		while (choice = menu(option, 8)) {
			if (choice >= 'A' && choice <= 'G')
				break;
			else if (choice == 'Q')
				return 0;
			else
				cct_cls();
		}
		Sleep(200);
		cct_cls();
		int row, col;
		input(row, col, choice);
		int map[max_row][max_col] = { 0 };
		int v[max_row][max_col] = { 0 };
     
		/*   初始化矩阵   */
		srand((unsigned int)(time(NULL)));
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				map[i][j] = rand() % 5 + 1;
			}
		}

		/*   根据选项确定命令行还是伪图形   */
		if (choice >= 'A' && choice <= 'C')
			main_base(row, col, map, v, choice);
		else if (choice >= 'D' && choice <= 'G')
			main_console(row, col, map, v, choice);

		while (1) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(0, b, "                                                        ");
			cct_showstr(0, b, "本小题结束，请输入End继续... ");
			char k[100];
			cin >> k;
			if (tj_strcasecmp(k, "End") != 0) {
				cout << "输入错误，请重新输入" << endl;
				continue;
			}
			break;
		}
		cct_cls();
	}
	return 0;
}
