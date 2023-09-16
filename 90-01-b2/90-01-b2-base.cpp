/* 2152046 信14 程瑞真 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"../include/消除类游戏函数集.h"
#include"90-01-b2.h"

using namespace std;

void print_main_base(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	int sum_score = 0, once_score, max;
	while (1) {
		cout << "当前数组：" << endl;
		print(row, col, map, v, 1);
		cout << endl;
		if (finish(row, col, map)) {
			cout << "无可消除项" << endl;
			break;
		}
		char x, y;
		while (1) {
			cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
			cin >> x >> y;
			if (x >= 'a' && x <= 'z') {
				x -= 32;
			}
			if (cin.fail() || (x<'A' || x>char('A' + row - 1)) || (y<'0' || y>char('0' + col - 1))) {
				cout << "输入错误，请重新输入." << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				continue;
			}
			cout << "输入为" << x << "行" << y << "列" << endl;
			if (choice == '1') {
				no_search(row, col, x - 'A' + 1, y - '0' + 1, map, v);
			}
			else {
				is_search(row, col, x - 'A' + 1, y - '0' + 1, map, v);
			}
			int num = 0;
			for (int i = 1; i <= row; i++) {
				for (int j = 1; j <= col; j++) {
					if (v[i][j] != 0) {
						num++;
					}
				}
			}
			if (num == 1) {
				cout << "输入的矩阵坐标位置处无连续相同值，请重新输入" << endl;
				for (int i = 1; i <= row; i++) {
					for (int j = 1; j <= col; j++) {
						v[i][j] = 0;
					}
				}
				continue;
			}
			else {
				cout << endl << "查找结果数组：" << endl;
				print(row, col, map, v, 2);
				cout << endl;
				cout << "当前数组(不同色标识)：" << endl;
				print(row, col, map, v, 3);
			}
			break;
		}
		if (choice == '1' || choice == '2') {
			cout << endl;
			break;
		}
		cout << endl << "请确认是否把相邻的相同值合并到" << x << y << "中(Y / N / Q)：";
		char ch, ch1;
		while (1) {
			ch = _getch(), ch1 = ch;
			if (ch >= 'a' && ch <= 'z') {
				ch1 -= 32;
			}
			if (ch1 == 'Y' || ch1 == 'N' || ch1 == 'Q') {
				cout << ch << endl << endl;
				break;
			}
		}
		if (ch1 == 'Y') {
			once_score = score(row, col, map, v);
			sum_score += once_score;
			max = change(row, col, x - 'A' + 1, y - '0' + 1, map, v);
			cout << "相同值归并后的数组(不同色标识)：" << endl;
			print(row, col, map, v, 3);
			cout << endl << "本次得分：" << once_score << " 总得分：" << sum_score << " 合成目标：" << target << endl << endl;
			v[x - 'A' + 1][y - '0' + 1] = 0;
			cout << "按回车键进行数组下落除0操作...";
			while (1) {
				char ch = _getch();
				if (ch == '\r') 
					break;
			}
			cout << endl << "除0后的数组(不同色标识)：" << endl;
			int a = 0, b = 0;
			array_move(down, single, wid, heig, row, col, map, v, form, false, 0, (char*)" ");
			print(row, col, map, v, 3);
			cout << endl << "按回车键进行新值填充...";
			while (1) {
				char ch = _getch();
				if (ch == '\r') 
					break;
			}
			cout << endl << "新值填充后的数组(不同色标识)：" << endl;
			fill(row, col, max, map);
			print(row, col, map, v, 3);
		}
		else if (ch1 == 'N') {
			for (int i = 1; i <= row; i++) {
				for (int j = 1; j <= col; j++) {
					v[i][j] = 0;
				}
			}
			continue;
		}
		else if (ch1 == 'Q')
			break;
		if (choice == '3') {
			cout << endl;
			break;
		}
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				v[i][j] = 0;
			}
		}
		cout << endl << "本次合成结束，按回车键继续新一次的合成...";
		while (1) {
			char ch = _getch();
			if (ch == '\r') 
				break;
		}
		cout << endl << endl;
		if (max == target) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(a, b, "已经合成到", COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			cct_getxy(a, b);
			cct_showch(a, b, char(target + '0'), COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			target++;
			cout << endl << "按回车键继续向更高目标进发..." << endl << endl;
			while (1) {
				char ch = _getch();
				if (ch == '\r')
					break;
			}
		}
	}
}