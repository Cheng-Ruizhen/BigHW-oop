/* 2152046 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cmath>
#include<iomanip>
#include"../include/cmd_console_tools.h"
#include"../include/cmd_gmw_tools.h"
#include"90-02-b2.h"

using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ӧ��ֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void input(int& row, int& col, int& target, int& delay)
{
	CONSOLE_GRAPHICS_INFO G2048_CGI;
	gmw_init(&G2048_CGI, 15, 15, COLOR_BLACK, COLOR_HWHITE);
	gmw_set_frame_style(&G2048_CGI, 3, 2, false);
	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
	gmw_set_font(&G2048_CGI, "������", 16);
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);
	gmw_draw_frame(&G2048_CGI);

	cct_showstr(G2048_CGI.start_x + 3, G2048_CGI.start_y + 2, "��Ϸ2048��������", COLOR_WHITE, COLOR_BLACK);
	int y = G2048_CGI.start_y + 3;
	while (1) {
		cct_showstr(G2048_CGI.start_x + 3, y, "����������[4-8] ", COLOR_WHITE, COLOR_BLACK);
		y++;
		cin >> row;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (row >= 4 && row <= 8) {
			break;
		}
	}
	while (1) {
		cct_showstr(G2048_CGI.start_x + 3, y, "����������[4-10] ", COLOR_WHITE, COLOR_BLACK);
		y++;
		cin >> col;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (col >= 4 && col <= 10) {
			break;
		}
	}
	int min_target = 1;
	for (int i = 0; i < min(row, col) + 5; i++)
		min_target *= 2;
	char temp[256];
	sprintf(temp, "������Ŀ�����[%d/%d/%d/%d] ", min_target, min_target * 2, min_target * 4, min_target * 8);
	while (1) {
		cct_showstr(G2048_CGI.start_x + 3, y, temp, COLOR_WHITE, COLOR_BLACK);
		y++;
		cin >> target;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (target == min_target || target == min_target * 2 || target == min_target * 4 || target == min_target * 8) {
			break;
		}
	}
	while (1) {
		cct_showstr(G2048_CGI.start_x + 3, y, "�����붯����ʱ[0-5] ", COLOR_WHITE, COLOR_BLACK);
		y++;
		cin >> delay;
		if (!cin.good()) {
			cin.ignore(10000, '\n');
			cin.clear();
		}
		if (delay >= 0 && delay <= 5)
			break;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ڲ�������ƶ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void array_move(CONSOLE_GRAPHICS_INFO* G2048_CGI, int row, int col, int g2048[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi,int direction,int &score,int &num)
{
	bool vis[max_row][max_col] = { 0 }; //���״̬���ж��Ƿ��Ѿ��ϳɹ�
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			vis[i][j] = 0;
		}
	}

	if (direction == UP_TO_DOWN) //�����ƶ�
	{
		for (int j = 0; j < col; j++) {
			for (int i = row - 2; i >= 0; i--) {
				if (g2048[i][j] != 0) {
					int x;
					for (x = i + 1; x < row; x++) {
						if (g2048[x][j] != 0)
							break;
					}
					if (x != i + 1) {
						if (x == row || g2048[i][j] != g2048[x][j]) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - i - 1);
							g2048[x - 1][j] = g2048[i][j], g2048[i][j] = 0;
							vis[x - 1][j] = vis[i][j], vis[i][j] = 0;
						}
						else {
							if (vis[i][j] == 0 && vis[x][j] == 0) {
								gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - i);
								g2048[x][j] += g2048[i][j], g2048[i][j] = 0, vis[x][j] = 1;
								gmw_draw_block(G2048_CGI, x, j, g2048[x][j], bdi);
								score += g2048[x][j];
							}
							else {
								gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - i - 1);
								g2048[x - 1][j] = g2048[i][j], g2048[i][j] = 0;
								vis[x - 1][j] = vis[i][j], vis[i][j] = 0;
							}
						}
						num++;
					}
					else {
						if (g2048[i][j] == g2048[x][j] && vis[i][j] == 0 && vis[x][j] == 0) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - i);
							g2048[x][j] += g2048[i][j], g2048[i][j] = 0, vis[x][j] = 1;
							gmw_draw_block(G2048_CGI, x, j, g2048[x][j], bdi);
							score += g2048[x][j];
							num++;
						}
					}
				}
			}
		}
	}
	else if (direction == DOWN_TO_UP) //�����ƶ�
	{
		for (int j = 0; j < col; j++) {
			for (int i = 1; i < row; i++) {
				if (g2048[i][j] != 0) {
					int x;
					for (x = i - 1; x >= 0; x--) {
						if (g2048[x][j] != 0)
							break;
					}
					if (x == -1 || (g2048[i][j] != g2048[x][j] && x != i - 1)) {
						gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, i - 1 - x);
						g2048[x + 1][j] = g2048[i][j], g2048[i][j] = 0;
						vis[x + 1][j] = vis[i][j], vis[i][j] = 0;
						num++;
					}
					else if(g2048[i][j] == g2048[x][j]) {
						if (vis[i][j] == 0 && vis[x][j] == 0) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, i - x);
							g2048[x][j] += g2048[i][j], g2048[i][j] = 0, vis[x][j] = 1;
							gmw_draw_block(G2048_CGI, x, j, g2048[x][j], bdi);
							score += g2048[x][j];
							num++;
						}
						else if(x != i - 1) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, i - x - 1);
							g2048[x + 1][j] = g2048[i][j], g2048[i][j] = 0;
							vis[x + 1][j] = vis[i][j], vis[i][j] = 0;
							num++;
						}
					}
				}
			}
		}
	}
	else if (direction == LEFT_TO_RIGHT) //�����ƶ�
	{
		for (int i = 0; i < row; i++) {
			for (int j = col - 2; j >= 0; j--) {
				if (g2048[i][j] != 0) {
					int x;
					for (x = j + 1; x < col; x++) {
						if (g2048[i][x] != 0)
							break;
					}
					if (x == col || (g2048[i][x] != g2048[i][j] && x != j + 1)) {
						gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - j - 1);
						g2048[i][x - 1] = g2048[i][j], g2048[i][j] = 0;
						vis[i][x - 1] = vis[i][j], vis[i][j] = 0;
						num++;
					}
					else if (g2048[i][x] == g2048[i][j]) {
						if (vis[i][j] == 0 && vis[i][x] == 0) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - j);
							g2048[i][x] += g2048[i][j], g2048[i][j] = 0, vis[i][x] = 1;
							gmw_draw_block(G2048_CGI, i, x, g2048[i][x], bdi);
							score += g2048[i][x];
							num++;
						}
						else if (x != j + 1) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, x - j - 1);
							g2048[i][x - 1] = g2048[i][j], g2048[i][j] = 0;
							vis[i][x - 1] = vis[i][j], vis[i][j] = 0;
							num++;
						}
					}
				}
			}
		}
	}
	else if (direction == RIGHT_TO_LEFT) //�����ƶ�
	{
		for (int i = 0; i < row; i++) {
			for (int j = 1; j < col; j++) {
				if (g2048[i][j] != 0) {
					int x;
					for (x = j - 1; x >= 0; x--) {
						if (g2048[i][x] != 0)
							break;
					}
					if (x == -1 || (g2048[i][x] != g2048[i][j] && x != j - 1)) {
						gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, j - 1 - x);
						g2048[i][x + 1] = g2048[i][j], g2048[i][j] = 0;
						vis[i][x + 1] = vis[i][j], vis[i][j] = 0;
						num++;
					}
					else if (g2048[i][x] == g2048[i][j]) {
						if (vis[i][j] == 0 && vis[i][x] == 0) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, j - x);
							g2048[i][x] += g2048[i][j], g2048[i][j] = 0, vis[i][x] = 1;
							gmw_draw_block(G2048_CGI, i, x, g2048[i][x], bdi);
							score += g2048[i][x];
							num++;
						}
						else if (x != j - 1) {
							gmw_move_block(G2048_CGI, i, j, g2048[i][j], 0, bdi, direction, j - 1 - x);
							g2048[i][x + 1] = g2048[i][j], g2048[i][j] = 0;
							vis[i][x + 1] = vis[i][j], vis[i][j] = 0;
							num++;
						}
					}
				}
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����������Ŀǰ�������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int find_max(const CONSOLE_GRAPHICS_INFO* G2048_CGI,int g2048[max_row][max_col])
{
	int max = 0;
	for (int i = 0; i < G2048_CGI->row_num; i++) {
		for (int j = 0; j < G2048_CGI->col_num; j++) {
			if (g2048[i][j] > max)
				max = g2048[i][j];
		}
	}
	return max;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж�������Ϸ�Ƿ����
  ���������
  �� �� ֵ��
  ˵    ��������ֵ��Ϊ 0 ��ʾ��Ϸ����
***************************************************************************/
int finish(int row, int col, int g2048[max_row][max_col])
{
	int xx[4] = { 1,-1,0,0 };
	int yy[4] = { 0,0,1,-1 };
	int num = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (g2048[i][j] != 0)
				num++;
			int newi, newj;
			for (int k = 0; k < 4; k++) {
				newi = i + xx[k];
				newj = j + yy[k];
				if (g2048[i][j] != 0 && g2048[i][j] == g2048[newi][newj])
					return 0;
			}
		}
	}
	if (num == row * col)
		return num;
	else
		return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����仯������ɫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void fill(const CONSOLE_GRAPHICS_INFO* G2048_CGI, int row, int col, int g2048[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi)
{
	srand((unsigned int)(time(NULL)));
	int x, y;
	while (1) {
		x = rand() % row, y = rand() % col;
		if (g2048[x][y] == 0) {
			g2048[x][y] = 2;
			gmw_draw_block(G2048_CGI, x, y, g2048[x][y], bdi);
			break;
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���Ϸ��������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void print_end_of_game(const CONSOLE_GRAPHICS_INFO* G2048_CGI,int result)
{
	/* ������Ϸ�������λ�� */
	int start_x, start_y;
	if (G2048_CGI->col_num % 2 == 1)
		start_x = G2048_CGI->start_x + 2 * G2048_CGI->draw_frame_with_row_no + 2 + G2048_CGI->col_num / 2 * G2048_CGI->CFI.bwidth;
	else
		start_x = G2048_CGI->start_x + 2 * G2048_CGI->draw_frame_with_row_no + 2 + (G2048_CGI->col_num - 1) / 2 * G2048_CGI->CFI.bwidth + G2048_CGI->CFI.block_width / 2;
	if (G2048_CGI->row_num % 2 == 1)
		start_y = G2048_CGI->start_y + G2048_CGI->draw_frame_with_col_no + G2048_CGI->top_status_line + 1 + G2048_CGI->row_num / 2 * G2048_CGI->CFI.bhigh;
	else
		start_y = G2048_CGI->start_y + G2048_CGI->draw_frame_with_col_no + G2048_CGI->top_status_line + 1 + (G2048_CGI->row_num - 1) / 2 * G2048_CGI->CFI.bhigh + G2048_CGI->CFI.block_high / 2 + 1;

	char temp[256];
	sprintf(temp, "%s%s%s%s%s%s", G2048_CGI->CBI.top_left, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.top_right);
	cct_showstr(start_x, start_y, temp, COLOR_HRED, COLOR_HWHITE);
	if (result == 0)
		sprintf(temp, "%s��Ϸʧ��%s", G2048_CGI->CBI.v_normal, G2048_CGI->CBI.v_normal);
	else
		sprintf(temp, "%s��Ϸʤ��%s", G2048_CGI->CBI.v_normal, G2048_CGI->CBI.v_normal);
	cct_showstr(start_x, start_y + 1, temp, COLOR_HRED, COLOR_HWHITE);
	sprintf(temp, "%sR������ %s", G2048_CGI->CBI.v_normal, G2048_CGI->CBI.v_normal);
	cct_showstr(start_x, start_y + 2, temp, COLOR_HRED, COLOR_HWHITE);
	sprintf(temp, "%sQ���˳� %s", G2048_CGI->CBI.v_normal, G2048_CGI->CBI.v_normal);
	cct_showstr(start_x, start_y + 3, temp, COLOR_HRED, COLOR_HWHITE);
	sprintf(temp, "%s%s%s%s%s%s", G2048_CGI->CBI.lower_left, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.h_normal, G2048_CGI->CBI.lower_right);
	cct_showstr(start_x, start_y + 4, temp, COLOR_HRED, COLOR_HWHITE);

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�2048�ľ���ʵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void step_of_2048(CONSOLE_GRAPHICS_INFO* pG2048_CGI,int row,int col,int target,int g2048[max_row][max_col],bool& remake,int &maxscore)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_HBLACK, COLOR_BLACK, NULL},
		{32, COLOR_HGREEN, COLOR_BLACK, NULL},
		{64, COLOR_YELLOW, COLOR_BLACK, NULL},
		{128, COLOR_RED, COLOR_BLACK, NULL},
		{256, COLOR_HWHITE, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_PINK, COLOR_BLACK, NULL},
		{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //������ģ������Լ�����
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};

	char temp[256];
	int i, j;

	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(pG2048_CGI, row, col);

	/* ��ʾ��� */
	gmw_draw_frame(pG2048_CGI);

	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(pG2048_CGI, i, j, g2048[i][j], bdi_normal);
		}

	if (pG2048_CGI->inited == CGI_INITED) {

		int score = 0;

		while (1) {
			int maction, mrow = 0, mcol = 0;
			int keycode1, keycode2;
			int ret;
			int direction = -1;

			if (finish(row, col, g2048) || find_max(pG2048_CGI, g2048) == target) {
				if (finish(row, col, g2048))
					print_end_of_game(pG2048_CGI, 0);
				else
					print_end_of_game(pG2048_CGI, 1);
				int X, Y;
				while (1) {
					ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
					if (maction == CCT_KEYBOARD_EVENT) {
						if (keycode1 == 'r' || keycode1 == 'R') {
							remake = 1;
							break;
						}
						else if (keycode1 == 'q' || keycode1 == 'Q')
							break;
					}
				}
				break;
			}

			if (score > maxscore)
				maxscore = score;
			cct_setcolor();
			cct_gotoxy(0, 0);//����赽ָ��λ��
			cout << setw(pG2048_CGI->cols - 1ll) << ' '; //���ÿո��������
			sprintf(temp, "Ŀ��: %d ����: %d ���: %d <R:���� Q:�˳�>", target, score, maxscore);
			int x = (pG2048_CGI->SLI.width - strlen(temp)) / 2;
			cct_showstr(x, 0, temp, COLOR_WHITE, COLOR_BLACK);

			ret = gmw_read_keyboard_and_mouse(pG2048_CGI, maction, mrow, mcol, keycode1, keycode2);
			switch (keycode1) {
			case 'q':
			case 'Q':
				gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, "��Ϸ����", NULL);
				return;
			case 'r':
			case 'R':
				remake = 1;
				return;
			case 0xE0: //224
				switch (keycode2) {
				case KB_ARROW_UP:
					sprintf(temp, "[�����Ϸ����]");
					gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, temp);
					direction = DOWN_TO_UP;
					break;
				case KB_ARROW_DOWN:
					sprintf(temp, "[�����·����]");
					gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, temp);
					direction = UP_TO_DOWN;
					break;
				case KB_ARROW_LEFT:
					sprintf(temp, "[���������]");
					gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, temp);
					direction = RIGHT_TO_LEFT;
					break;
				case KB_ARROW_RIGHT:
					sprintf(temp, "[�����ҷ����]");
					gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, temp);
					direction = LEFT_TO_RIGHT;
					break;
				}
				break;
			default:
				sprintf(temp, "[��������] %d/%d", keycode1, keycode2);
				gmw_status_line(pG2048_CGI, LOWER_STATUS_LINE, temp);
				break;
			}
			int num = 0;
			array_move(pG2048_CGI, row, col, g2048, bdi_normal, direction, score, num);
			if (num != 0)
				fill(pG2048_CGI, row, col, g2048, bdi_normal);
		}

	}
	return;
}