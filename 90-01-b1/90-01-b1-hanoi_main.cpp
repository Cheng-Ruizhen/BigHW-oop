/* 2152046 ��14 ������ */

/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����main����
	2����ʼ����Ļ
	3�����ò˵�������hanoi_menu.cpp�У�������ѡ��
	4������ѡ����ò˵������Ӧ��ִ�к�����hanoi_multiple_solutions.cpp�У�

     ���ļ�Ҫ��
	1����������ȫ�ֱ��������ⲿȫ�ֺ;�̬ȫ�֣�const��#define�������Ʒ�Χ�ڣ�
	2����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	3���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */
#include<iostream>
#include<conio.h>
#include"../include/cmd_console_tools.h"
#include"90-01-b1-hanoi.h"
#include"../include/menu.h"
using namespace std;
/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
	/* demo������ִ�д˾䣬��cmd��������Ϊ40��x120�У����������120�У�����9000�У���cmd�����Ҳ���д�ֱ�����ˣ�*/
	cct_setconsoleborder(120, 40, 120, 9000);

    const char option[][100] = {
            "1.������",
            "2.������(������¼)",
            "3.�ڲ�������ʾ(����)",
            "4.�ڲ�������ʾ(����+����)",
            "5.ͼ�ν�-Ԥ��-������Բ��",
            "6.ͼ�ν�-Ԥ��-����ʼ���ϻ�n������",
            "7.ͼ�ν�-Ԥ��-��һ���ƶ�",
            "8.ͼ�ν�-�Զ��ƶ��汾",
            "9.ͼ�ν�-��Ϸ��",
            "0.�˳�"
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
                cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " ��" << endl;
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
        cout << "���س�������";
        while (1) {
            int ch = _getch();
            if (ch == '\r')
                break;
        }
        cct_cls();
	}
}
