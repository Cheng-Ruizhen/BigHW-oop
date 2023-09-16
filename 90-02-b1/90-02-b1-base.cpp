/* 2152046 大数据 程瑞真 */
#include<iostream>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"../include/消除类游戏函数集.h"
#include"90-02-b1.h"

using namespace std;

void main_base(int row, int col,int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	int once_score, sum_score = 0;
	while (1) {
		cout << "当前数组：" << endl;
		print(row, col, map, v, 1);
		cout << endl;
		int num;
		if (num = finish(row, col, map)) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(a, b, "剩余", COLOR_BLACK, COLOR_HYELLOW);
			cct_getxy(a, b);
			cct_showint(a, b, num, COLOR_BLACK, COLOR_HYELLOW);
			cct_getxy(a, b);
			cct_showstr(a, b, "个星星，无可消除项，本关结束！", COLOR_BLACK, COLOR_HYELLOW);
			cout << "按回车继续下一关";
			while (1) {
				char ch = _getch();
				if (ch == '\r')
					break;
			}
			cct_cls();
			continue;
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
			no_search(row, col, x - 'A' + 1, y - '0' + 1, map, v);
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
		if (choice == 'A') {
			cout << endl;
			break;
		}
		cout << endl << "请确认是否把"<<x<<y<<"及周围的相同值消除(Y / N / Q)：";
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
			once_score = score(row, col, v);
			sum_score += once_score;
			change(row, col, map, v);
			cout << "相同值归并后的数组(不同色标识)：" << endl;
			print(row, col, map, v, 3);
			cout << endl << "本次得分：" << once_score << " 总得分：" << sum_score << endl << endl;
			cout << "按回车键进行数组下落除0操作...";
			while (1) {
				char ch = _getch();
				if (ch == '\r')
					break;
			}
			cout << endl << "下落后的数组：" << endl;
			array_move(down, single, wid, heig, row, col, map, v, form, false, 0, (char*)" ");
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
		if (choice == 'B') {
			cout << endl;
			break;
		}
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				v[i][j] = 0;
			}
		}
		cout << endl << "本次合成结束，按回车键继续新一次的合成..." << endl;
		while (1) {
			char ch = _getch();
			if (ch == '\r')
				break;
		}
	}
}