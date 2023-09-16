/* 2152046 信14 程瑞真 */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */
void shuru(int &n, char &src, char &tmp, char &dst);       //输入初始信息
void fuzhi(char src, int n);        //给数组赋初值
void recover(char dst, int n);             //初始化
void my_move(char src, int n, char dst);     //内部数组改变
void dayin(char src, char dst, char choice);     //纵向的变化
void column();      //打印三个圆柱
void disk(int n, char src, char dst);       //在起始柱上打印n个圆盘
void movedisk(int n, char src, char dst, char choice);     //移动圆盘
void time(int m);            //延时
void game(int n, char src, char dst);                //游戏版本
int my_sleep(int n, char src, char dst, char choice);        //用于输入第4、8题延时和打印初始表
void qiujie(char src, char dst, int n, char choice);     //根据选择，表示汉诺塔的每一步变化

/*        hanoi递归          */
void hanoi(char src, char tmp, char dst, int n, char choice);
