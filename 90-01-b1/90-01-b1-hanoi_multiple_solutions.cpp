/* 2152046 信14 程瑞真 */


/* ----------------------------------------------------------------------------------

     本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include"../include/cmd_console_tools.h"
#include"90-01-b1-hanoi.h"
using namespace std;
int num = 1, a[3] = { 0 }, b[3][10], m;
/***************************************************************************
  函数名称：
  功    能：输入初始信息
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void shuru(int &n,char &src,char &tmp,char &dst)
{
    while (1) {
        cout << "请输入汉诺塔的层数(1-10)：" << endl;
        cin >> n;
        if (cin.good() == 0 || n < 1 || n>10) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        break;
    }
    while (1) {
        cout << "请输入起始柱(A-C)：" << endl;
        cin >> src;
        if (src >= 'a') {
            src = char(src - 32);
        }
        if (src < 'A' || src>'C')
            continue;
        break;
    }
    while (1) {
        cout << "请输入目标柱(A-C)：" << endl;
        cin >> dst;
        if (dst >= 'a') {
            dst = char(dst - 32);
        }
        if (dst < 'A' || dst>'C')
            continue;
        else if (src == dst) {
            cout << "目标柱(" << dst << ")不能与起始柱(" << src << ")相同" << endl;
            continue;
        }
        break;
    }
}
/***************************************************************************
  函数名称：
  功    能：给数组赋初值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void fuzhi(char src,int n)
{
    int i;
    for (i = 0; i < n; i++) {
        b[src - 'A'][i] = n - i;
        a[src - 'A']++;
    }
}
/***************************************************************************
  函数名称：
  功    能：打印每步详细
  输入参数：
  返 回 值：
  说    明：用于3、4、8
***************************************************************************/
void my_move(char src, int n, char dst)
{
    int  i;
    cout << "第" << setw(4) << num << " 步( " << n << "#: " << src << "-->" << dst << ") ";
    b[dst - 'A'][a[dst - 'A']++] = b[src - 'A'][--a[src - 'A']];
    b[src - 'A'][a[src - 'A']] = 0;
    cout<<"A: ";
    for (i = 0; i < 10; i++) {
        if (b[0][i] != 0)
            cout << b[0][i] << " ";
        else
            cout << "  ";
    }
    cout<<"B: ";
    for (i = 0; i < 10; i++) {
        if (b[1][i] != 0)
            cout << b[1][i] << " ";
        else
            cout << "  ";
    }
    cout << "C: ";
    for (i = 0; i < 10; i++) {
        if (b[2][i] != 0)
            cout << b[2][i] << " ";
        else
            cout << "  ";
    }
    cout << endl;
}
/***************************************************************************
  函数名称：
  功    能：根据选择进行求解
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void qiujie(char src, char dst, int n, char choice)
{
	if (choice == '1') {
		cout << n << "# " << src << "---->" << dst << endl;
	}
	else if (choice == '2') {
		cout << "第" << setw(4) << num << " 步( " << n << "#: " << src << "-->" << dst << ")" << endl;
	}
	else if (choice == '3') {
		my_move(src, n, dst);
	}
    else if (choice == '4'||choice=='8') {
        int x = choice == '4' ? 0 : 17;
        cct_gotoxy(0, 15+x);
        my_move(src, n, dst);
        dayin(src, dst, choice);
        if (choice == '8')
            movedisk(n, src, dst, choice);
        time(m);
        if (m == 0)
            while (1) {
                char ch = _getch();
                if (ch == '\r')
                    break;
            }
    }
    num++;
}
/***************************************************************************
  函数名称：
  功    能：递归
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hanoi(char src,char tmp,char dst,int n,char choice)
{
	if (n==1) {
        qiujie(src, dst, n, choice);
	}
	else {
        hanoi(src, dst, tmp, n - 1, choice);
        qiujie(src, dst, n, choice);
        hanoi(tmp, src, dst, n - 1, choice);
	}
}
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void recover(char dst,int n)
{
    int i, j;
    for (i = 0; i < 3; i++) {
        a[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 10; j++) {
            b[i][j] = 0;
        }
    }
    num = 1;
}
/***************************************************************************
  函数名称：
  功    能：纵向详细
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void dayin(char src, char dst,char choice)
{
    int m = (choice == '4') ? 0 : 17;
    cct_gotoxy((src - 'A' + 1) * 10, 11 + m - a[src - 'A']);
    cout << "  ";
    cct_gotoxy((dst - 'A' + 1) * 10, 12 + m - a[dst - 'A']);
    cout << setw(2) << b[dst - 'A'][a[dst - 'A'] - 1];
}
/***************************************************************************
  函数名称：
  功    能：用于输入第4、8题延时和打印初始表
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int my_sleep(int n,char src, char dst,char choice)
{
    int  i, x = choice == '4' ? 0 : 17;
    if (choice != '9') {
        while (1) {
            cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)" << endl;
            cin >> m;
            if (!cin.good() || m < 0 || m>5) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            break;
        }
        cct_cls();
    }
    cct_gotoxy(9, 12 + x);
    cout << setw(25) << setfill('=') << '=' << endl;
    cout << setfill(' ');
    cct_gotoxy(11, 13 + x);
    cout << "A" << setw(10) << "B" << setw(10) << "C";
    for (i = 0; i < n; i++) {
        cct_gotoxy((src - 'A' + 1) * 10, 11 - i + x);
        cout << setw(2) << b[src - 'A'][i];
    }
    cct_gotoxy(0, 15 + x);
    cout << "初始:" << setw(17) << "A: ";
    for (i = 0; i < 10; i++) {
        if (b[0][i] != 0)
            cout << b[0][i] << " ";
        else
            cout << "  ";
    }
    cout << "B: ";
    for (i = 0; i < 10; i++) {
        if (b[1][i] != 0)
            cout << b[1][i] << " ";
        else
            cout << "  ";
    }
    cout << "C: ";
    for (i = 0; i < 10; i++) {
        if (b[2][i] != 0)
            cout << b[2][i] << " ";
        else
            cout << "  ";
    }
    if (choice != '9') {
        cct_gotoxy(0, 0);
        cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 " << m;
    }
    time(m);
    if (m == 0 && choice == '4')
        while (1) {
            char ch = _getch();
            if (ch == '\r')
                break;
        }
    return m;
}
/***************************************************************************
  函数名称：
  功    能：打印3个圆柱
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void column()
{
    cct_setcursor(CURSOR_INVISIBLE);
    int x, y;
    for (x = 1; x <=83; x++) {
        if ((x >= 1 && x <= 25) || (x >= 30 && x <= 54) || (x >= 59 && x <= 83))
            cct_showch(x, 16, ' ', COLOR_HYELLOW, COLOR_BLACK, 1);
    }
    for (y = 15; y > 3; y--) {
        cct_showch(13, y, ' ', COLOR_HYELLOW, COLOR_BLACK, 1);
        cct_showch(42, y, ' ', COLOR_HYELLOW, COLOR_BLACK, 1);
        cct_showch(71, y, ' ', COLOR_HYELLOW, COLOR_BLACK, 1);
        Sleep(100);
    }
    cct_setcolor();
    cct_gotoxy(0, 22);
}
/***************************************************************************
  函数名称：
  功    能：在起始柱打印n个圆盘
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void disk(int n, char src,char dst)
{
    cct_setcursor(CURSOR_INVISIBLE);
    column();
    int x, y, s = 13 + (src - 'A')*29, i = n, m = n;
    for (y = 15; y > 15 - n; y--) {
        for (x = s - m; x < s + m + 1; x++) {
            cct_showch(x, y, ' ', i, 0, 1);
        }
        Sleep(100);
        m--;
        i--;
    }
    cct_setcolor();
    cct_gotoxy(0, 22);
}
/***************************************************************************
  函数名称：
  功    能：移动圆盘
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void movedisk(int n, char src, char dst, char choice)
{
    cct_setcursor(CURSOR_INVISIBLE);
    int s1 = 13 + (src - 'A') * 29, s2 = 13 + (dst - 'A') * 29, x, y, p = s1 - s2, q;
    for (y = 14 - a[src - 'A'] + (choice == '7'); y >= 3; y--) {
        for (x = s1 - n; x < s1 + n + 1; x++) {
            cct_showch(x, y, ' ', n, 0, 1);
            if (x == s1)
                cct_showch(x, y + 1, ' ', COLOR_HYELLOW, 0, 1);
            else
                cct_showch(x, y + 1, ' ', 0, 0, 1);
        }
        if (choice == '7' || (choice == '8' && m == 0) || choice == '9')
            time(6);
        else
            time(m);
    }
    if (p < 0) {
        for (x = s1; x <= s2 - 1; x++) {
            q = x - n;
            cct_showch(q, 3, ' ', 0, 0, 1);
            cct_showch(q + 2 * n + 1, 3, ' ', n, 0, 1);
            if (choice == '7' || (choice == '8' && m == 0) || choice == '9')
                time(6);
            else
                time(m);
        }
    }
    else {
        for (x = s1; x >= s2 + 1; x--) {
            q = x + n;
            cct_showch(q, 3, ' ', 0, 0, 1);
            cct_showch(q - 2 * n - 1, 3, ' ', n, 0, 1);
            if (choice == '7' || (choice == '8' && m == 0) || choice == '9')
                time(6);
            else
                time(m);
        }
    }
    for (y = 3; y <= 16 - a[dst - 'A'] - (choice == '7'); y++) {
        for (x = s2 - n; x < s2 + n + 1; x++) {
            cct_showch(x, y, ' ', n, 0, 1);
            if (y > 4 && x == s2)
                cct_showch(x, y - 1, ' ', COLOR_HYELLOW, 0, 1);
            else
                cct_showch(x, y - 1, ' ', 0, 0, 1);
        }
        if (choice == '7' || (choice == '8' && m == 0) || choice == '9')
            time(6);
        else
            time(m);
    }
    cct_setcolor();
    cct_gotoxy(0, 22);
}
/***************************************************************************
  函数名称：
  功    能：根据选择进行延时
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void time(int m)
{
    switch (m) {
        case 1:
            Sleep(160);
            break;
        case 2:
            Sleep(80);
            break;
        case 3:
            Sleep(40);
            break;
        case 4:
        case 6:
            Sleep(20);
            break;
        case 5:
            Sleep(0);
            break;
    }
}
/***************************************************************************
  函数名称：
  功    能：游戏版
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void game(int n,char src,char dst)
{
    int x, y;
    char _src, _dst;
    while (1) {
        cct_gotoxy(0, 34);
        cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) : ";
        cct_getxy(x, y);
        _src = _getche();
        if (_src >= 'a') {
            _src = char(_src - 32);
        }
        if (_src == 'Q')
            return;
        _dst = _getche();
        cct_gotoxy(x, y);
        cout << "  ";
        if (_dst >= 'a') {
            _dst = char(_dst - 32);
        }
        if (_src <= 'C' && _src >= 'A' && _dst >= 'A' && _dst <= 'C' && _src != _dst) {
            if (a[_src - 'A'] == 0) {
                cout << endl << "源柱为空!";
                cct_gotoxy(0, 35);
                Sleep(1000);
                cout << "         ";
            }
            else {
                if (a[_dst - 'A'] != 0) {
                    if (b[_src - 'A'][a[_src - 'A'] - 1] > b[_dst - 'A'][a[_dst - 'A'] - 1]) {
                        cout << endl << "大盘压小盘，非法移动!";
                        cct_gotoxy(0, 35);
                        Sleep(1000);
                        cout << "                     ";
                    }
                    else {
                        cct_gotoxy(0, 32);
                        my_move(_src, b[_src - 'A'][a[_src - 'A'] - 1], _dst);
                        dayin(_src, _dst, 9);
                        if (a[_dst - 'A'] != 0)
                            movedisk(b[_dst - 'A'][a[_dst - 'A'] - 1], _src, _dst, 9);
                        num++;
                        if (a[dst - 'A'] == n) {
                            cct_gotoxy(0, 35);
                            cout << "游戏结束！！！";
                            return;
                        }
                    }
                    continue;
                }
                cct_gotoxy(0, 32);
                my_move(_src, b[_src - 'A'][a[_src - 'A'] - 1], _dst);
                dayin(_src, _dst, 9);
                if (a[_dst - 'A'] != 0)
                    movedisk(b[_dst - 'A'][a[_dst - 'A'] - 1], _src, _dst, 9);
                num++;
                if (a[dst - 'A'] == n) {
                    cct_gotoxy(0, 35);
                    cout << "游戏结束！！！";
                    return;
                }
            }
        }
    }
}