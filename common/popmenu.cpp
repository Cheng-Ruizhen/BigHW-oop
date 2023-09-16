/* 2152046 ������ ������ */
#include <iostream>
#include <string>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж���ʾ���������Ƿ�Ϊ�������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void checkchinese(string& str, int menu_col)
{
	int len = str.size();
	if (len <= menu_col * 4) //һ���п���װ���������
		return;
	else {
		int charnum = 0;
		for (int i = 0; i < menu_col * 4; i++) {
			if (str[i] >= 33 && str[i] <= 126)
				charnum++;
		}
		if (charnum % 2 != 0) { //����������ĸ
			str[menu_col * 4 - 1] = ' ';
		}
	}
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ʽ�˵�
  ���������
  �� �� ֵ��
  ˵    �������ڲ���gmw����Ϸ������ж������в����������ϵ��ʵ����=menu_row * 2��ʵ����=menu_col * 4
***************************************************************************/
string pop_menu(string* opt, int num, int menu_row, int menu_col)
{
	CONSOLE_GRAPHICS_INFO Menu_CGI; //����һ���˵�CGI

	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&Menu_CGI, menu_row, menu_col);

	gmw_set_font(&Menu_CGI, "������", 16);				//����
	gmw_set_ext_rowcol(&Menu_CGI, 3, 3, 21, 21);       //��������
	Menu_CGI.is_need_mouse = 0;                         //����Ҫ������
	Menu_CGI.CFI.separator = 0;                         //ɫ��֮�䲻��Ҫ�ָ���

	gmw_draw_frame(&Menu_CGI);
	Menu_CGI.SLI.top_start_x++;
	gmw_status_line(&Menu_CGI, TOP_STATUS_LINE, "��ѡ���ļ�");

	int pos = 0; //ָ��λ�ã���Χ[0 - menu_row-1]
	int first = 0; //�˵��е�һ��ѡ����opt�е��±�
	int start_x = Menu_CGI.start_x + 2, start_y = Menu_CGI.start_y + 2; //ѡ�ʼλ��
	while (1)
	{
		for (int i = 0; i < 2 * menu_row && i < num; i++) {
			string temp = opt[first + i];
			checkchinese(temp, menu_col);
			if (i == pos)
				cct_showstr(start_x, start_y + i, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
			else
				cct_showstr(start_x, start_y + i, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
		}

		int maction, mrow = 0, mcol = 0;
		int keycode1, keycode2;
		int ret;
		while (1) {
			int status = 0;
			ret = gmw_read_keyboard_and_mouse(&Menu_CGI, maction, mrow, mcol, keycode1, keycode2);
			switch (keycode1) {
			case 0xE0:
				switch (keycode2) {
				case KB_ARROW_UP:
					if (pos == 0) { //ָ��ͷ��ѡ��
						if (first != 0) { //��ͷ��ѡ��ǵ�һ��ѡ��
							first--;
							status = 1;
						}
					}
					else { //ָ���м��ѡ��
						string temp = opt[first + pos];
						checkchinese(temp, menu_col);
						cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
						pos--;
						temp = opt[first + pos];
						checkchinese(temp, menu_col);
						cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
					}
					break;
				case KB_ARROW_DOWN:
					if (pos == 2 * menu_row - 1) { //ָ���β��ѡ��
						if (first != (num - 2 * menu_row - 1)) { //���������¹���
							first++;
							status = 1;
						}
					}
					else { //ָ���м��ѡ��
						if (pos != num - 1) { //���ñ�֤����ָ�����һ��Ԫ��
							string temp = opt[first + pos];
							checkchinese(temp, menu_col);
							cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_BLACK, COLOR_WHITE, 1, menu_col * 4);
							pos++;
							temp = opt[first + pos];
							checkchinese(temp, menu_col);
							cct_showstr(start_x, start_y + pos, temp.c_str(), COLOR_WHITE, COLOR_BLACK, 1, menu_col * 4);
						}
					}
					break;
				} //switch1
				break;
			case '\n':
			case '\r':
				return opt[first + pos]; //ѭ������
			} //switch2

			if (status == 1)
				break;
		} //while1
	} //while2
}