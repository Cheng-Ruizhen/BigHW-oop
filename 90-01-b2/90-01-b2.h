#pragma once
/* 2152046 ��14 ������ */
#include"../include/��������Ϸ������.h"   //�궨��max_row,max_col

#define wid  6
#define heig 3    //�ϳ�ʮ��ɫ��Ŀ�͸�
#define form  1     //�߿����ʵ����

void pivot(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);    //���������������
void shuru(int& row, int& col, int& target, char choice);    //��ʼ����
//void no_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //�ǵݹ���Ѱ
void is_search(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //�ݹ���Ѱ
//int tj_strcasecmp(const char s1[], const char s2[]);     //�ַ�������Ĵ���

/* ��ӡ���� append:1-������ӡ 2-��ӡ�������� 3-��ӡ��ɫ��ʶ������ */
//void print(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int append);
/* αͼ�δ�ӡ */
void print_console(int row, int col, int map[max_row][max_col], char choice);
//void print_blank(int row, int col, int map[max_row][max_col]);

/* ����仯������ */
int change(int row, int col, int my_row, int my_col, int map[max_row][max_col], int v[max_row][max_col]);   //���кϲ������������ֵ
//void all_down(int row, int col, int map[max_row][max_col], int v[max_row][max_col], int& rrow, int& ccol, char choice='0');     //�����0������αͼ�ε�����
void fill(int row, int col, int max, int map[max_row][max_col],char choice='0');    //��ֵ���

int score(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);   //�������
//int finish(int row, int col, int map[max_row][max_col], int v[max_row][max_col]);   //�жϽ��� 0-����
void int_to_char(int n, char* ch);

/*                            ���������ӡ                                */
/* ������ */
void print_main_base(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);
/* αͼ�� */
void print_main_console(int row, int col, int target, int map[max_row][max_col], int v[max_row][max_col], char choice);