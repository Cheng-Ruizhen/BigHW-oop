/* 2152046 信14 程瑞真 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
#include"90-01-b2.h"
#include"../include/cmd_console_tools.h"
#include"../include/消除类游戏函数集.h"

using namespace std;

void pivot(int row, int col,int target, int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	if (choice >= '1' && choice <= '4') {
		print_main_base(row, col, target, map, v, choice);
	}
	else if (choice >= '5' && choice <= '9') {
		print_main_console(row, col, target, map, v, choice);
	}
}

void shuru(int &row,int &col,int &target,char choice) 
{
	if (choice >= '1' && choice <= '9') {
		while (1) {
			cout << "请输入行数(5-8)：" << endl;
			cin >> row;
			if (!cin.good()) {
				cin.ignore(10000, '\n');
				cin.clear();
			}
			if (row >= 5 && row <= 8) {
				break;
			}
		}
		while (1) {
			cout << "请输入列数(5-10)：" << endl;
			cin >> col;
			if (!cin.good()) {
				cin.ignore(10000, '\n');
				cin.clear();
			}
			if (col >= 5 && col <= 10) {
				break;
			}
		}
		if (choice == '3' || choice == '4' || choice == '9') {
			while (1) {
				cout << "请输入合成目标(5-20)：" << endl;
				cin >> target;
				if (!cin.good()) {
					cin.ignore(10000, '\n');
					cin.clear();
				}
				if (target >= 5 && target <= 20) {
					break;
				}
			}
		}
	}
}

void is_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col])
{
	int xx[4] = { 1,-1,0,0 };
	int yy[4] = { 0,0,1,-1 };
	v[my_row][my_col] = 1;
	for (int i = 0; i < 4; i++) {
		int newx = my_row + xx[i];
		int newy = my_col + yy[i];
		if (newx > 0 && newx <= row && newy > 0 && newy <= col) {
			if ((map[newx][newy] == map[my_row][my_col]) && (!v[newx][newy])) {
				is_search(row, col, newx, newy, map, v);
			}
		}
	}
}

int score(int row, int col, int map[max_row][max_col], int v[max_row][max_col])
{
	int num = 0, x = 0, y = 0;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (v[i][j] == 1) {
				num++;
				x = i, y = j;
			}
		}
	}
	return 3 * num * map[x][y];
}

int change(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col])
{
	int max = map[1][1];
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (v[i][j] == 1) {
				if (i == my_row && j == my_col) {
					map[i][j]++;
				}
				else {
					map[i][j] = 0;
				}
			}
			if (map[i][j] > max) {
				max = map[i][j];
			}
		}
	}
	return max;
}

void fill(int row, int col, int max, int map[max_row][max_col],char choice)
{
	srand((unsigned int)(time(NULL)));
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (map[i][j] == 0) {
				int s = rand() % 100 + 1;
				if (max == 3) 
					map[i][j] = rand() % 3 + 1;
				else if (max == 4) {
					if (s >= 1 && s <= 10) 
						map[i][j] = 4;
					else 
						map[i][j] = rand() % 3 + 1;
				}
				else if (max == 5) {
					if (s >= 1 && s <= 10)
						map[i][j] = 5;
					else if (s > 10 && s <= 25)
						map[i][j] = 4;
					else
						map[i][j] = rand() % 3 + 1;
				}
				else if (max == 6) {
					if (s >= 1 && s <= 5)
						map[i][j] = 6;
					else if (s >= 6 && s <= 20)
						map[i][j] = 5;
					else
						map[i][j] = rand() % 4 + 1;
				}
				else {
					if (s >= 1 && s <= 5)
						map[i][j] = max;
					else if (s >= 6 && s <= 10)
						map[i][j] = max - 1;
					else if (s >= 11 && s <= 20)
						map[i][j] = max - 2;
					else
						map[i][j] = rand() % (max - 3) + 1;
				}
				if (choice >= '8') {
					cct_gotoxy(8 * j - 4, 4 * i - 1);
					char ch[(wid - 4) + 1] = { '\0' };
					int_to_char(map[i][j], ch);
					diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
				}
			}
		}
	}
}

void int_to_char(int n, char* ch)
{
	memset(ch, ' ', wid - 4);
	if (n >= 10)
		ch[0] = '1', ch[1] = (char)(n % 10 + '0');
	else
		ch[0] = (char)(n + '0');
}