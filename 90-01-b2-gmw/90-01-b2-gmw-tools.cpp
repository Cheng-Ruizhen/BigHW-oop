/* 2152046 程瑞真 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

/***************************************************************************
  函数名称：
  功    能：输入相应的值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void input(int& row, int& col, int& target)
{
	while (1) {
		cout << "请输入行数(5-8)：" << endl;
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
		cout << "请输入列数(5-10)：" << endl;
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
		cout << "请输入合成目标(5-20)：" << endl;
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
  函数名称：
  功    能：非递归方式找相邻位置相同数
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：判断整个游戏是否结束
  输入参数：
  返 回 值：
  说    明：返回值为0，表示仍有可消除项；否则返回还有几个为未消除项
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
  函数名称：
  功    能：计算分数
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：选中色块对应数组的变化
  输入参数：
  返 回 值：
  说    明：返回变化后的数组中的最大值
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
  函数名称：
  功    能：数组变化后，补满空色块
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：
  功    能：内部数组的移动
  输入参数：
  返 回 值：
  说    明：合成十中数组全为向下移动
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
  函数名称：
  功    能：合成十的具体实现
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void step_of_merge_to_10(CONSOLE_GRAPHICS_INFO* pMto10_CGI, int row, int col, int map[max_row][max_col], int v[max_row][max_col], int target)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//数字1的“正常”状态
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{6,  COLOR_HBLUE,  COLOR_BLACK,  NULL},
		{7,  COLOR_HGREEN, COLOR_BLACK,  NULL},
		{8,  COLOR_HCYAN,  COLOR_BLACK,  NULL},
		{9,  COLOR_HRED,   COLOR_BLACK,  NULL},
		{10, COLOR_HPINK,  COLOR_BLACK,  NULL},	//数字10的“正常”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//数字1的“可选择+关联”状态
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{6,  COLOR_HBLUE,  COLOR_WHITE,  NULL},
		{7,  COLOR_HGREEN, COLOR_WHITE,  NULL},
		{8,  COLOR_HCYAN,  COLOR_WHITE,  NULL},
		{9,  COLOR_HRED,   COLOR_WHITE,  NULL},
		{10, COLOR_HPINK,  COLOR_WHITE,  NULL},	//数字10的“可选择+关联”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//数字1的“选中”状态
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{6, COLOR_HBLUE,  COLOR_HWHITE, NULL},
		{7, COLOR_HGREEN, COLOR_HWHITE, NULL},
		{8, COLOR_HCYAN,  COLOR_HWHITE, NULL},
		{9, COLOR_HRED,   COLOR_HWHITE, NULL},
		{10,COLOR_HPINK,  COLOR_HWHITE, NULL},	//数字10的“选中”状态，如果需要，还可以继续增加11及以后的数字
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	char temp[256];
	int i, j;

	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(pMto10_CGI, row, col);

	/* 显示框架 */
	gmw_draw_frame(pMto10_CGI);

	/* 上状态栏显示内容 */
	sprintf(temp, "窗口大小：%d行 %d列", pMto10_CGI->lines, pMto10_CGI->cols);
	gmw_status_line(pMto10_CGI, TOP_STATUS_LINE, temp);

	/* 将内部数组展现到屏幕上 */
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(pMto10_CGI, i, j, map[i][j], bdi_normal);
		}

	/* 读鼠标/键盘并根据返回值进行相应的动作
	   注：1、本处约定鼠标右键退出，左键显示<行，列>，实际游戏中，要根据返回的行列进行某种动作（找出相同项、设置选中项、消除等）
		   2、本处约定按回车退出，其它显示按键信息，实际游戏中，要根据返回的行列进行某种动作（找出相同项、设置选中项、消除等）
		   3、实际游戏中应写为函数 */
	if (pMto10_CGI->inited == CGI_INITED) {
		int once_score, sum_score = 0;
		int mrow = 0, mcol = 0;

		while (1) {
			if (finish(row, col, map)) {
				gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "无可消除项，游戏结束");
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
						/* 这时，mrow、mcol肯定跟刚才不同 */

						/* 原色块正常显示 */
						if (old_mrow >= 0 && old_mcol >= 0)
							gmw_draw_block(pMto10_CGI, old_mrow, old_mcol, map[old_mrow][old_mcol], bdi_normal);

						/* 新色块亮显 */
						gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
					}
					else if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
						/* 下状态栏显示内容 */
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "游戏结束", NULL);
						return;
					}
					else {
						sprintf(temp, "[读到左键] %c行%d列", char('A' + mrow), mcol); //未考虑mrow超过26，mcol超过99的情况，如有需要，请自行处理
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						loop = 0;
					}
				}
				else { //CCT_KEYBOARD_EVENT - 具体键码含义去读 cmd_console_tools.cpp
					switch (keycode1) {
					case 0x0D:
						/* 下状态栏显示内容 */
						sprintf(temp, "[读到回车键] %c行%d列", char('A' + mrow), mcol);
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						loop = 0;
						break;
					case 'q':
					case 'Q':
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "游戏结束", NULL);
						return;
					case 0xE0: //224
						sprintf(temp, "[读到方向键]");
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
						/* 原色块正常显示 */
						if (old_mrow >= 0 && old_mcol >= 0)
							gmw_draw_block(pMto10_CGI, old_mrow, old_mcol, map[old_mrow][old_mcol], bdi_normal);

						/* 新色块亮显 */
						gmw_draw_block(pMto10_CGI, mrow, mcol, map[mrow][mcol], bdi_selected);
						break;
					default:
						sprintf(temp, "[读到键码] %d/%d", keycode1, keycode2);
						gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, temp);
						break;
					}//end of switch
				}//end of else
			}//end of while

			/* 退出循环,说明已经选好了坐标 */
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
				gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "选择的矩阵坐标位置处无连续相同值，请重新选择");
				v[mrow][mcol] = 0;
				continue;
			}
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (v[i][j] == 1 && map[i][j])
						gmw_draw_block(pMto10_CGI, i, j, map[i][j], bdi_related);
				}
			}
			cct_showstr(pMto10_CGI->SLI.lower_start_x, pMto10_CGI->SLI.lower_start_y + 1, "箭头键/鼠标移动取消当前选择，回车键/单击左键合成");
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
			if (!(maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || char(keycode1) == '\r')) { //取消了选择
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
					/* 新色块亮显 */
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
			else //确定选择
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
				sprintf(temp, "本次得分: %d 总分: %d 合成目标: %d", once_score, sum_score, target);
				gmw_status_line(pMto10_CGI, TOP_STATUS_LINE, temp);

				/* 求出选择的那个色块移动后的位置 */
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
					sprintf(temp, "已经合成到%d ", target);
					gmw_status_line(pMto10_CGI, LOWER_STATUS_LINE, "按回车键或单击左键继续向更高目标进发...", temp);
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