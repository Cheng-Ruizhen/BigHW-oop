/* 2152046 ������ ������ */
//  game : 2048
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"../include/cmd_console_tools.h"
#include"../include/cmd_gmw_tools.h"
#include"90-02-b2.h"

using namespace std;

int main()
{
	int maxscore = 0;
	while (1) {
		int row, col, target, delay;
		input(row, col, target, delay);
		int g2048[max_row][max_col] = { 0 };

		/* ��ʼ������ */
		srand((unsigned int)(time(NULL)));
		int x1 = rand() % row, x2, y1 = rand() % col, y2;
		while (1) {
			x2 = rand() % row;
			if (x2 != x1)
				break;
		}
		while (1) {
			y2 = rand() % col;
			if (y2 != y1)
				break;
		}

		g2048[x1][y1] = 2, g2048[x2][y2] = 2;

		CONSOLE_GRAPHICS_INFO G2048_CGI; //����һ��CGI����

		/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
		gmw_init(&G2048_CGI);

		gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
		gmw_set_font(&G2048_CGI, "������", 16);						//����
		gmw_set_frame_style(&G2048_CGI, 12, 5, true);					//��Ϸ��������ÿ��ɫ���12��5���зָ��ߡ�����ɫ����߿򣬿��Ϊ12(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
		gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
		gmw_set_block_border_switch(&G2048_CGI, true);				//Сɫ����߿�
		gmw_set_delay(&G2048_CGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS + delay);   //����ɫ���ƶ���ʱ
		gmw_set_status_line_switch(&G2048_CGI, TOP_STATUS_LINE, false);  //�ر���״̬��
		gmw_set_ext_rowcol(&G2048_CGI, 1);
		G2048_CGI.is_need_mouse = 0;                         //����Ҫ������

		bool remake = 0;
		step_of_2048(&G2048_CGI, row, col,target, g2048, remake, maxscore);

		if (remake == 0)
			break;
	}

	return 0;
}