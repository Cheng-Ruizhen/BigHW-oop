/* 2152046 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ӧ��ֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void input(int& row, int& col, int& target)
{
	while (1) {
		cout << "����������(5-8)��" << endl;
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
		cout << "����������(5-10)��" << endl;
		cin >> col;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (col >= 5 && col <= 10) {
			break;
		}
	}
	while (1) {
		cout << "������ϳ�Ŀ��(5-20)��" << endl;
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

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ǵݹ鷽ʽ������λ����ͬ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void no_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col])
{
	int xx[4] = { 1,-1,0,0 };
	int yy[4] = { 0,0,1,-1 };
	struct ss {
		int x;
		int y;
	}line[1000];
	int l = 0, r = 1;
	line[1].x = my_row;
	line[1].y = my_col;
	v[my_row][my_col] = 1;
	while (l < r) {
		l++;
		int newx, newy;
		for (int i = 0; i < 4; i++) {
			newx = line[l].x + xx[i];
			newy = line[l].y + yy[i];
			if (newx >= 0 && newx < row && newy >= 0 && newy < col) {
				if ((map[newx][newy] == map[line[l].x][line[l].y]) && (!v[newx][newy])) {
					r++;
					line[r].x = newx;
					line[r].y = newy;
					v[newx][newy] = 1;
				}
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж�������Ϸ�Ƿ����
  ���������
  �� �� ֵ��
  ˵    ��������ֵΪ0����ʾ���п���������򷵻ػ��м���Ϊδ������
***************************************************************************/
int finish(int row, int col, int map[max_row][max_col])
{
	int xx[4] = { 1,-1,0,0 };
	int yy[4] = { 0,0,1,-1 };
	int num = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j] != 0)
				num++;
			int newi, newj;
			for (int k = 0; k < 4; k++) {
				newi = i + xx[k];
				newj = j + yy[k];
				if (map[i][j] != 0 && map[i][j] == map[newi][newj])
					return 0;
			}
		}
	}
	return num;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int score(int row, int col, int map[max_row][max_col], int v[max_row][max_col])
{
	int num = 0, x = 0, y = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (v[i][j] == 1) {
				num++;
				x = i, y = j;
			}
		}
	}
	return 3 * num * map[x][y];
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ѡ��ɫ���Ӧ����ı仯
  ���������
  �� �� ֵ��
  ˵    �������ر仯��������е����ֵ
***************************************************************************/
int change(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col])
{
	int max = map[0][0];
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
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

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����仯�󣬲�����ɫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void fill(const CONSOLE_GRAPHICS_INFO* Mto10_CGI,int row, int col, int max, int map[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi)
{
	srand((unsigned int)(time(NULL)));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
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
				gmw_draw_block(Mto10_CGI, i, j, map[i][j], bdi);
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ڲ�������ƶ�
  ���������
  �� �� ֵ��
  ˵    �����ϳ�ʮ������ȫΪ�����ƶ�
***************************************************************************/
void array_move(CONSOLE_GRAPHICS_INFO* pMto10_CGI, int row, int col, int map[max_row][max_col], int v[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi)
{
	for (int j = 0; j < col; j++) {
		for (int i = row - 2; i >= 0; i--) {
			if (map[i][j] != 0) {
				int x;
				for (x = i + 1; x <= row; x++) {
					if (x == 0 || x == row || map[x][j] != 0)
						break;
				}
				if (i != (x - 1)) {
					gmw_move_block(pMto10_CGI, i, j, map[i][j], 0, bdi, UP_TO_DOWN, abs(x - 1 - i));
					map[x - 1][j] = map[i][j], map[i][j] = 0;
					v[x - 1][j] = 0, v[i][j] = 1;
				}
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ϳ�ʮ�ľ���ʵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void step_of_merge_to_10(CONSOLE_GRAPHICS_INFO* pMto10_CGI, int row, int col, int map[max_row][max_col], int v[max_row][max_col], int target)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//����1�ġ�������״̬
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{6,  COLOR_HBLUE,  COLOR_BLACK,  NULL},
		{7,  COLOR_HGREEN, COLOR_BLACK,  NULL},
		{8,  COLOR_HCYAN,  COLOR_BLACK,  NULL},
		{9,  COLOR_HRED,   COLOR_BLACK,  NULL},
		{10, COLOR_HPINK,  COLOR_BLACK,  NULL},	//����10�ġ�������״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//����1�ġ���ѡ��+������״̬
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{6,  COLOR_HBLUE,  COLOR_WHITE,  NULL},
		{7,  COLOR_HGREEN, COLOR_WHITE,  NULL},
		{8,  COLOR_HCYAN,  COLOR_WHITE,  NULL},
		{9,  COLOR_HRED,   COLOR_WHITE,  NULL},
		{10, COLOR_HPINK,  COLOR_WHITE,  NULL},	//����10�ġ���ѡ��+������״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//����1�ġ�ѡ�С�״̬
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{6, COLOR_HBLUE,  COLOR_HWHITE, NULL},
		{7, COLOR_HGREEN, COLOR_HWHITE, NULL},
		{8, COLOR_HCYAN,  COLOR_HWHITE, NULL},
		{9, COLOR_HRED,   COLOR_HWHITE, NULL},
		{10,COLOR_HPINK,  COLOR_HWHITE, NULL},	//����10�ġ�ѡ�С�״̬�������Ҫ�������Լ�������11���Ժ������
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};

	char temp[256];
	int i, j;

	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(pMto10_CGI, row, col);

	/* ��ʾ��� */
	gmw_draw_frame(pMto10_CGI);

	/* ��״̬����ʾ���� */
	sprintf(temp, "���ڴ�С��%d�� %d��", pMto10_CGI->lines, pMto10_CGI->cols);
	gmw_status_line(pMto10_CGI, TOP_STATUS_LINE, temp);

	/* ���ڲ�����չ�ֵ���Ļ�� */
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(pMto10_CGI, i, j, map[i][j], bdi_normal);
		}

	/* �����/���̲����ݷ���ֵ������Ӧ�Ķ���
	   ע��1������Լ������Ҽ��˳��������ʾ<�У���>��ʵ����Ϸ�У�Ҫ���ݷ��ص����н���ĳ�ֶ������ҳ���ͬ�����ѡ��������ȣ�
		   2������Լ�����س��˳���������ʾ������Ϣ��ʵ����Ϸ�У�Ҫ���ݷ��ص����н���ĳ�ֶ������ҳ���ͬ�����ѡ��������ȣ�
		   3��ʵ����Ϸ��ӦдΪ���� */
	if (pMto10_CGI->inited == CGI_INITED) {
		int once_score, sum_score = 0;
		int mrow = 0, mcol = 0;

		while (1) {
			if (finish(row, col, map)) {
				gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "�޿��������Ϸ����");
				break;
			}

			int loop = 1;
			int maction, old_mrow, old_mcol;
			int keycode1, keycode2;
			int ret;

			gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);

			while (loop) {
				old_mrow = mrow;
				old_mcol = mcol;
				ret = gmw_read_keyboard_and_mouse(pMto10_CGI, maction, mrow, mcol, keycode1, keycode2);

				if (ret == CCT_MOUSE_EVENT) {
					if (maction == MOUSE_ONLY_MOVED) {
						/* ��ʱ��mrow��mcol�϶����ղŲ�ͬ */

						/* ԭɫ��������ʾ */
						if (old_mrow >= 0 && old_mcol >= 0)
							gmw_draw_block(pMto10_CGI, old_mrow, old_mcol, map[old_mrow][old_mcol], bdi_normal);

						/* ��ɫ������ */
						gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
					}
					else if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
						/* ��״̬����ʾ���� */
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "��Ϸ����", NULL);
						return;
					}
					else {
						sprintf(temp, "[�������] %c��%d��", char('A' + mrow), mcol); //δ����mrow����26��mcol����99�������������Ҫ�������д���
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						loop = 0;
					}
				}
				else { //CCT_KEYBOARD_EVENT - ������뺬��ȥ�� cmd_console_tools.cpp
					switch (keycode1) {
					case 0x0D:
						/* ��״̬����ʾ���� */
						sprintf(temp, "[�����س���] %c��%d��", char('A' + mrow), mcol);
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						loop = 0;
						break;
					case 'q':
					case 'Q':
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "��Ϸ����", NULL);
						return;
					case 0xE0: //224
						sprintf(temp, "[���������]");
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						if (mrow > row - 1)
							mrow = row - 1;
						else if (mrow < 0)
							mrow = 0;
						if (mcol > col - 1)
							mcol = col - 1;
						else if (mcol < 0)
							mcol = 0;
						switch (keycode2) {
						case KB_ARROW_UP:
							while (1) {
								if (mrow == 0) {
									mrow = row - 1;
									if (map[mrow][mcol] != 0)
										break;
								}
								else if (mrow > 0) {
									mrow--;
									if (map[mrow][mcol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_DOWN:
							while (1) {
								if (mrow == row - 1) {
									mrow = 0;
									if (map[mrow][mcol] != 0)
										break;
								}
								else if (mrow < row - 1) {
									mrow++;
									if (map[mrow][mcol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_LEFT:
							while (1) {
								if (mcol == 0) {
									mcol = col - 1;
									if (map[mrow][mcol] != 0)
										break;
								}
								else if (mcol > 0) {
									mcol--;
									if (map[mrow][mcol] != 0)
										break;
								}
							}
							break;
						case KB_ARROW_RIGHT:
							while (1) {
								if (mcol == col - 1) {
									mcol = 0;
									if (map[mrow][mcol] != 0)
										break;
								}
								else if (mcol < col - 1) {
									mcol++;
									if (map[mrow][mcol] != 0)
										break;
								}
							}
							break;
						}
						/* ԭɫ��������ʾ */
						if (old_mrow >= 0 && old_mcol >= 0)
							gmw_draw_block(pMto10_CGI, old_mrow, old_mcol, map[old_mrow][old_mcol], bdi_normal);

						/* ��ɫ������ */
						gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
						break;
					default:
						sprintf(temp, "[��������] %d/%d", keycode1, keycode2);
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						break;
					}//end of switch
				}//end of else
			}//end of while

			/* �˳�ѭ��,˵���Ѿ�ѡ�������� */
			no_search(row, col, mrow, mcol, map, v);
			int num = 0;
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (v[i][j] != 0) {
						num++;
					}
				}
			}
			if (num == 1) {
				gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "ѡ��ľ�������λ�ô���������ֵͬ��������ѡ��");
				v[mrow][mcol] = 0;
				continue;
			}
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (v[i][j] == 1 && map[i][j])
						gmw_draw_block(pMto10_CGI, i, j, map[i][j], bdi_related);
				}
			}
			cct_showstr(pMto10_CGI->SLI.lower_start_x, pMto10_CGI->SLI.lower_start_y + 1, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
			int X, Y;
			if (maction == MOUSE_LEFT_BUTTON_CLICK) {
				keycode1 = 0;
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			}
			else {
				maction = 0;
				ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
			}
			if (!(maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || char(keycode1) == '\r')) { //ȡ����ѡ��
				if (keycode1 == 224) {
					if (mrow > row - 1)
						mrow = row - 1;
					else if (mrow < 0)
						mrow = 0;
					if (mcol > col - 1)
						mcol = col - 1;
					else if (mcol < 0)
						mcol = 0;
					switch (keycode2) {
					case KB_ARROW_UP:
						while (1) {
							if (mrow == 0) {
								mrow = row - 1;
								if (map[mrow][mcol] != 0)
									break;
							}
							else if (mrow > 0) {
								mrow--;
								if (map[mrow][mcol] != 0)
									break;
							}
						}
						break;
					case KB_ARROW_DOWN:
						while (1) {
							if (mrow == row - 1) {
								mrow = 0;
								if (map[mrow][mcol] != 0)
									break;
							}
							else if (mrow < row - 1) {
								mrow++;
								if (map[mrow][mcol] != 0)
									break;
							}
						}
						break;
					case KB_ARROW_LEFT:
						while (1) {
							if (mcol == 0) {
								mcol = col - 1;
								if (map[mrow][mcol] != 0)
									break;
							}
							else if (mcol > 0) {
								mcol--;
								if (map[mrow][mcol] != 0)
									break;
							}
						}
						break;
					case KB_ARROW_RIGHT:
						while (1) {
							if (mcol == col - 1) {
								mcol = 0;
								if (map[mrow][mcol] != 0)
									break;
							}
							else if (mcol < col - 1) {
								mcol++;
								if (map[mrow][mcol] != 0)
									break;
							}
						}
						break;
					}
					/* ��ɫ������ */
					gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
					keycode1 = 0;
				}
				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						if (v[i][j])
							gmw_draw_block(pMto10_CGI, i, j, map[i][j], bdi_normal);
						v[i][j] = 0;
					}
				}
				cct_showstr(pMto10_CGI->SLI.lower_start_x, pMto10_CGI->SLI.lower_start_y + 1, "                                                ");
				continue;
			}
			else //ȷ��ѡ��
			{
				cct_showstr(pMto10_CGI->SLI.lower_start_x, pMto10_CGI->SLI.lower_start_y + 1, "                                                ");
				once_score = score(row, col, map, v);
				sum_score += once_score;
				int max = change(row, col, mrow, mcol, map, v);
				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						if (map[i][j] == 0) {
							gmw_draw_block(pMto10_CGI, i, j, BDI_VALUE_BLANK, bdi_normal);
						}
					}
				}
				sprintf(temp, "���ε÷�: %d �ܷ�: %d �ϳ�Ŀ��: %d", once_score, sum_score, target);
				gmw_status_line(pMto10_CGI, TOP_STATUS_LINE, temp);

				/* ���ѡ����Ǹ�ɫ���ƶ����λ�� */
				int new_row = mrow, new_col = mcol, j;
				for (j = mrow + 1; j < row; j++) {
					if (map[j][mcol] != 0) {
						new_row = j - 1;
						break;
					}
				}
				if (j == row)
					new_row = row - 1;

				array_move(pMto10_CGI, row, col, map, v, bdi_normal);
				if (ret == CCT_KEYBOARD_EVENT)
				{
					mrow = new_row, mcol = new_col;
					gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
				}
				fill(pMto10_CGI, row, col, max, map, bdi_normal);
				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						v[i][j] = 0;
					}
				}

				if (max == target) {
					sprintf(temp, "�Ѿ��ϳɵ�%d ", target);
					gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "���س����򵥻�������������Ŀ�����...", temp);
					target++;
					while (1) {
						ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
						if (maction == MOUSE_LEFT_BUTTON_CLICK || char(keycode1) == '\r')
						{
							gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "                                                    ");
							break;
						}
					}
				}
			}
		}
	}
}