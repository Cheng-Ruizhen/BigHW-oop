/* 2152046 ��14 ������ */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"../include/��������Ϸ������.h"
#include"90-01-b2.h"

using namespace std;

void print_main_base(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	int sum_score = 0, once_score, max;
	while (1) {
		cout << "��ǰ���飺" << endl;
		print(row, col, map, v, 1);
		cout << endl;
		if (finish(row, col, map)) {
			cout << "�޿�������" << endl;
			break;
		}
		char x, y;
		while (1) {
			cout << "������ĸ+������ʽ[����c2]����������꣺";
			cin >> x >> y;
			if (x >= 'a' && x <= 'z') {
				x -= 32;
			}
			if (cin.fail() || (x<'A' || x>char('A' + row - 1)) || (y<'0' || y>char('0' + col - 1))) {
				cout << "�����������������." << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				continue;
			}
			cout << "����Ϊ" << x << "��" << y << "��" << endl;
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
				cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
				for (int i = 1; i <= row; i++) {
					for (int j = 1; j <= col; j++) {
						v[i][j] = 0;
					}
				}
				continue;
			}
			else {
				cout << endl << "���ҽ�����飺" << endl;
				print(row, col, map, v, 2);
				cout << endl;
				cout << "��ǰ����(��ͬɫ��ʶ)��" << endl;
				print(row, col, map, v, 3);
			}
			break;
		}
		if (choice == '1' || choice == '2') {
			cout << endl;
			break;
		}
		cout << endl << "��ȷ���Ƿ�����ڵ���ֵͬ�ϲ���" << x << y << "��(Y / N / Q)��";
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
			cout << "��ֵͬ�鲢�������(��ͬɫ��ʶ)��" << endl;
			print(row, col, map, v, 3);
			cout << endl << "���ε÷֣�" << once_score << " �ܵ÷֣�" << sum_score << " �ϳ�Ŀ�꣺" << target << endl << endl;
			v[x - 'A' + 1][y - '0' + 1] = 0;
			cout << "���س����������������0����...";
			while (1) {
				char ch = _getch();
				if (ch == '\r') 
					break;
			}
			cout << endl << "��0�������(��ͬɫ��ʶ)��" << endl;
			int a = 0, b = 0;
			array_move(down, single, wid, heig, row, col, map, v, form, false, 0, (char*)" ");
			print(row, col, map, v, 3);
			cout << endl << "���س���������ֵ���...";
			while (1) {
				char ch = _getch();
				if (ch == '\r') 
					break;
			}
			cout << endl << "��ֵ���������(��ͬɫ��ʶ)��" << endl;
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
		cout << endl << "���κϳɽ��������س���������һ�εĺϳ�...";
		while (1) {
			char ch = _getch();
			if (ch == '\r') 
				break;
		}
		cout << endl << endl;
		if (max == target) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(a, b, "�Ѿ��ϳɵ�", COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			cct_getxy(a, b);
			cct_showch(a, b, char(target + '0'), COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			target++;
			cout << endl << "���س������������Ŀ�����..." << endl << endl;
			while (1) {
				char ch = _getch();
				if (ch == '\r')
					break;
			}
		}
	}
}