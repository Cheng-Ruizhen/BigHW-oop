/* 2152046 ������ ������ */
#include<iostream>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"../include/��������Ϸ������.h"
#include"90-02-b1.h"

using namespace std;

void main_base(int row, int col,int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	int once_score, sum_score = 0;
	while (1) {
		cout << "��ǰ���飺" << endl;
		print(row, col, map, v, 1);
		cout << endl;
		int num;
		if (num = finish(row, col, map)) {
			int a, b;
			cct_getxy(a, b);
			cct_showstr(a, b, "ʣ��", COLOR_BLACK, COLOR_HYELLOW);
			cct_getxy(a, b);
			cct_showint(a, b, num, COLOR_BLACK, COLOR_HYELLOW);
			cct_getxy(a, b);
			cct_showstr(a, b, "�����ǣ��޿���������ؽ�����", COLOR_BLACK, COLOR_HYELLOW);
			cout << "���س�������һ��";
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
		if (choice == 'A') {
			cout << endl;
			break;
		}
		cout << endl << "��ȷ���Ƿ��"<<x<<y<<"����Χ����ֵͬ����(Y / N / Q)��";
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
			cout << "��ֵͬ�鲢�������(��ͬɫ��ʶ)��" << endl;
			print(row, col, map, v, 3);
			cout << endl << "���ε÷֣�" << once_score << " �ܵ÷֣�" << sum_score << endl << endl;
			cout << "���س����������������0����...";
			while (1) {
				char ch = _getch();
				if (ch == '\r')
					break;
			}
			cout << endl << "���������飺" << endl;
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
		cout << endl << "���κϳɽ��������س���������һ�εĺϳ�..." << endl;
		while (1) {
			char ch = _getch();
			if (ch == '\r')
				break;
		}
	}
}