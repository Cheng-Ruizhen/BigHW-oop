/* 2152046 ��14 ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include "lib_mto10_net_tools.h"
#include"cmd_console_tools.h"
#include"90-b2.h"
using namespace std;

#define		MAX_ROW_NUM		8
#define		MAX_COL_NUM		10

int game_progress(mto10_net_tools& client,char choice)
{
	char svrpack[RECVBUF_LEN];	//��Ŵ�Server���յ�������
	char row;
	int col;
	const char* char_map;
	int i = 0;
	if(choice=='B')
		cct_setconsoleborder(8 * client.get_col() + 5, 4 * client.get_row() + 11, 8 * client.get_col() + 5, 4 * client.get_row() + 11);
	while (1) {
		/* -------------------
			Client=>Server
		   ------------------- */
		   /* ���ݷ���˷����ĵ�ͼ������һ�������꣨ĿǰΪ�ֹ�������ʽ����Ҫ��Ϊ�Զ�����ѽ����꣩
			  ��ʾ�����Խ�client.get_map()�ķ���ֵ���Ƶ��ַ������У��ٻ�ԭΪ���Լ��Ķ�ά��������
		   */
		int map[10][12] = { 0 }, v[10][12] = { 0 }, x[2] = { 1,1 };
		char_map = client.get_map();
		for (int i = 1; i <= client.get_row(); i++) {
			for (int j = 1; j <= client.get_col(); j++) {
				map[i][j] = char_map[j - 1 + (i - 1) * client.get_col()] - '0';
			}
		}
		solution(client.get_row(), client.get_col(), map, v, x);
		if(choice=='A')
			client.print_map(); //��������Ϊʾ�⣬ͼ�ν����в��ܵ��ã�Ҫ�Լ�дת������
		else if (choice == 'B') {
			if (i == 0)
				print_console(client.get_row(), client.get_col(), map, choice);
			cct_setcolor();
			cct_gotoxy(0, 0);
			cout << "Step=" << client.get_step() << " Score=" << client.get_score() << "                         ";
			is_search(client.get_row(), client.get_col(), x[0], x[1], map, v);
			for (int i = 1; i <= client.get_row(); i++) {
				for (int j = 1; j <= client.get_col(); j++) {
					cct_showstr(8 * j - 4, 4 * i - 1, "������", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 4, 4 * i, "��", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showch(8 * j - 2, 4 * i, (char)(map[i][j] + '0'), map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 1, 4 * i, " ��", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 4, 4 * i + 1, "������", map[i][j] % 6 + 1, COLOR_BLACK);
				}
			}
			if (client.is_gameover()==0) {
				for (int i = 1; i <= client.get_row(); i++) {
					for (int j = 1; j <= client.get_col(); j++) {
						if (v[i][j] == 1) {
							cct_showstr(8 * j - 4, 4 * i - 1, "������", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 4, 4 * i, "��", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showch(8 * j - 2, 4 * i, (char)(map[x[0]][x[1]] + '0'), map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 1, 4 * i, " ��", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 4, 4 * i + 1, "������", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
						}
					}
				}
			}
			Sleep(200);
			cct_setcolor();
			int max = change(client.get_row(), client.get_col(), x[0], x[1], map, v);
			print_blank(client.get_row(), client.get_col(), map);
			Sleep(100);
			cct_setcolor();
			int p = x[0], q = x[1];
			cct_showstr(8 * x[1] - 4, 4 * x[0] - 1, "������", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 4, 4 * x[0], "��", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showch(8 * x[1] - 2, 4 * x[0], (char)(map[x[0]][x[1]] + '0'), map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 1, 4 * x[0], " ��", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 4, 4 * x[0] + 1, "������", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			down(client.get_row(), client.get_col(), map, v, x[0], x[1], choice);
			Sleep(100);
			for (int i = 1; i <= client.get_row(); i++) {
				for (int j = 1; j <= client.get_col(); j++) {
					v[i][j] = 0;
				}
			}
			x[0] = p, x[1] = q;
		}
		i++;
		/* ��Ϸ�ѽ������ٶ���*/
		if (client.is_gameover())
			break;

		row = char('A' + x[0] - 1);
		col = x[1] - 1;


		if (row >= 'a' && row <= 'j')
			row -= 32; //�����Сд��תΪ��д
		client.send_coordinate(row, col);

		/* -------------------
			Server=>Client
		   ------------------- */
		   /* �ȴ�Server�˵�gameprogress */
		if (client.get_gameprogress_string(svrpack) < 0) {
			return -1;
		}
		/* ��ӡ�յ���ԭʼ�ַ��� */
		if(choice=='A')
			cout << "ServerӦ�� : " << endl << svrpack << endl;
	}//end of while(1)

	return 0;
}

int main_net(char choice)
{
	const char* my_no = "2152046";		//����ѧ��
	const char* my_pwd = "crz135799.";	//��������

	int wanted_row, wanted_col, wanted_id, wanted_delay;

#if 1 //�������룬�˴���Ϊ #if 0 ���ʾ�������� #else �е����
	wanted_row = 5;			//�˴������޸�Ϊ�����������ʽ
	wanted_col = 5;			//�˴������޸�Ϊ�����������ʽ
	wanted_id = -1;		//idΪ�����������Ϸ��ţ������-1����ʾ���������������[0..2^31-1]���ʾ��̶���ţ���Ź̶�����ÿ�θ����ĳ�ʼ���󼰺���������ͬ��������ԣ�
	wanted_delay = 60 * 1000;	//delayΪ�����ÿ��Ӧ����ʱ�ӣ���λms
#else
	wanted_row = -1;
	wanted_col = -1;
	wanted_id = -1;
	wanted_delay = -1;
#endif
	mto10_net_tools client;

	if (choice == 'A' || choice == 'B') {
		/* ��client������е�debug���أ�����ʱ�ɴ򿪣���ͼ�ν���ʱ��رգ� */
		client.set_debug_switch(false);
		/* ���ӷ����� */
		if (client.connect() < 0) {
			/* ����ʧ����Ϣ����û��debug_switch����ӡ */
			cout << "���ӷ�����ʧ��!" << endl;
			return -1;
		}

		/* ��Server�˷�����֤��Ϣ������ */
		if (client.send_parameter(my_no, '*', my_pwd, wanted_row, wanted_col, wanted_id, wanted_delay) < 0) {
			cout << "������֤��������Ϣʧ��!" << endl;
			return -1;
		}

		/* �ȴ�Server���·���������Ϸ��ʼ��*/
		if (client.wait_for_game_start() < 0) {
			cout << "�ȴ�GameStartʧ��!" << endl;
			return -1;
		}

		if (choice == 'A') {
			/* ��ӡ�ӷ������յ��Ĳ�����������Ϸ�Դ˲���Ϊ׼����������6��6�У�����8��10�У��򱾴���Ϸ��8��10��Ϊ׼��
		   �����ӡ����Ϣ��ͼ�λ������������ȥ�� */
			cout << "������������Ϣ : " << endl;
			cout << "  ���� : " << client.get_row() << endl;
			cout << "  ���� : " << client.get_col() << endl;
			cout << "  ID�� : " << client.get_gameid() << endl;
			cout << "  ��ʱ : " << client.get_delay() / 1000.0 << "(��)" << endl;
			cout << "  ��ͼ : " << client.get_map() << endl;
		}
		/* ��ͼ���е�ֵΪ1-9��A-F���ֱ��ʾ1-15��ֵ��
			   ���磺
				 ������������=3
				 ������������=4
				 ���������ص�ͼ=123A85CF2123
			   ���ʾ��ǰ��ͼΪ(����������ʾ��)
				 1 2 3  10
				 8 5 12 15
				 2 1 2  3
				��Ϸ����������Ϊ���ϳɵ�16�����κοɺϳ�Ԫ�� */

				/* ������Ϸ��������
				   1���յ�Server��GameOver�򷵻�0����Ϸ����
				   2���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
		if (game_progress(client, choice) < 0) {
			client.close();
			return -1;
		}
	}
	else {
		int num = 0;
		client.set_debug_switch(false);
		while (num < 64) {
			num++;
			mto10_net_tools client;
			/* ���ӷ����� */
			if (client.connect() < 0) {
				/* ����ʧ����Ϣ����û��debug_switch����ӡ */
				cout << "���ӷ�����ʧ��!" << endl;
				continue;
			}

			/* ��Server�˷�����֤��Ϣ������ */
			if (client.send_parameter(my_no, '-', my_pwd, wanted_row, wanted_col, wanted_id, wanted_delay) < 0) {
				cout << "������֤��������Ϣʧ��!" << endl;
				continue;
			}

			/* �ȴ�Server���·���������Ϸ��ʼ��*/
			if (client.wait_for_game_start() < 0) {
				cout << "�ȴ�GameStartʧ��!" << endl;
				continue;
			}

			if (game_progress(client, choice) < 0) {
				client.close();
				continue;
			}
			cout << setiosflags(ios::left);
			cout << "���=" << setw(2) << num << " MAPID=" << setw(10) << client.get_gameid() << " ,��Ϸ���������յ÷֣�" << client.get_score() << endl;
			client.close();
		}
	}
	if (choice == 'B') {
		cct_gotoxy(0, client.get_col() * 4 + 4);
		cct_setcolor();
	}
	cout << "��Ϸ����" << endl;

	return 0;
}

void solution(int row, int col, int map[10][12], int v[10][12],int x[2])
{
	int min = 1, num[10][12] = { 0 }, num1[10][12] = { 0 };
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			no_search(row, col, i, j, map, v);
			for (int x = 1; x <= row; x++) {
				for (int y = 1; y <= col; y++) {
					if (v[x][y] != 0) {
						num[i][j]++;
						v[x][y] = 0;
					}
				}
			}
			if (num[i][j] > 1) {
				min = map[i][j];
			}
		}
	}
	int i, j;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (map[i][j] < min && num[i][j]>1)
				min = map[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (map[i][j] == min && num[i][j] > 1) {
				if (row <= 6 && col <= 7) {
					x[0] = i, x[1] = j;
					return;
				}
				num1[i][j] = num[i][j];
			}
		}
	}
	int min_num = 0;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] != 0)
				min_num = num1[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] < min_num && num1[i][j]>1)
				min_num = num1[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] == min_num) {
				x[0] = i, x[1] = j;
				return;
			}
		}
	}
}
