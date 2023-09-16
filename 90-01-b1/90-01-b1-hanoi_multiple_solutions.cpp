/* 2152046 ��14 ������ */


/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����ű� hanoi_main.cpp �и��ݲ˵�����ֵ���õĸ��˵����Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

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
  �������ƣ�
  ��    �ܣ������ʼ��Ϣ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void shuru(int &n,char &src,char &tmp,char &dst)
{
    while (1) {
        cout << "�����뺺ŵ���Ĳ���(1-10)��" << endl;
        cin >> n;
        if (cin.good() == 0 || n < 1 || n>10) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        break;
    }
    while (1) {
        cout << "��������ʼ��(A-C)��" << endl;
        cin >> src;
        if (src >= 'a') {
            src = char(src - 32);
        }
        if (src < 'A' || src>'C')
            continue;
        break;
    }
    while (1) {
        cout << "������Ŀ����(A-C)��" << endl;
        cin >> dst;
        if (dst >= 'a') {
            dst = char(dst - 32);
        }
        if (dst < 'A' || dst>'C')
            continue;
        else if (src == dst) {
            cout << "Ŀ����(" << dst << ")��������ʼ��(" << src << ")��ͬ" << endl;
            continue;
        }
        break;
    }
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ������鸳��ֵ
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ���ӡÿ����ϸ
  ���������
  �� �� ֵ��
  ˵    ��������3��4��8
***************************************************************************/
void my_move(char src, int n, char dst)
{
    int  i;
    cout << "��" << setw(4) << num << " ��( " << n << "#: " << src << "-->" << dst << ") ";
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
  �������ƣ�
  ��    �ܣ�����ѡ��������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void qiujie(char src, char dst, int n, char choice)
{
	if (choice == '1') {
		cout << n << "# " << src << "---->" << dst << endl;
	}
	else if (choice == '2') {
		cout << "��" << setw(4) << num << " ��( " << n << "#: " << src << "-->" << dst << ")" << endl;
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
  �������ƣ�
  ��    �ܣ��ݹ�
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�������ϸ
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ����������4��8����ʱ�ʹ�ӡ��ʼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int my_sleep(int n,char src, char dst,char choice)
{
    int  i, x = choice == '4' ? 0 : 17;
    if (choice != '9') {
        while (1) {
            cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)" << endl;
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
    cout << "��ʼ:" << setw(17) << "A: ";
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
        cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " �㣬��ʱ����Ϊ " << m;
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
  �������ƣ�
  ��    �ܣ���ӡ3��Բ��
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�����ʼ����ӡn��Բ��
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ��ƶ�Բ��
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�����ѡ�������ʱ
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ���Ϸ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void game(int n,char src,char dst)
{
    int x, y;
    char _src, _dst;
    while (1) {
        cct_gotoxy(0, 34);
        cout << "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) : ";
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
                cout << endl << "Դ��Ϊ��!";
                cct_gotoxy(0, 35);
                Sleep(1000);
                cout << "         ";
            }
            else {
                if (a[_dst - 'A'] != 0) {
                    if (b[_src - 'A'][a[_src - 'A'] - 1] > b[_dst - 'A'][a[_dst - 'A'] - 1]) {
                        cout << endl << "����ѹС�̣��Ƿ��ƶ�!";
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
                            cout << "��Ϸ����������";
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
                    cout << "��Ϸ����������";
                    return;
                }
            }
        }
    }
}