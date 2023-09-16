/* 2152046 ������ ������ */
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include"../include/cmd_console_tools.h"
#include"../include/��������Ϸ������.h"
#include"90-02-b1.h"

using namespace std;

void print_console(int row, int col, int map[max_row][max_col], char choice)
{
	if (choice == 'D') {
		background(row, col, wid, heig, 1, form);
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				cct_gotoxy(wid * (j - 1) + 4, heig * (i - 1) + 3);
				diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, (char*)"��", form);
			}
		}
		cout << endl << endl;
	}
	else {
		background(row, col, wid, heig, 2, form);
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
				diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, (char*)"��", form);
			}
		}
		cout << endl << endl;
	}
}

int move_and_compound(int row, int col, int map[max_row][max_col], int v[max_row][max_col], char choice,int append)
{
	int operation;
	int ret, maction;
	int keycode1, keycode2;
	int sum_score = 0, once_score;
	int X, Y;
	int old_X = 4, old_Y = 3;

	cct_enable_mouse();

	cct_setcursor(CURSOR_INVISIBLE);	//�رչ��

	cct_showstr(0, (heig + append - 1) * row + 4, "��ͷ��/����ƶ����س���/�������ѡ��Q/�����Ҽ�����");

	int rrow = 1, ccol = 1;
	while (1) {
		operation = read_keybord_and_mouse(row, col, wid, heig, rrow, ccol, old_X, old_Y, map, append, form, true, (char*)"��");
		if (operation == MOUSE_LEFT_BUTTON_CLICK || (char)operation == '\r') {
			cct_setcolor();
			cct_gotoxy(0, (heig + append - 1) * row + 4);
			no_search(row, col, rrow, ccol, map, v);
			cout << "ѡ����" << char(rrow + 'A' - 1) << "��" << ccol - 1 << "��                                      " << endl;
			if (choice == 'D' || choice == 'E')
				return 0;
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
					cct_showstr(0, (heig + append - 1) * row + 4, "ѡ��ľ�������λ�ô���������ֵͬ��������ѡ��");
					v[rrow][ccol] = 0;
					continue;
				}
				for (int i = 1; i <= row; i++) {
					for (int j = 1; j <= col; j++) {
						if (v[i][j] == 1 && map[i][j]) {
							cct_gotoxy((wid + 2 * (append - 1)) * (j - 1) + 4, (heig + append - 1) * (i - 1) + 3);
							diamond(wid, heig, map[i][j] % 6 + 1, COLOR_WHITE, (char*)"��", form);
						}
					}
				}
				cct_showstr(0, (heig + append - 1) * row + 5, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
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
					if (rrow > row)
						rrow = row;
					else if (rrow < 1)
						rrow = 1;
					if (ccol > col)
						ccol = col;
					else if (ccol < 1)
						ccol = 1;
					if (keycode1 == 224) {
						switch (keycode2) {
						case KB_ARROW_UP:
							while (1) {
								if (rrow == 1) {
									rrow = row;
									if (map[rrow][ccol] != 0)
										break;
								}
								else if (rrow > 1) {
									rrow--;
									if (map[rrow][ccol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_DOWN:
							while (1) {
								if (rrow == row) {
									rrow = 1;
									if (map[rrow][ccol] != 0)
										break;
								}
								else if (rrow < row) {
									rrow++;
									if (map[rrow][ccol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_LEFT:
							while (1) {
								if (ccol == 1) {
									ccol = col;
									if (map[rrow][ccol] != 0)
										break;
								}
								else if (ccol > 1) {
									ccol--;
									if (map[rrow][ccol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_RIGHT:
							while (1) {
								if (ccol == col) {
									ccol = 1;
									if (map[rrow][ccol] != 0)
										break;
								}
								else if (ccol < col) {
									ccol++;
									if (map[rrow][ccol] != 0)
										break;
								}
							}
							break;
						}
						cct_setcolor();
						cct_gotoxy(0, (heig + append - 1) * row + 4);
						cout << "[��ǰ����] :";
						cout << char(rrow + 'A' - 1) << "��" << ccol - 1 << "��                                                ";
						cct_gotoxy((wid + 2 * (append - 1)) * (ccol - 1) + 4, (heig + append - 1) * (rrow - 1) + 3);
						diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, (char*)"��", form);
						old_X = (wid + 2 * (append - 1)) * (ccol - 1) + 4, old_Y = (heig + append - 1) * (rrow - 1) + 3;
						for (int i = 1; i <= row; i++) {
							for (int j = 1; j <= col; j++) {
								if ((i != rrow || j != ccol) && map[i][j]) {
									cct_gotoxy((wid + 2 * (append - 1)) * (j - 1) + 4, (heig + append - 1) * (i - 1) + 3);
									diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, (char*)"��", form);
								}
							}
						}
					}
					cct_showstr(0, (heig + append - 1) * row + 5, "                                                ");
					for (int i = 1; i <= row; i++) {
						for (int j = 1; j <= col; j++) {
							v[i][j] = 0;
						}
					}
					continue;
				}
			}
		}
		else if ((char)operation == 'Q' || (char)operation == 'q' || operation == MOUSE_RIGHT_BUTTON_CLICK) {
			cct_setcolor();
			cct_gotoxy(0, (heig + append - 1) * row + 4);
			break;
		}
		cct_showstr(0, (heig + append - 1) * row + 5, "                                                                   ");
		once_score = score(row, col, v);
		sum_score += once_score;
		change(row, col, map, v);
		print_blank(row, col, map);
		cct_setcolor();
		cct_showstr(0, 0, "                                             ");
		cct_gotoxy(0, 0);
		cout << "���ε÷�: " << once_score << " �ܷ�: " << sum_score ;
		if (choice == 'F') {
			cct_showstr(0, (heig + append - 1) * row + 4, "�ϳ���ɣ��س�/�����������0");
			cct_showstr(0, (heig + append - 1) * row + 5, "                                                ");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
					break;
			}
		}
		array_move(down, single, wid, heig, row, col, map, v, form, true, 1, (char*)"��");
		if (choice == 'F') {
			cct_showstr(0, (heig + append - 1) * row + 4, "���κϳɽ�������c/���������ʼ��һ�εĺϳ�");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == 'c' || char(keycode1) == 'C')
					break;
			}
			break;
		}
		array_move(left, together, wid, heig, row, col, map, v, form, true, 1, (char*)"��");
		if ((char)operation == '\r') {
			ccol = 1;
			for (int i = 1; i <= row; i++) {
				if (map[i][1] != 0) {
					rrow = i;
					break;
				}
			}
			cct_gotoxy((wid + 2 * (append - 1)) * (ccol - 1) + 4, (heig + append - 1) * (rrow - 1) + 3);
			diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, (char*)"��", form);
		}
		old_X = (wid + 2 * (append - 1)) * (ccol - 1) + 4, old_Y = (heig + append - 1) * (rrow - 1) + 3;
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				v[i][j] = 0;
			}
		}
		int num;
		if (num = finish(row, col, map)) {
			int a, b;
			cct_showstr(0, (heig + append - 1) * row + 4, "ʣ��", COLOR_BLACK, COLOR_HYELLOW);
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
			cct_setcolor();
			cct_cls();
			srand((unsigned int)(time(NULL)));
			for (int i = 1; i <= row; i++) {
				for (int j = 1; j <= col; j++) {
					map[i][j] = rand() % 5 + 1;
					v[i][j] = 0;
				}
			}
			return 1;
		}
	}
	return 0;
}
void main_console(int row, int col, int map[max_row][max_col], int v[max_row][max_col], char choice)
{
	while (1) {
		int c;
		print_console(row, col, map, choice);
		cct_gotoxy(4, 3);
		diamond(wid, heig, map[1][1] % 6 + 1, COLOR_WHITE, (char*)"��", form);
		if (choice == 'D') {
			move_and_compound(row, col, map, v, choice, 1);
			break;
		}
		else {
			c = move_and_compound(row, col, map, v, choice, 2);
			if (!c)
				break;
		}
	}
}