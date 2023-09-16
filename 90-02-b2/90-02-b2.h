/* 2152046 程瑞真 大数据 */
#pragma once

#define max_row 20
#define max_col 20

void input(int& row, int& col, int& target, int& delay);
void array_move(CONSOLE_GRAPHICS_INFO* G2048_CGI, int row, int col, int g2048[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi, int direction, int& score, int& num);
int finish(int row, int col, int g2048[max_row][max_col]);
void fill(const CONSOLE_GRAPHICS_INFO* G2048_CGI, int row, int col, int g2048[max_row][max_col], const BLOCK_DISPLAY_INFO* const bdi);
void step_of_2048(CONSOLE_GRAPHICS_INFO* pG2048_CGI, int row, int col, int target, int g2048[max_row][max_col], bool& remake, int& maxscore);
void print_end_of_game(const CONSOLE_GRAPHICS_INFO* G2048_CGI, int result);
int find_max(const CONSOLE_GRAPHICS_INFO* G2048_CGI, int g2048[max_row][max_col]);