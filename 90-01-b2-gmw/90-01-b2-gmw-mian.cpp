/* 2152046 ������ ������ */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

int main()
{
	int row, col,target;
	input(row, col, target);
	int map[max_row][max_col] = { 0 };
	int v[max_row][max_col] = { 0 };

	/*   ��ʼ������   */
	srand((unsigned int)(time(NULL)));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j] = rand() % 3 + 1;
		}
	}

	CONSOLE_GRAPHICS_INFO Mto10_CGI; //����һ��CGI����

	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&Mto10_CGI);

	gmw_set_color(&Mto10_CGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_font(&Mto10_CGI, "Terminal", 16, 8);
	gmw_set_frame_style(&Mto10_CGI, 6, 3, false);	//ɫ����߿򣬿��Ϊ6(�����2λ����)���߶�Ϊ3(����һ��)
	gmw_set_frame_color(&Mto10_CGI, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_border_switch(&Mto10_CGI, true);
	gmw_set_rowno_switch(&Mto10_CGI, true);//��ʾ�к�
	gmw_set_colno_switch(&Mto10_CGI, true);//��ʾ�б�
	gmw_set_delay(&Mto10_CGI, DELAY_OF_BLOCK_MOVED, 10);

	step_of_merge_to_10(&Mto10_CGI, row, col, map, v, target);

	return 0;
}