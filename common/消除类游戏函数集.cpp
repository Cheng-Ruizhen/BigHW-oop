 /* 2152046 ������ ������ */
#include<iostream>
#include<iomanip>
#include<Windows.h>
#include"../include/cmd_console_tools.h"
#include"../include/��������Ϸ������.h"

using namespace std;

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
			if (newx > 0 && newx <= row && newy > 0 && newy <= col) {
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
  ��    �ܣ���ӡ����
  ���������
  �� �� ֵ��
  ˵    ����append:1-������ӡ 2-��ӡ�������� 3-��ӡ��ɫ��ʶ������
***************************************************************************/
void print(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int append)
{
	cout << "  |";
	for (int i = 0; i < col; i++) {
		cout << setw(3) << i;
	}
	cout << endl;
	cout << "--+" << setw((long long)3 * col) << setfill('-') << "" << setfill(' ') << endl;
	for (int i = 1; i <= row; i++) {
		cout << char('A' + i - 1) << " |";
		for (int j = 1; j <= col; j++) {
			if (append == 1) {
				cout << setw(3) << map[i][j];
			}
			else if (append == 2) {
				if (v[i][j] == 1) {
					cout << setw(3) << '*';
				}
				else {
					cout << setw(3) << v[i][j];
				}
			}
			else if (append == 3) {
				if (v[i][j] == 1) {
					cout << "  ";
					int a, b;
					cct_getxy(a, b);
					cct_showch(a, b, char('0' + map[i][j]), COLOR_HYELLOW, COLOR_BLACK);
					cct_setcolor();
				}
				else {
					cout << "  " << map[i][j];
				}
			}
		}
		cout << endl;
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
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
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
  ��    �ܣ��ȶ������ַ��������ڱ�֤�ض�ָ�������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int tj_strcasecmp(const char s1[], const char s2[])
{
	int a = strlen(s1) > strlen(s2) ? strlen(s1) : strlen(s2), i;
	for (i = 0; i < a; i++) {
		int m = int(s1[i]), n = int(s2[i]);
		if (m > 64 && m < 91)
			m = m + 32;
		if (n > 64 && n < 91)
			n = n + 32;
		if (m - n != 0)
			return m - n;
	}
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�αͼ�ν��汳����Ĵ�ӡ
  ���������int row,col ����������������
            int wid,heig��ɫ��Ŀ���
			int append  ��ֵΪ1����ʾ�޷ָ��ߣ�ֵΪ2����ʾ�зָ���
			int form    ��ֵΪ1����ʾʵ���ߣ�ֵΪ2����ʾ������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void background(int row, int col, int wid, int heig, int append, int form)
{
	char solid[][3] = { "��","��","��","��","��","��","��","��","��","��","��" };
	char hollow[][3] = { "�X","�T","�[","�^","�a","�U","�j","�m","�p","�d","�g" };
	char(*p)[3];
	if (form == 1)
		p = solid;
	else
		p = hollow;
	int x, y;

	/* �޷ָ��߻��� */
	if (append == 1) {
		x = heig * row + 8;
		y = wid * col + 7;
		cct_setconsoleborder(y, x, y, x);
		cout << "��Ļ��ǰ����Ϊ��" << x << "��" << y << "��" << endl;
		cout << setw(7) << 0;
		for (int i = 2; i <= col; i++) {
			cout << setw(wid) << i - 1;
		}
		cout << endl;
		for (int i = 1; i <= heig * row + 2; i++) {
			if (i % heig == 0)
				cout << (char)('A' + i / heig - 1) << " ";
			else
				cout << "  ";
			for (int j = 1; j <= wid / 2 * col + 2; j++) {
				int a, b;
				cct_getxy(a, b);
				if (i == 1) {
					if (j == 1)
						cct_showstr(a, b, p[0], COLOR_WHITE, COLOR_BLACK);
					else if (j == wid / 2 * col + 2)
						cct_showstr(a, b, p[2], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, p[1], COLOR_WHITE, COLOR_BLACK);
				}
				else if (i == heig * row + 2) {
					if (j == 1)
						cct_showstr(a, b, p[3], COLOR_WHITE, COLOR_BLACK);
					else if (j == wid / 2 * col + 2)
						cct_showstr(a, b, p[4], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, p[1], COLOR_WHITE, COLOR_BLACK);
				}
				else {
					if (j == 1 || j == wid / 2 * col + 2)
						cct_showstr(a, b, p[5], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, "  ", COLOR_WHITE, COLOR_WHITE);
				}
				Sleep(1);
			}
			cct_setcolor();
			cout << endl;
		}
	}

	/* �зָ��߻��� */
	else {
		x = (heig + 1) * row + 11;
		y = (wid + 2) * col + 5;
		cct_setconsoleborder(y, x, y, x);
		cout << "��Ļ��ǰ����Ϊ��" << x << "��" << y << "��" << endl;
		cout << setw(7) << 0;
		for (int i = 2; i <= col; i++) {
			cout << setw(wid + 2) << i - 1;
		}
		cout << endl;
		for (int i = 1; i <= (heig + 1) * row + 1; i++) {
			if (i % (heig + 1) == 3)
				cout << (char)('A' + i / 4) << " ";
			else
				cout << "  ";
			for (int j = 1; j <= (wid + 2) / 2 * col + 1; j++) {
				int a, b;
				cct_getxy(a, b);
				if (i == 1) {
					if (j == 1)
						cct_showstr(a, b, p[0], COLOR_WHITE, COLOR_BLACK);
					else if (j == (wid + 2) / 2 * col + 1)
						cct_showstr(a, b, p[2], COLOR_WHITE, COLOR_BLACK);
					else if (j % ((wid + 2) / 2) == 1)
						cct_showstr(a, b, p[6], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, p[1], COLOR_WHITE, COLOR_BLACK);
				}
				else if (i == (heig + 1) * row + 1) {
					if (j == 1)
						cct_showstr(a, b, p[3], COLOR_WHITE, COLOR_BLACK);
					else if (j == (wid + 2) / 2 * col + 1)
						cct_showstr(a, b, p[4], COLOR_WHITE, COLOR_BLACK);
					else if (j % ((wid + 2) / 2) == 1)
						cct_showstr(a, b, p[7], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, p[1], COLOR_WHITE, COLOR_BLACK);
				}
				else if (i % (heig + 1) == 1) {
					if (j == 1)
						cct_showstr(a, b, p[9], COLOR_WHITE, COLOR_BLACK);
					else if (j == (wid + 2) / 2 * col + 1)
						cct_showstr(a, b, p[10], COLOR_WHITE, COLOR_BLACK);
					else if (j % ((wid + 2) / 2) == 1)
						cct_showstr(a, b, p[8], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, p[1], COLOR_WHITE, COLOR_BLACK);
				}
				else {
					if (j % ((wid + 2) / 2) == 1)
						cct_showstr(a, b, p[5], COLOR_WHITE, COLOR_BLACK);
					else
						cct_showstr(a, b, "  ", COLOR_WHITE, COLOR_WHITE);
				}
				Sleep(1);
			}
			cct_setcolor();
			cout << endl;
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡαͼ���е�С����
  ���������int wid,heig��ɫ��Ŀ�͸�
            int bg_color������ɫ
			int fg_color��ǰ��ɫ
			char* ch    ����Ҫ��ӡ���ַ�
			int form    ��ֵΪ1����ʾʵ���ߣ�ֵΪ2����ʾ������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void diamond(int wid, int heig, int bg_color, int fg_color, char* ch, int form)
{
	char solid[][3] = { "��","��","��","��","��","��" };
	char hollow[][3] = { "�X","�T","�[","�^","�a","�U" };
	char(*p)[3];
	if (form == 1)
		p = solid;
	else
		p = hollow;

	int a, b;
	cct_getxy(a, b);
	cct_showstr(a, b, p[0], bg_color, fg_color);
	for (int i = 2; i <= wid - 4; i += 2)
		cct_showstr(a + i, b, p[1], bg_color, fg_color);
	cct_showstr(a + wid - 2, b, p[2], bg_color, fg_color);
	for (int i = 1; i <= heig - 2; i++) {
		cct_showstr(a, b + i, p[5], bg_color, fg_color);
		cct_showstr(a + wid - 2, b + i, p[5], bg_color, fg_color);
		if (i == (heig - 1) / 2)
			cct_showstr(a + 2, b + i, ch, bg_color, fg_color);
		else {
			for (int j = 2; j <= wid - 4; j += 2)
				cct_showstr(a + j, b + i, "  ", bg_color, fg_color);
		}
	}
	cct_showstr(a, b + heig - 1, p[3], bg_color, fg_color);
	for (int i = 2; i <= wid - 4; i += 2)
		cct_showstr(a + i, b + heig - 1, p[1], bg_color, fg_color);
	cct_showstr(a + wid - 2, b + heig - 1, p[4], bg_color, fg_color);
	cct_setcolor();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ɫ���������ҵ��ƶ�
  ���������int up_or_down_or_left_or_right�������ƶ�����
            int num                        ��ɫ���ƶ��ĸ���
            int wid,heig                   ��ɫ��Ŀ�Ⱥ͸߶�
			int bg_color,fg_color          ��ɫ��ı���ɫ��ǰ��ɫ
			int form                       ���߿����ʵ(1)/��(2)����
			int append                     �������Ƿ��зָ���
			char *ch                       ��ɫ���е��ַ���
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void single_move(int up_or_down_or_left_or_right, int num, int wid, int heig, int bg_color, int fg_color, int form, int append, char* ch)
{
	char solid[][3] = { "��","��","��","��","��","��" };
	char hollow[][3] = { "�X","�T","�[","�^","�a","�U" };
	char(*p)[3];
	if (form == 1)
		p = solid;
	else
		p = hollow;

	int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
	int a, b;
	cct_getxy(a, b);
	for (int i = 1; i <= num; i++) {
		cct_gotoxy(a, b);
		if (dx[up_or_down_or_left_or_right - 1] != 0) {
			int y = b;
			for (int i = 1; i <= heig + append - 1; i++) {
				cct_gotoxy(a, y);
				char in[20] = { '\0' };
				memset(in, ' ', wid - 4);
				diamond(wid, heig, COLOR_WHITE, COLOR_WHITE, in, form);
				y += dx[up_or_down_or_left_or_right - 1];
				cct_gotoxy(a, y);
				diamond(wid, heig, bg_color, fg_color, ch, form);
				if (append == 2 && i % (heig + 1) == 0) {
					for (int j = 0; j <= wid - 2; j += 2) {
						if (dx[up_or_down_or_left_or_right - 1] > 0)
							cct_showstr(a + j, y - 1, p[1], COLOR_WHITE, COLOR_BLACK);
						else
							cct_showstr(a + j, y + heig, p[1], COLOR_WHITE, COLOR_BLACK);
					}
				}
				Sleep(10);
			}
			b += (heig + append - 1) * dx[up_or_down_or_left_or_right - 1];
		}
		else if (dy[up_or_down_or_left_or_right - 1] != 0) {
			int x = a;
			for (int i = 1; i <= wid + (append - 1) * 2; i++) {
				cct_gotoxy(x, b);
				char in[20] = { '\0' };
				memset(in, ' ', wid - 4);
				diamond(wid, heig, COLOR_WHITE, COLOR_WHITE, in, form);
				x += dy[up_or_down_or_left_or_right - 1];
				cct_gotoxy(x, b);
				diamond(wid, heig, bg_color, fg_color, ch, form);
				if (append == 2 && i % (wid + 2) == 0) {
					for (int j = 0; j <= heig - 1; j++) {
						if (dy[up_or_down_or_left_or_right - 1] > 0)
							cct_showstr(x - 2, b + j, p[5], COLOR_WHITE, COLOR_BLACK);
						else
							cct_showstr(x + wid, b + j, p[5], COLOR_WHITE, COLOR_BLACK);
					}
				}
				Sleep(10);
			}
			a += (wid + (append - 1) * 2) * dy[up_or_down_or_left_or_right - 1];
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ڲ�������ƶ�
  ���������int up_or_down_or_left_or_right�������ƶ�����
            int tog_or_sin                 �������ǵ������ƶ������������ƶ�
			bool need_console              ���Ƿ���Ҫ��ӡɫ��
			int is_same                    ������ӡ���ַ�����ͬ���Ż�������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void array_move(int up_or_down_or_left_or_right, int tog_or_sin, int wid, int heig, int row, int col, int map[max_row][max_col],int v[max_row][max_col], int form, bool need_console,int is_same,char* ch)
{
	int sum1[max_row] = {0}, sum2[max_col] = {0};
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++)
			sum1[i] += map[i][j];
	}
	for (int j = 1; j <= col; j++) {
		for (int i = 1; i <= row; i++)
			sum2[j] += map[i][j];
	}

	int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
	if (dx[up_or_down_or_left_or_right - 1] != 0) {
		if (tog_or_sin == together) {
			for (int i = dx[up_or_down_or_left_or_right - 1] > 0 ? row - 1 : 2; i >= 1 && i <= row; i -= dx[up_or_down_or_left_or_right - 1]) {
				if (sum1[i] != 0) {
					int j;
					for (j = i + dx[up_or_down_or_left_or_right - 1]; j >= 0 && j <= row + 1; j++) {
						if (j == (row + 1) || j == 0 || sum1[j] != 0)
							break;
					}
					if (i != (j - dx[up_or_down_or_left_or_right - 1])) {
						sum1[j - dx[up_or_down_or_left_or_right - 1]] = sum1[i], sum1[i] = 0;
						for (int x = 1; x <= col; x++) {
							if (need_console && map[i][x] != 0) {
								cct_gotoxy((wid + 2) * (x - 1) + 4, (heig + 1) * (i - 1) + 3);
								if (!is_same) {
									char ch[10] = { '\0' };
									sprintf_s(ch, "%d", map[i][j]);
									int len = strlen(ch);
									ch[len++] = ' ', ch[len] = '\0';
									single_move(up_or_down_or_left_or_right, abs(j - dx[up_or_down_or_left_or_right - 1] - i), wid, heig, map[i][x] % 6 + 1, COLOR_BLACK, form, 2, ch);
								}
								else
									single_move(up_or_down_or_left_or_right, abs(j - dx[up_or_down_or_left_or_right - 1] - i), wid, heig, map[i][x] % 6 + 1, COLOR_BLACK, form, 2, ch);
							}
							map[j - dx[up_or_down_or_left_or_right - 1]][x] = map[i][x], map[i][x] = 0;
							v[j - dx[up_or_down_or_left_or_right - 1]][x] = 0, v[i][x] = 1;
						}
					}
				}
			}
		}
		else if(tog_or_sin == single) {
			for (int j = 1; j <= col; j++) {
				for (int i = dx[up_or_down_or_left_or_right - 1] > 0 ? row - 1 : 2; i >= 1 && i <= row; i -= dx[up_or_down_or_left_or_right - 1]) {
					if (map[i][j] != 0) {
						int x;
						for (x = i + dx[up_or_down_or_left_or_right - 1]; x >= 0 && x <= row + 1; x += dx[up_or_down_or_left_or_right - 1]) {
							if (x == 0 || x == (row + 1) || map[x][j] != 0)
								break;
						}
						if (i != (x - dx[up_or_down_or_left_or_right - 1])) {
							if (need_console) {
								cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
								if (!is_same) {
									char ch[10] = { '\0' };
									sprintf_s(ch, "%d", map[i][j]);
									int len = strlen(ch);
									ch[len++] = ' ', ch[len] = '\0';
									single_move(up_or_down_or_left_or_right, abs(x - dx[up_or_down_or_left_or_right - 1] - i), wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
								}
								else 
									single_move(up_or_down_or_left_or_right, abs(x - dx[up_or_down_or_left_or_right - 1] - i), wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
							}
							map[x - dx[up_or_down_or_left_or_right - 1]][j] = map[i][j], map[i][j] = 0;
							v[x - dx[up_or_down_or_left_or_right - 1]][j] = 0, v[i][j] = 1;
						}
					}
				}
			}
		}
	}
	if (dy[up_or_down_or_left_or_right - 1] != 0) {
		if (tog_or_sin == together) {
			for (int j = dy[up_or_down_or_left_or_right - 1] > 0 ? col - 1 : 2; j >= 1 && j <= col; j -= dy[up_or_down_or_left_or_right - 1]) {
				if (sum2[j] != 0) {
					int i;
					for (i = j + dy[up_or_down_or_left_or_right - 1]; i >= 0 && i <= (col + 1); i += dy[up_or_down_or_left_or_right - 1]) {
						if (i == 0 || i == (col + 1) || sum2[i] != 0)
							break;
					}
					if (j != (i - dy[up_or_down_or_left_or_right - 1])) {
						sum2[i - dy[up_or_down_or_left_or_right - 1]] = sum2[j], sum2[j] = 0;
						for (int x = 1; x <= row; x++) {
							if (need_console && map[x][j] != 0) {
								cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (x - 1) + 3);
								if (!is_same) {
									char ch[10] = { '\0' };
									sprintf_s(ch, "%d", map[i][j]);
									int len = strlen(ch);
									ch[len++] = ' ', ch[len] = '\0';
									single_move(up_or_down_or_left_or_right, abs(i - dy[up_or_down_or_left_or_right - 1] - j), wid, heig, map[x][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
								}
								else
									single_move(up_or_down_or_left_or_right, abs(i - dy[up_or_down_or_left_or_right - 1] - j), wid, heig, map[x][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
							}
							map[x][i - dy[up_or_down_or_left_or_right - 1]] = map[x][j], map[x][j] = 0;
							v[x][i - dy[up_or_down_or_left_or_right - 1]] = 0, v[x][j] = 1;
						}
					}
				}
			}
		}
		else if (tog_or_sin == single) {
			for (int i = 1; i <= row; i++) {
				for (int j = dy[up_or_down_or_left_or_right - 1] > 0 ? col - 1 : 2; j >= 1 && j <= col; j -= dy[up_or_down_or_left_or_right - 1]) {
					if (map[i][j] != 0) {
						int x;
						for (x = j + dy[up_or_down_or_left_or_right - 1]; x >= 0 && x < +col + 1; x += dy[up_or_down_or_left_or_right - 1]) {
							if (x == 0 || x == col + 1 || map[i][x] != 0)
								break;
						}
						if (j != x - dy[up_or_down_or_left_or_right - 1]) {
							if (need_console) {
								cct_gotoxy((wid + 2) * (j - 1) + 4, (heig + 1) * (i - 1) + 3);
								if (!is_same) {
									char ch[10] = { '\0' };
									sprintf_s(ch, "%d", map[i][j]);
									int len = strlen(ch);
									ch[len++] = ' ', ch[len] = '\0';
									single_move(up_or_down_or_left_or_right, abs(x - dy[up_or_down_or_left_or_right - 1] - j), wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
								}
								else
									single_move(up_or_down_or_left_or_right, abs(x - dy[up_or_down_or_left_or_right - 1] - j), wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, form, 2, ch);
							}
							map[i][x - dy[up_or_down_or_left_or_right - 1]] = map[i][j], map[i][j] = 0;
							v[i][x - dy[up_or_down_or_left_or_right - 1]] = 0, v[i][j] = 1;
						}
					}
				}
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ѡ���ɫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void print_blank(int row, int col, int map[max_row][max_col])
{
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (map[i][j] == 0) {
				cct_showstr(8 * j - 4, 4 * i - 1, "      ", COLOR_WHITE, COLOR_WHITE);
				cct_showstr(8 * j - 4, 4 * i, "      ", COLOR_WHITE, COLOR_WHITE);
				cct_showstr(8 * j - 4, 4 * i + 1, "      ", COLOR_WHITE, COLOR_WHITE);
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ȡ���ͼ�����ѡ���λ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int read_keybord_and_mouse(int row, int col, int wid, int heig, int& rrow, int& ccol,int &old_X,int &old_Y, int map[max_row][max_col], int append, int form, int is_same, char* ch)
{
	int X = 4, Y = 3;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;

	while (loop) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		int s = 0;
		if (ret == CCT_MOUSE_EVENT) {
			if (maction == MOUSE_ONLY_MOVED)
				old_X = X, old_Y = Y;
			cct_gotoxy(0, (heig + append - 1) * row + 4);
			rrow = (Y + append - 1) / (heig + append - 1), ccol = (X + wid + 2 * append - 6) / (wid + 2 * (append - 1));
			cct_setcolor();
			cout << "[���λ��] :";
			if (X == 0 || X == 1 || X == 2 || X == 3 || Y == 0 || Y == 1 || Y == 2 || Y > (heig + append - 1) * row + 1 || X > (wid + 2 * (append - 1)) * col + 1 || map[rrow][ccol] == 0) {
				cout << "λ�÷Ƿ�                                  ";
				s = 1;
			}
			else if (append == 2 && (X % (wid + 2) == 2 || X % (wid + 2) == 3 || Y % (heig + 1) == 2)) {
				cout << "λ�÷Ƿ�                                  ";
				s = 1;
			}
			else {
				if (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_RIGHT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK)
					return maction;
				cout << char(rrow + 'A' - 1) << "��" << ccol - 1 << "��                                                ";
				cct_gotoxy((wid + 2 * (append - 1)) * (ccol - 1) + 4, (heig + append - 1) * (rrow - 1) + 3);
				if (!is_same) {
					char ch[10] = { '\0' };
					sprintf_s(ch, "%d", map[rrow][ccol]);
					int len = strlen(ch);
					while (len < wid - 4) {
						ch[len] = ' ';
						len++;
					}
					ch[len] = '\0';
					diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
				}
				else
					diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
			}
		}
		else if (ret == CCT_KEYBOARD_EVENT) {
			rrow = (old_Y + append - 1) / (heig + append - 1), ccol = (old_X + wid + 2 * append - 6) / (wid + 2 * (append - 1));
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
				if (!is_same) {
					char ch[10] = { '\0' };
					sprintf_s(ch, "%d", map[rrow][ccol]);
					int len = strlen(ch);
					while (len < wid - 4) {
						ch[len] = ' ';
						len++;
					}
					ch[len] = '\0';
					diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
				}
				else
					diamond(wid, heig, map[rrow][ccol] % 6 + 1, COLOR_WHITE, ch, form);
				old_X = (wid + 2 * (append - 1)) * (ccol - 1) + 4, old_Y = (heig + append - 1) * (rrow - 1) + 3;
			}
			else
				return keycode1;
		}
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= col; j++) {
				if ((s == 1 || i != rrow || j != ccol) && map[i][j]) {
					cct_gotoxy((wid + 2 * (append - 1)) * (j - 1) + 4, (heig + append - 1) * (i - 1) + 3);
					if (!is_same) {
						char ch[10] = { '\0' };
						sprintf_s(ch, "%d", map[i][j]);
						int len = strlen(ch);
						while (len < wid - 4) {
							ch[len] = ' ';
							len++;
						}
						ch[len] = '\0';
						diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
					}
					else
						diamond(wid, heig, map[i][j] % 6 + 1, COLOR_BLACK, ch, form);
				}
			}
		}
	}
	return 0;
}