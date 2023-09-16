/* 2152046 信14 程瑞真 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
#include<string.h>
#include"../include/cmd_console_tools.h"
#include"../include/menu.h"
#include"../include/消除类游戏函数集.h"
#include"90-01-b2.h"
using namespace std;
int main()
{
	const char option[][100] = {
		"1.命令行找出可合成项并标识（非递归)",
		"2.命令行找出可合成项并标识（递归）",
		"3.命令行完成一次合成（分步骤显示）",
		"4.命令行完整版（分步骤显示）",
		"5.伪图形界面显示初始数组（无分割线）",
		"6.伪图形界面显示初始数组（有分割线）",
		"7.伪图形界面下用箭头键/鼠标选择当前色块",
		"8.伪图形界面完成一次合成（分步骤）",
		"9.伪图形界面完整版（支持鼠标）",
		"0.退出"
	};
	while (1) {
		char choice;
		while (choice = menu(option,10)) {
			if (choice >= '0' && choice <= '9')
				break;
			else
				cct_cls();
		}
		if (choice == '0') {
			break;
		}
		else{
			Sleep(200);
			cct_cls();
			int row, col, target = 5;
			shuru(row, col, target, choice);
			int map[max_row][max_col] = { 0 };
			int v[max_row][max_col] = { 0 };

			/*   初始化矩阵   */
			srand((unsigned int)(time(NULL)));
			for (int i = 1; i <= row; i++) {
				for (int j = 1; j <= col; j++) {
					map[i][j] = rand() % 3 + 1;
				}
			}

			pivot(row, col, target, map, v, choice);
			while (1) {
				int a, b;
				cct_getxy(a, b);
				cct_showstr(0, b, "本小题结束，请输入End继续... ");
				char sta[4] = { 'E','N','D','\0' };
				char k[100] = {'\0'};
				cin >> k;
				if (tj_strcasecmp(k, "End") != 0) {
					cout << "输入错误，请重新输入" << endl;
					continue;
				}
				break;
			}
		}
		cct_cls();
	}
}