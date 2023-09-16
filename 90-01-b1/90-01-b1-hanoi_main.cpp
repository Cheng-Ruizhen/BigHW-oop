/* 2152046 信14 程瑞真 */

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

     本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
#include<iostream>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"90-01-b1-hanoi.h"
#include"../include/menu.h"
using namespace std;
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
	/* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
	cct_setconsoleborder(120, 40, 120, 9000);

    const char option[][100] = {
            "1.基本解",
            "2.基本解(步数记录)",
            "3.内部数组显示(横向)",
            "4.内部数组显示(纵向+横向)",
            "5.图形解-预备-画三个圆柱",
            "6.图形解-预备-在起始柱上画n个盘子",
            "7.图形解-预备-第一次移动",
            "8.图形解-自动移动版本",
            "9.图形解-游戏版",
            "0.退出"
    };
	while (1) {
        char src, tmp, dst, * a2 = &src, * a3 = &tmp, * a4 = &dst, choice;
        while (choice = menu(option,10)) {
            if (choice >= '0' && choice <= '9')
                break;
            else
                cct_cls();
        }
        int m = 0, n, * a1 = &n;
        if (choice == '0')
            return 0;
        if (choice != '5') {
            shuru(*a1, *a2, *a3, *a4);
            tmp = 'A' + 'B' + 'C' - src - dst;
            fuzhi(src, n);
            if (choice == '6' || choice == '7' || choice == '9') {
                cct_cls();
                cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;
                disk(n, src, dst);
                if (choice == '7') {
                    if (n % 2 == 1)
                        movedisk(1, src, dst, choice);
                    else
                        movedisk(1, src, tmp, choice);
                }
                else if (choice == '9') {
                    my_sleep(n, src, dst, '9');
                    game(n, src, dst);
                }
            }
            else {
                if (choice == '4' || choice == '8') {
                    m = my_sleep(n, src, dst, choice);
                }
                if (choice == '8') {
                    disk(n, src, dst);
                    if (m == 0) {
                        while (1) {
                            char ch = _getch();
                            if (ch == '\r')
                                break;
                        }
                    }
                }
                hanoi(src, tmp, dst, n, choice);
                cout << endl;
            }
            recover(dst, n);
        }
        else {
            cct_cls();
            column();
        }
        if (choice == '4' || choice == '8' || choice == '9')
            cct_gotoxy(0, 39);
        cout << "按回车键继续";
        while (1) {
            int ch = _getch();
            if (ch == '\r')
                break;
        }
        cct_cls();
	}
}
