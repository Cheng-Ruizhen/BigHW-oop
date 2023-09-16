/* 2152046 信14 程瑞真 */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
#include"../include/cmd_console_tools.h"
#include"../include/消除类游戏函数集.h"
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

	cct_setcursor(CURSOR_INVISIBLE);	//关闭光标
	cct_showstr(0, (heig + 1) * row + 4, "箭头键/鼠标移动，回车键/单击左键选择，Q/单击右键结束");

	int old_X = 4, old_Y = 3;
	int rrow = 1, ccol = 1;
	while (1) {
		operation = read_keybord_and_mouse(row, col, wid, heig, rrow, ccol, old_X, old_Y, map, 2, form, 0, (char*)" ");
		if (operation == MOUSE_LEFT_BUTTON_CLICK || (char)operation == '\r') {
			cct_setcolor();
			cct_gotoxy(0, (heig + 1) * row + 4);
			no_search(row, col, rrow, ccol, map, v);
			cout << "选中了" << char(rrow + 'A' - 1) << "行" << ccol - 1 << "列                                      " << endl;
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
					cct_showstr(0, (heig + 1) * row + 4, "选择的矩阵坐标位置处无连续相同值，请重新选择");
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
				cct_showstr(0, (heig + 1) * row + 5, "箭头键/鼠标移动取消当前选择，回车键/单击左键合成");
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
						cout << "[当前键盘] :";
						cout << char(rrow + 'A' - 1) << "行" << ccol - 1 << "列                                                ";
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
		cout << "本次得分: " << once_score << " 总分: " << sum_score << " 合成目标: " << target;
		if (choice == '8') {
			cct_showstr(8 * ccol - 4, 4 * rrow - 1, "┌─┐", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 4, 4 * rrow, "│", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showch(8 * ccol - 2, 4 * rrow, (char)(map[rrow][ccol] + '0'), map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 1, 4 * rrow, " │", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(8 * ccol - 4, 4 * rrow + 1, "└─┘", map[rrow][ccol]%6 + 1, COLOR_WHITE);
			cct_showstr(0, 4 * row + 4, "合成完成，回车/单击左键下落0");
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
			cct_showstr(0, 4 * row + 4, "下落0完成，回车/单击左键填充新值");
			while (1) {
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
					break;
			}
		}
		fill(row, col, max, map, '8');
		if (choice == '8') {
			cct_showstr(0, 4 * row + 4, "本次合成结束，按c/单击左键开始新一次的合成");
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
			cct_showstr(0, 4 * row + 6, "已经合成到", COLOR_HYELLOW, COLOR_RED);
			cct_showint(10, 4 * row + 6, target, COLOR_HYELLOW, COLOR_RED);
			cct_setcolor();
			target++;
			cout << endl << "按回车键或单击左键继续向更高目标进发..." << endl << endl;
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
			cct_showstr(0, 4 * row + 4, "无可消除项，游戏结束");
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