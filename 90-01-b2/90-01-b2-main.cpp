/* 2152046 ��14 ������ */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
#include<string.h>
#include"../include/cmd_console_tools.h"
#include"../include/menu.h"
#include"../include/��������Ϸ������.h"
#include"90-01-b2.h"
using namespace std;
int main()
{
	const char option[][100] = {
		"1.�������ҳ��ɺϳ����ʶ���ǵݹ�)",
		"2.�������ҳ��ɺϳ����ʶ���ݹ飩",
		"3.���������һ�κϳɣ��ֲ�����ʾ��",
		"4.�����������棨�ֲ�����ʾ��",
		"5.αͼ�ν�����ʾ��ʼ���飨�޷ָ��ߣ�",
		"6.αͼ�ν�����ʾ��ʼ���飨�зָ��ߣ�",
		"7.αͼ�ν������ü�ͷ��/���ѡ��ǰɫ��",
		"8.αͼ�ν������һ�κϳɣ��ֲ��裩",
		"9.αͼ�ν��������棨֧����꣩",
		"0.�˳�"
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

			/*   ��ʼ������   */
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
				cct_showstr(0, b, "��С�������������End����... ");
				char sta[4] = { 'E','N','D','\0' };
				char k[100] = {'\0'};
				cin >> k;
				if (tj_strcasecmp(k, "End") != 0) {
					cout << "�����������������" << endl;
					continue;
				}
				break;
			}
		}
		cct_cls();
	}
}