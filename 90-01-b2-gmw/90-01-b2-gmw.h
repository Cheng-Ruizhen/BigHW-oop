/* 2152046 程瑞真 大数据 */
#pragma once

#define max_row 20
#define max_col 20

void input(int& row, int& col, int& target);
void no_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);
int finish(int row, int col, int map[max_row][max_col]);
int score(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);
int change(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);
void fill(const CONSOLE_GRAPHICS_INFO* Mto10_CGI, int row, int col, int max, int map[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi);
void step_of_merge_to_10(CONSOLE_GRAPHICS_INFO* pMto10_CGI, int row, int col, int map[max_row][max_col], int v[max_row][max_col], int target);