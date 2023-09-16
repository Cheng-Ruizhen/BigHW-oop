/* 2152046 ��14 ������ */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
#include"../include/cmd_console_tools.h"
#include"../include/��������Ϸ������.h"
#include"90-01-b2.h"

using namespace std;

void print_console(int row, int col, int map[max_row][max_col], char choice)
{
	char ch[(wid - 4) + 1] = { '0' };
	if (choice == '5') {
		background(row, col, wid, heig, 1, form);
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				cct_gotoxy(wid * (j - 1) + 4, heig * (i - 1) + 3);
				int_to_char(map[i][j], ch);
				diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
			}
		}
		cout << endl << endl;
	}
	else {
		background(row, col, wid, heig, 2, form);
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
				int_to_char(map[i][j], ch);
				diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
			}
		}
		cout << endl << endl;
	}
}

void move_and_compound(int row, int col, int target,int map[max_row][max_col],int v[max_row][max_col],char choice)
{
	int X = 4, Y = 3;
	int ret, maction;
	int keycode1, keycode2;
	int sum_score = 0, once_score;
	int operation;
	char ch[(wid - 4) + 1] = { '0' };

	cct_enable_mouse();

	cct_setcursor(CURSOR_INVISIBLE);	//�رչ��
	cct_showstr(0, (heig + 1) * row + 4, "��ͷ��/����ƶ����س���/�������ѡ��Q/�����Ҽ�����");

	int old_X = 4, old_Y = 3;
	int rrow = 1, ccol = 1;
	while (1) {
		operation = read_keybord_and_mouse(row, col, wid, heig, rrow, ccol, old_X, old_Y, map, 2, form, 0, (char*)" ");
		if (operation == MOUSE_LEFT_BUTTON_CLICK || (char)operation == '\r') {
			cct_setcolor();
			cct_gotoxy(0, (heig + 1) * row + 4);
			no_search(row, col, rrow, ccol, map, v);
			cout << "ѡ����" << char(rrow + 'A' - 1) << "��" << ccol - 1 << "��                                      " << endl;
			if (choice == '7')
				break;
			else {
				no_search(row, col, rrow, ccol, map, v);
				int num = 0;
				for (int i = 1; i <= row; i++) {
					for (int j = 1; j <= col; j++) {
						if (v[i][j] != 0) {
							num++;
						}
					}
				}
				if (num == 1) {
					cct_showstr(0, (heig + 1) * row + 4, "ѡ��ľ�������λ�ô���������ֵͬ��������ѡ��");
					v[rrow][ccol] = 0;
					continue;
				}
				for (int i = 1; i <= row; i++) {
					for (int j = 1; j <= col; j++) {
						if (v[i][j] == 1 && map[i][j]) {
							cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
							int_to_char(map[i][j], ch);
							diamond(wid, heig, map[i][j] % 6 + 1, COLOR_WHITE, ch, form);
						}
					}
				}
				cct_showstr(0, (heig + 1) * row + 5, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
				if (operation == MOUSE_LEFT_BUTTON_CLICK) {
					keycode1 = 0;
					ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
					ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				}
				else {
					maction = 0;
					ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				}
				if (!(maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || char(keycode1) == '\r')) {
					if (keycode1 == 224) {
						switch (keycode2) {
							case KB_ARROW_UP:
								if (rrow > 1 && rrow <= row)
									rrow--;
								else
									rrow = row;
								break;
							case KB_ARROW_DOWN:
								if (rrow >= 1 && rrow < row)
									rrow++;
								else
									rrow = 1;
								break;
							case KB_ARROW_LEFT:
								if (ccol > 1 && ccol <= col)
									ccol--;
								else
									ccol = col;
								break;
							case KB_ARROW_RIGHT:
								if (ccol >= 1 && ccol < col)
									ccol++;
								else
									ccol = 1;
								break;
						}
						cct_setcolor();
						cct_gotoxy(0, (heig + 1) * row + 4);
						cout << "[��ǰ����] :";
						cout << char(rrow + 'A' - 1) << "��" << ccol - 1 << "��                                                ";
						cct_gotoxy((wid + 2) * (ccol - 1) + 4, (heig + 1) * (rrow - 1) + 3);
						int_to_char(map[rrow][ccol], ch);
						diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
						old_X = (wid + 2) * (ccol - 1) + 4, old_Y = (heig + 1) * (rrow - 1) + 3;
						for (int i = 1; i <= row; i++) {
							for (int j = 1; j <= col; j++) {
								if ((i != rrow || j != ccol) && map[i][j]) {
									cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
									int_to_char(map[i][j], ch);
									diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
								}
							}
						}
					}
					for (int i = 1; i <= row; i++) {
						for (int j = 1; j <= col; j++) {
							v[i][j] = 0;
						}
					}
					cct_showstr(0, (heig + 1) * row + 5, "                                                ");
					continue;
				}
			}
		}
		else if ((char)operation == 'Q' || (char)operation == 'q' || operation == MOUSE_RIGHT_BUTTON_CLICK) {
			cct_setcolor();
			cct_gotoxy(0, (heig + 1)* row + 4);
			break;
		}
		cct_showstr(0, 4 * row + 5, "                                                                   ");
		once_score = score(row, col, map, v);
		sum_score += once_score;
		int max = change(row, col, rrow, ccol, map, v);
		print_blank(row, col, map);
		cct_setcolor();
		cct_showstr(0, 0, "                                             ");
		cct_gotoxy(0, 0);
		cout << "���ε÷�: " << once_score << " �ܷ�: " << sum_score << " �ϳ�Ŀ��: " << target;
		if (choice == '8') {
			cct_showstr(8 * ccol - 4, 4 * rrow - 1, "������", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 4, 4 * rrow, "��", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showch(8 * ccol - 2, 4 * rrow, (char)(map[rrow][ccol] + '0'), map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 1, 4 * rrow, " ��", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 4, 4 * rrow + 1, "������", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(0, 4 * row + 4, "�ϳ���ɣ��س�/�����������0");
			cct_showstr(0, 4 * row + 5, "                                                ");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
					break;
			}
		}
		cct_gotoxy(8 * ccol - 4, 4 * rrow - 1);
		char ch[(wid - 4) + 1] = { '\0' };
		int_to_char(map[rrow][ccol], ch);
		diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_BLACK, ch, form);
		array_move(down, single, wid, heig, row, col, map, v, form, true, 0, ch);
		old_X = 8 * ccol - 6, old_Y = 4 * rrow - 1;
		if (choice == '8') {
			cct_showstr(0, 4 * row + 4, "����0��ɣ��س�/������������ֵ");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
					break;
			}
		}
		fill(row, col, max, map, '8');
		if (choice == '8') {
			cct_showstr(0, 4 * row + 4, "���κϳɽ�������c/���������ʼ��һ�εĺϳ�");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == 'c' || char(keycode1) == 'C')
					break;
			}
		}
		old_X = (wid + 2) * (ccol - 1) + 4, old_Y = (heig + 1) * (rrow - 1) + 3;
		if ((char)operation == '\r') {
			cct_gotoxy(old_X, old_Y);
			int_to_char(map[rrow][ccol], ch);
			diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
		}
		if (max == target) {
			cct_showstr(0, 4 * row + 6, "�Ѿ��ϳɵ�", COLOR_HYELLOW, COLOR_RED);
			cct_showint(10, 4 * row + 6, target, COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			target++;
			cout << endl << "���س����򵥻�������������Ŀ�����..." << endl << endl;
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
					break;
			}
		}
		for (int i = 4; i <= 7; i++) {
			cct_showstr(0, 4 * row + i, "                                                                      ", COLOR_BLACK, COLOR_BLACK);
		}
		if (choice == '8')
			break;
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				v[i][j] = 0;
			}
		}
		if (finish(row, col, map)) {
			cct_showstr(0, 4 * row + 4, "�޿��������Ϸ����");
			break;
		}
	}
}

void print_main_console(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	print_console(row, col, map, choice);
	if (choice == '5' || choice == '6')
		return;
	cct_gotoxy(4, 3);
	char ch[(wid - 4) + 1] = { '\0' };
	int_to_char(map[1][1], ch);
	diamond(wid, heig, map[1][1] % 6 + 1, COLOR_WHITE, ch, form);
	move_and_compound(row, col,target, map, v, choice);
}