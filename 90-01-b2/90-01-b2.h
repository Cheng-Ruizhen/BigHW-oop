#pragma once
/* 2152046 信14 程瑞真 */
#include"../include/消除类游戏函数集.h"   //宏定义max_row,max_col

#define wid  6
#define heig 3    //合成十的色块的宽和高
#define form  1     //边框采用实心线

void pivot(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);    //负责调用其它函数
void shuru(int& row, int& col, int& target, char choice);    //初始输入
//void no_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //非递归搜寻
void is_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //递归搜寻
//int tj_strcasecmp(const char s1[], const char s2[]);     //字符串数组的处理

/* 打印数表 append:1-正常打印 2-打印查找数组 3-打印颜色标识的数组 */
//void print(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int append);
/* 伪图形打印 */
void print_console(int row, int col, int map[max_row][max_col], char choice);
//void print_blank(int row, int col, int map[max_row][max_col]);

/* 数组变化三件套 */
int change(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //进行合并，并返回最大值
//void all_down(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int& rrow, int& ccol, char choice='0');     //下落除0并负责伪图形的下落
void fill(int row, int col, int max, int map[max_row][max_col],char choice='0');    //新值填充

int score(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);   //计算分数
//int finish(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);   //判断结束 0-结束
void int_to_char(int n, char* ch);

/*                            控制数组打印                                */
/* 命令行 */
void print_main_base(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);
/* 伪图形 */
void print_main_console(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);