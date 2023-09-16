/* 2152046 大数据 程瑞真 */
#pragma once
#define wid  6
#define heig 3
#define form 2

void input(int& row, int& col, char choice);
void main_base(int row, int col, int map[max_row][max_col], int v[max_row][max_col], char choice);
int score(int row, int col, int v[max_row][max_col]);
void change(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);
void main_console(int row, int col, int map[max_row][max_col], int v[max_row][max_col], char choice);