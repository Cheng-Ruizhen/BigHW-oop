/* 2152046 ������ ������ */
#include <iostream>
#include<Windows.h>
#include "../include/cmd_console_tools.h"
#include"../include/menu.h"
#include"../include/��������Ϸ������.h"
#include"90-02-b1.h"
using namespace std;

int main()
{
	const char option[][100] = {
		"A.�������ҳ����������ʶ",
		"B.���������һ���������ֲ�����ʾ��",
		"C.���������һ�أ��ֲ�����ʾ��",
		"D.αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
		"E.αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�",
		"F.αͼ�ν������һ���������ֲ��裩",
		"G.αͼ�ν���������",
		"Q.�˳�"
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
     
		/*   ��ʼ������   */
		srand((unsigned int)(time(NULL)));
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				map[i][j] = rand() % 5 + 1;
			}
		}

		/*   ����ѡ��ȷ�������л���αͼ��   */
		if (choice >= 'A' && choice <= 'C')
			main_base(row, col, map, v, choice);
		else if (choice >= 'D' && choice <= 'G')
			main_console(row, col, map, v, choice);

		while (1) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(0, b, "                                                        ");
			cct_showstr(0, b, "��С�������������End����... ");
			char k[100];
			cin >> k;
			if (tj_strcasecmp(k, "End") != 0) {
				cout << "�����������������" << endl;
				continue;
			}
			break;
		}
		cct_cls();
	}
	return 0;
}
