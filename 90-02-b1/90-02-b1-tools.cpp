/* 2152046 大数据 程瑞真 */
#include<iostream>
#include"../include/cmd_console_tools.h"
#include"../include/消除类游戏函数集.h"
#include"90-02-b1.h"

using namespace std;

void input(int& row, int& col, char choice)
{
	while (1) {
		cout << "请输入行数(8-10)：" << endl;
		cin >> row;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (row >= 8 && row <= 10) {
			break;
		}
	}
	while (1) {
		cout << "请输入列数(8-10)：" << endl;
		cin >> col;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (col >= 8 && col <= 10) {
			break;
		}
	}
}

int score(int row, int col, int v[max_row][max_col])
{
	int num = 0;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (v[i][j] == 1)
				num++;
		}
	}
	return num * num * 5;
}

void change(int row, int col, int map[max_row][max_col], int v[max_row][max_col])
{
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (v[i][j] == 1)
				map[i][j] = 0;
		}
	}
}