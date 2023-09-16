/* 2152046 大数据 程瑞真 */
#pragma once
/* 设置数组的最大行列 */
#define max_row  40
#define max_col  40

/* 上下左右移动 */
#define up       1
#define down     2
#define left     3
#define right    4

/* 整行整列移动或非零移动 */
#define together 1
#define single   2

void no_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);
void print(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int append);
int finish(int row, int col, int map[max_row][max_col]);
int tj_strcasecmp(const char s1[], const char s2[]);
void background(int row, int col, int wid, int heig, int append, int form);
void diamond(int wid, int heig, int bg_color, int fg_color, char* ch, int form);
void single_move(int up_or_down_or_left_or_right, int num, int wid, int heig, int bg_color, int fg_color, int form, int append, char* ch);
void array_move(int up_or_down_or_left_or_right, int tog_or_sin, int wid, int heig, int row, int col, int map[max_row][max_col], int v[max_row][max_col], int form, bool need_console, int is_same, char* ch);
void print_blank(int row, int col, int map[max_row][max_col]);
int read_keybord_and_mouse(int row, int col, int wid, int heig, int& rrow, int& ccol, int& old_X, int& old_Y, int map[max_row][max_col], int append, int form, int is_same, char* ch);