/* 215246 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */
   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ���֤�����Ʊ�����ֽڵ�Ҫ��
	 ���������cosnt char* f	        ��Ҫ��Ϊ�������Ʊ��
			   char* pCGI_f		    ���ṹ���б�ʾ��һλ���Ʊ����ָ��
	 �� �� ֵ��
	 ˵    ����
   ***************************************************************************/
static void gmw_inner_set_linetype(const char* f,char* pCGI_f)
{
	if (f == NULL)
		pCGI_f[0] = ' ', pCGI_f[1] = ' ', pCGI_f[2] = '\0';
	else if (strlen(f) == 1)
		pCGI_f[0] = f[0], pCGI_f[1] = ' ', pCGI_f[2] = '\0';
	else
		pCGI_f[0] = f[0], pCGI_f[1] = f[1], pCGI_f[2] = '\0';
	return;
}

/***************************************************************************
	 �������ƣ�
	 ��    �ܣ���һ��Сɫ��
	 ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
	           int bgcolor                          ��ɫ��ı���ɫ
			   int fgcolor                          ��ɫ���ǰ��ɫ
			   char* ch                             ��ɫ������Ҫ��ӡ���ַ�
	 �� �� ֵ��
	 ˵    ����û�й涨��ӡλ�ã�ʹ�ñ�����ǰ������ת����Ҫ��ӡ��λ�ã�������ʱ
	           ��Ҫ������ʾɫ���ƶ�ʱ�Ķ���Ч��
   ***************************************************************************/
static void gmw_inner_print_block(const CONSOLE_GRAPHICS_INFO* const pCGI,int bgcolor,int fgcolor, char* ch)
{
	int x, y;
	cct_getxy(x, y);
	if (pCGI->CBI.block_border) {
		cct_showstr(x, y, pCGI->CBI.top_left, bgcolor, fgcolor);
		for (int i = 2; i <= pCGI->CFI.block_width - 4; i += 2) {
			cct_showstr(x + i, y, pCGI->CBI.h_normal, bgcolor, fgcolor);
		}
		cct_showstr(x + pCGI->CFI.block_width - 2, y, pCGI->CBI.top_right, bgcolor, fgcolor);
	}
	for (int i = pCGI->CBI.block_border; i <= pCGI->CFI.block_high - pCGI->CBI.block_border - 1; i++) {
		if(pCGI->CBI.block_border)
			cct_showstr(x, y + i, pCGI->CBI.v_normal, bgcolor, fgcolor);
		for (int j = 2 * pCGI->CBI.block_border; j <= pCGI->CFI.block_width - 2 * pCGI->CBI.block_border - 1; j++)
		{
			if (i == (pCGI->CFI.block_high - 1) / 2 && j == (pCGI->CFI.block_width - strlen(ch)) / 2)
			{
				cct_showstr(x + j, y + i, ch, bgcolor, fgcolor);
				j += (strlen(ch) - 1);
			}
			else
				cct_showstr(x + j, y + i, " ", bgcolor, fgcolor);
		}
		if(pCGI->CBI.block_border)
			cct_showstr(x + pCGI->CFI.block_width - 2, y + i, pCGI->CBI.v_normal, bgcolor, fgcolor);
	}
	if (pCGI->CBI.block_border) {
		cct_showstr(x, y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bgcolor, fgcolor);
		for (int i = 2; i <= pCGI->CFI.block_width - 4; i += 2)
		{
			cct_showstr(x + i, y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bgcolor, fgcolor);
		}
		cct_showstr(x + pCGI->CFI.block_width - 2, y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bgcolor, fgcolor);
	}
	cct_setcolor();
}

/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = row;
	pCGI->col_num = col;

	/* ��״̬��λ����֮�仯 */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * row + 2 - pCGI->CFI.separator;

	/* ״̬�������֮�仯 */
	pCGI->SLI.width = pCGI->CFI.bwidth * col + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (cascade)
	{
		gmw_set_frame_color(pCGI, bgcolor, fgcolor);

		gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, bgcolor, fgcolor, bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, bgcolor, fgcolor, bgcolor, pCGI->SLI.top_catchy_fgcolor);
	}

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (strcmp(fontname, "Terminal") && strcmp(fontname, "������")) //����������������ֱ�ӷ���
		return 0;
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	strcpy(pCGI->CFT.font_type, fontname);
	cct_setfontsize(fontname, fs_high, fs_width);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == DELAY_OF_DRAW_FRAME) 
	{
		if (delay_ms < 0)
			pCGI->delay_of_draw_frame = 0;
		else
			pCGI->delay_of_draw_frame = delay_ms;
	}
	else if (type == DELAY_OF_DRAW_BLOCK)
	{
		if (delay_ms < 0)
			pCGI->delay_of_draw_block = 0;
		else
			pCGI->delay_of_draw_block = delay_ms;
	}
	else if (type == DELAY_OF_BLOCK_MOVED)
	{
		if (delay_ms < BLOCK_MOVED_DELAY_MS)
			pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS;
		else
			pCGI->delay_of_block_moved = delay_ms;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;

	/* �������ʼλ����֮�ı� */
	pCGI->start_x = left_cols;
	pCGI->start_y = up_lines;

	/* ����״̬��λ����֮�ı� */
	pCGI->SLI.top_start_x = pCGI->start_x;
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
		gmw_set_frame_linetype(pCGI, "�X", "�^", "�[", "�a", "�T", "�U", "�j", "�m", "�d", "�g", "�p");
	else if (type == 2)
		gmw_set_frame_linetype(pCGI, "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��");
	else if (type == 3)
		gmw_set_frame_linetype(pCGI, "�V", "�\", "�Y", "�_", "�T", "��", "�h", "�k", "�b", "�e", "�n");
	else if (type == 4)
		gmw_set_frame_linetype(pCGI, "�W", "�]", "�Z", "�`", "��", "�U", "�i", "�l", "�c", "�f", "�o");
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_set_linetype(top_left,          pCGI->CFI.top_left);
	gmw_inner_set_linetype(lower_left,        pCGI->CFI.lower_left);
	gmw_inner_set_linetype(top_right,         pCGI->CFI.top_right);
	gmw_inner_set_linetype(lower_right,       pCGI->CFI.lower_right);
	gmw_inner_set_linetype(h_normal,          pCGI->CFI.h_normal);
	gmw_inner_set_linetype(v_normal,          pCGI->CFI.v_normal);
	gmw_inner_set_linetype(h_top_separator,   pCGI->CFI.h_top_separator);
	gmw_inner_set_linetype(h_lower_separator, pCGI->CFI.h_lower_separator);
	gmw_inner_set_linetype(v_left_separator,  pCGI->CFI.v_left_separator);
	gmw_inner_set_linetype(v_right_separator, pCGI->CFI.v_right_separator);
	gmw_inner_set_linetype(mid_separator,     pCGI->CFI.mid_separator);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.block_width = block_width % 2 == 0 ? block_width : block_width + 1;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.separator = separator;

	pCGI->CFI.block_high_ext = pCGI->CFI.separator;
	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;

	/* ��״̬����λ����֮�ı� */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* ״̬�������֮�仯 */
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.bgcolor = bgcolor != -1 ? bgcolor : pCGI->area_bgcolor;
	pCGI->CFI.fgcolor = fgcolor != -1 ? fgcolor : pCGI->area_fgcolor;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
		gmw_set_block_linetype(pCGI, "�X", "�^", "�[", "�a", "�T", "�U");
	else if(type==2)
		gmw_set_block_linetype(pCGI, "��", "��", "��", "��", "��", "��");
	else if(type==3)
		gmw_set_block_linetype(pCGI, "�V", "�\", "�Y", "�_", "�T", "��");
	else if(type==4)
		gmw_set_block_linetype(pCGI, "�W", "�]", "�Z", "�`", "��", "�U");
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_set_linetype(top_left,    pCGI->CBI.top_left);
	gmw_inner_set_linetype(lower_left,  pCGI->CBI.lower_left);
	gmw_inner_set_linetype(top_right,   pCGI->CBI.top_right);
	gmw_inner_set_linetype(lower_right, pCGI->CBI.lower_right);
	gmw_inner_set_linetype(h_normal,    pCGI->CBI.h_normal);
	gmw_inner_set_linetype(v_normal,    pCGI->CBI.v_normal);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->SLI.is_top_status_line = on_off;
		pCGI->top_status_line = on_off;
	}
	else if (type == LOWER_STATUS_LINE)
	{
		pCGI->SLI.is_lower_status_line = on_off;
		pCGI->lower_status_line = on_off;
	}

	/* ��״̬����λ����֮�ı� */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor != -1 ? catchy_bgcolor : pCGI->area_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor != -1 ? catchy_fgcolor : COLOR_HYELLOW;
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor != -1 ? normal_bgcolor : pCGI->area_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor != -1 ? normal_fgcolor : pCGI->area_fgcolor;
	}
	else if (type == LOWER_STATUS_LINE)
	{
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor != -1 ? catchy_bgcolor : pCGI->area_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor != -1 ? catchy_fgcolor : COLOR_HYELLOW;
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor != -1 ? normal_bgcolor : pCGI->area_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor != -1 ? normal_fgcolor : pCGI->area_fgcolor;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	/* ״̬�������֮�仯 */
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	/* ��״̬����λ����֮�ı� */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd���ڵĴ�С��֮�仯 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
	pCGI->inited = CGI_INITED;

	pCGI->start_x = 0, pCGI->start_y = 0;
	pCGI->draw_frame_with_col_no = false, pCGI->draw_frame_with_row_no = false;
	pCGI->top_status_line = true, pCGI->lower_status_line = true;
	pCGI->extern_down_lines = 0, pCGI->extern_left_cols = 0, pCGI->extern_right_cols = 0, pCGI->extern_up_lines = 0;
	pCGI->is_need_mouse = 1;

	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI);
	gmw_set_font(pCGI);
	gmw_set_ext_rowcol(pCGI);

	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS);

	gmw_set_frame_linetype(pCGI);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);

	gmw_set_block_linetype(pCGI);
	gmw_set_block_border_switch(pCGI);

	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);

	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);


	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* �ı� cmd ���ڴ�С����ɫ */
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_cls();

	int start_x = pCGI->start_x,
		start_y = pCGI->start_y + pCGI->top_status_line; //��Ϸ�������ʼ��
	if (pCGI->draw_frame_with_col_no) //�Ȼ��б�
	{
		char temp[256];
		for (int i = 0; i < pCGI->col_num; i++) {
			if (i <= 99)
				sprintf(temp, "%d", i);
			else
				sprintf(temp, "**");
			cct_showstr(start_x + 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.block_width / 2 + 1 + i * pCGI->CFI.bwidth, start_y, temp, pCGI->area_bgcolor, pCGI->area_fgcolor);
		}
	}
	for (int i = pCGI->draw_frame_with_col_no; i < pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator; i++) {
		char temp[256];
		if ((i - pCGI->draw_frame_with_col_no - (pCGI->CFI.block_high + 1) / 2) % pCGI->CFI.bhigh == 0 && pCGI->draw_frame_with_row_no && i - pCGI->draw_frame_with_col_no - (pCGI->CFI.block_high + 1) / 2 >= 0) {
			int x = (i - pCGI->draw_frame_with_col_no - (pCGI->CFI.block_high + 1) / 2) / pCGI->CFI.bhigh;
			if (x < pCGI->row_num)
			{
				if (x < 26)
					sprintf(temp, "%c ", 'A' + x);
				else if (x < 52 && x >= 26)
					sprintf(temp, "%c ", 'a' + x - 26);
				else
					sprintf(temp, "* ");
				cct_showstr(start_x, start_y + i, temp, pCGI->area_bgcolor, pCGI->area_fgcolor);
			}
		}
		for (int j = 2 * pCGI->draw_frame_with_row_no; j < pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator; j += 2) {
			if (i == (int)pCGI->draw_frame_with_col_no) //��һ��
			{
				if (j == 2 * pCGI->draw_frame_with_row_no)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.top_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else if (j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.top_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else if ((j - 2 * pCGI->draw_frame_with_row_no) % pCGI->CFI.bwidth == 0 && pCGI->CFI.separator)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.h_top_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else if (i == pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator - 1) //���һ��
			{
				if (j == 2 * pCGI->draw_frame_with_row_no)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.lower_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else if (j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.lower_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else if ((j - 2 * pCGI->draw_frame_with_row_no) % pCGI->CFI.bwidth == 0 && pCGI->CFI.separator)
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.h_lower_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				else
					cct_showstr(start_x + j, start_y + i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else
			{
				if (!pCGI->CFI.separator) //�޷ָ���
				{
					if (j == 2 * pCGI->draw_frame_with_row_no || j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2)
						cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					else
						cct_showstr(start_x + j, start_y + i, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				else //�зָ���
				{
					if ((i - pCGI->draw_frame_with_col_no) % pCGI->CFI.bhigh == 0) //�����зָ���
					{
						if (j == 2 * pCGI->draw_frame_with_row_no) //��һ��
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_left_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else if (j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2) //���һ��
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_right_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else if ((j - 2 * pCGI->draw_frame_with_row_no) % pCGI->CFI.bwidth == 0) //�������紦
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.mid_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}
					else
					{
						if (j == 2 * pCGI->draw_frame_with_row_no || j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2 || (j - 2 * pCGI->draw_frame_with_row_no) % pCGI->CFI.bwidth == 0)
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else
							cct_showstr(start_x + j, start_y + i, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}
				}
			}
			Sleep(pCGI->delay_of_draw_frame);
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor); //�ָ���ʼ��ɫ
	int x, y;
	if (type == TOP_STATUS_LINE && pCGI->top_status_line)
	{
		cct_gotoxy(0, pCGI->SLI.top_start_y);//����赽ָ��λ��
		cout << setw(pCGI->cols - 1) << ' '; //���ÿո��������
		if (catchy_msg != NULL)
		{
			cct_showstr(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor, 1, (int)strlen(catchy_msg) < pCGI->SLI.width ? strlen(catchy_msg) : pCGI->SLI.width);
			cct_getxy(x, y);
			cct_showstr(x, y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width - (int)strlen(catchy_msg) ? strlen(msg) : pCGI->SLI.width - (int)strlen(catchy_msg));
		}
		else
			cct_showstr(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width ? strlen(msg) : pCGI->SLI.width);
	}
	else if (type == LOWER_STATUS_LINE && pCGI->lower_status_line)
	{
		cct_gotoxy(0, pCGI->SLI.lower_start_y);//����赽ָ��λ��
		cout << setw(pCGI->cols - 1) << ' '; //���ÿո��������
		if (catchy_msg != NULL)
		{
			cct_showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, catchy_msg, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor, 1, (int)strlen(catchy_msg) < pCGI->SLI.width ? strlen(catchy_msg) : pCGI->SLI.width);
			cct_getxy(x, y);
			cct_showstr(x, y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width - strlen(catchy_msg) ? strlen(msg) : pCGI->SLI.width - strlen(catchy_msg));
		}
		else
			cct_showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width ? strlen(msg) : pCGI->SLI.width);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* ɫ�����Ͻǵ����� */
	int x = pCGI->start_x + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	int y = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->top_status_line + 1 + row_no * pCGI->CFI.bhigh;

	if (bdi_value == BDI_VALUE_BLANK)
	{
		for (int i = 0; i < pCGI->CFI.block_high; i++) {
			for (int j = 0; j < pCGI->CFI.block_width; j += 2) {
				cct_showstr(x + j, y + i, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
		}
	}
	else
	{
		/* �ҳ�bdi_value����Ӧ����ʾֵ */
		const char* ch;
		int sta = 0;
		while (1) {
			if (bdi[sta].value == bdi_value) {
				ch = bdi[sta].content;
				break;
			}
			sta++;
		}

		char str[100] = { '\0' };
		if (ch != NULL) {
			for (unsigned i = 0; i < strlen(ch); i++)
				str[i] = ch[i];
		}
		else
			sprintf(str, "%d", bdi_value);

		int bgcolor, fgcolor;
		bgcolor = bdi[sta].bgcolor == -1 ? pCGI->CFI.bgcolor : bdi[sta].bgcolor;
		fgcolor = bdi[sta].fgcolor == -1 ? pCGI->CFI.fgcolor : bdi[sta].fgcolor;

		if (pCGI->CBI.block_border) {
			cct_showstr(x, y, pCGI->CBI.top_left, bgcolor, fgcolor);
			Sleep(pCGI->delay_of_draw_block);
			for (int i = 2; i <= pCGI->CFI.block_width - 4; i += 2) {
				cct_showstr(x + i, y, pCGI->CBI.h_normal, bgcolor, fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
			cct_showstr(x + pCGI->CFI.block_width - 2, y, pCGI->CBI.top_right, bgcolor, fgcolor);
			Sleep(pCGI->delay_of_draw_block);
		}
		for (int i = pCGI->CBI.block_border; i <= pCGI->CFI.block_high - pCGI->CBI.block_border - 1; i++) {
			if (pCGI->CBI.block_border) {
				cct_showstr(x, y + i, pCGI->CBI.v_normal, bgcolor, fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
			for (int j = 2 * pCGI->CBI.block_border; j <= pCGI->CFI.block_width - 2 * pCGI->CBI.block_border - 1; j++)
			{
				if (i == (pCGI->CFI.block_high - 1) / 2 && j == (pCGI->CFI.block_width - strlen(str)) / 2)
				{
					cct_showstr(x + j, y + i, str, bgcolor, fgcolor);
					j += (strlen(str) - 1);
				}
				else
					cct_showstr(x + j, y + i, " ", bgcolor, fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
			if (pCGI->CBI.block_border) {
				cct_showstr(x + pCGI->CFI.block_width - 2, y + i, pCGI->CBI.v_normal, bgcolor, fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
		}
		if (pCGI->CBI.block_border) {
			cct_showstr(x, y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bgcolor, fgcolor);
			Sleep(pCGI->delay_of_draw_block);
			for (int i = 2; i <= pCGI->CFI.block_width - 4; i += 2)
			{
				cct_showstr(x + i, y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bgcolor, fgcolor);
				Sleep(pCGI->delay_of_draw_block);
			}
			cct_showstr(x + pCGI->CFI.block_width - 2, y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bgcolor, fgcolor);
			Sleep(pCGI->delay_of_draw_block);
		}
		cct_setcolor();
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* �ҳ�bdi_value����Ӧ����ʾֵ */
	const char* ch;
	int sta = 0;
	while (1) {
		if (bdi[sta].value == bdi_value) {
			ch = bdi[sta].content;
			break;
		}
		sta++;
	}

	int bgcolor, fgcolor;
	bgcolor = bdi[sta].bgcolor == -1 ? pCGI->CFI.bgcolor : bdi[sta].bgcolor;
	fgcolor = bdi[sta].fgcolor == -1 ? pCGI->CFI.fgcolor : bdi[sta].fgcolor;

	char str[100] = { '\0' };
	if (ch != NULL) {
		for (unsigned i = 0; i < strlen(ch); i++)
			str[i] = ch[i];
	}
	else
		sprintf(str, "%d", bdi_value);

	int a = pCGI->start_x + 2 * pCGI->draw_frame_with_row_no + 2 + col_no * pCGI->CFI.bwidth;
	int b = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->top_status_line + 1 + row_no * pCGI->CFI.bhigh;
	if (direction <= 1)
	{
		int to_row = row_no + (2 * direction - 1) * distance, to_col = col_no;
		for (int z = row_no; z * (2 * direction - 1) < to_row * (2 * direction - 1); z += (2 * direction - 1))
		{
			cct_gotoxy(a, b);
			int y = b;
			for (int i = 1; i <= pCGI->CFI.bhigh; i++) {
				cct_gotoxy(a, y);
				char in[20] = { '\0' };
				memset(in, ' ', pCGI->CFI.block_width - 4 * pCGI->CBI.block_border);
				gmw_inner_print_block(pCGI, pCGI->CFI.bgcolor, pCGI->CFI.bgcolor, in);
				y += (2 * direction - 1);
				cct_gotoxy(a, y);
				gmw_inner_print_block(pCGI, bgcolor, fgcolor, str);
				if (pCGI->CFI.separator && i % pCGI->CFI.bhigh == 0) {
					for (int j = 0; j <= pCGI->CFI.block_width - 2; j += 2) {
						if (direction == UP_TO_DOWN)
							cct_showstr(a + j, y - 1, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else
							cct_showstr(a + j, y + pCGI->CFI.block_high, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}
				}
				Sleep(pCGI->delay_of_block_moved);
			}
			b += pCGI->CFI.bhigh * (2 * direction - 1);
		}
	}
	else
	{
		int to_row = row_no, to_col = col_no + (2 * direction - 5) * distance;
		for (int z = col_no; z * (2 * direction - 5) < to_col * (2 * direction - 5); z += (2 * direction - 5)) {
			int x = a;
			for (int i = 1; i <= pCGI->CFI.bwidth; i++) {
				cct_gotoxy(x, b);
				char in[20] = { '\0' };
				memset(in, ' ', pCGI->CFI.block_width - 4 * pCGI->CBI.block_border);
				gmw_inner_print_block(pCGI, pCGI->CFI.bgcolor, pCGI->CFI.bgcolor, in);
				x += (2 * direction - 5);
				cct_gotoxy(x, b);
				gmw_inner_print_block(pCGI, bgcolor, fgcolor, str);
				if (pCGI->CFI.separator && i % pCGI->CFI.bwidth == 0) {
					for (int j = 0; j <= pCGI->CFI.block_high - 1; j++) {
						if (direction == LEFT_TO_RIGHT)
							cct_showstr(x - 2, b + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else
							cct_showstr(x + pCGI->CFI.block_width, b + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}
				}
				Sleep(pCGI->delay_of_block_moved);
			}
			a += pCGI->CFI.bwidth * (2 * direction - 5);
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ����Ϊ�������ǵ��Ҽ������Ҫ���꣩
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int X, Y;
	int ret;
	int loop = 1;

	if(pCGI->is_need_mouse)
		cct_enable_mouse();  //����ʹ�����
	cct_setcursor(CURSOR_INVISIBLE);	//�رչ��

	while (loop) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = cct_read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);
		if (ret == CCT_MOUSE_EVENT) {
			if (MAction == MOUSE_RIGHT_BUTTON_CLICK) //����������Ҽ�������CCT_MOUSE_EVENT
				return CCT_MOUSE_EVENT;
			else //�������ж�λ���Ƿ�Ƿ����پ������
			{
				int start_x = pCGI->start_x + 2 * pCGI->draw_frame_with_row_no,
					start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->top_status_line; //��Ϸ���������ʼ��

				/* �ж�λ���Ƿ�Ƿ� */
				bool legal = 1; //legalΪ 0 ���ʾ�Ƿ�
				if (X <= start_x + 1 || X >= start_x + pCGI->CFI.bwidth * pCGI->col_num)
					legal = 0; //�������������ߣ����Ϸ�
				else if (Y <= start_y || Y >= pCGI->SLI.lower_start_y - 1)
					legal = 0; //�������������ߣ����Ϸ�
				if (pCGI->CFI.separator) //�зָ��ߵ��������(�����ڵķǷ���)
				{
					if ((X - start_x) % pCGI->CFI.bwidth == 0 || (X - start_x - 1) % pCGI->CFI.bwidth == 0)
						legal = 0; //��ֱ�ָ��߷Ƿ�
					else if ((Y - start_y) % pCGI->CFI.bhigh == 0)
						legal = 0; //ˮƽ�ָ��߷Ƿ�
				}

				if (!legal) //���Ƿ�������� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
				{
					//MRow = -1, MCol = -1;
					if (update_lower_status_line)
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�", NULL);
				}
				else //���Ϸ��������������������Ӧֵ
				{
					int new_row, new_col;
					for (new_row = 0; new_row < pCGI->row_num; new_row++) {
						if (Y >= start_y + new_row * pCGI->CFI.bhigh + 1 && Y <= start_y + (new_row + 1) * pCGI->CFI.bhigh - pCGI->CFI.separator)
							break;
					}
					for (new_col = 0; new_col < pCGI->col_num; new_col++) {
						if (X >= start_x + new_col * pCGI->CFI.bwidth + 2 && X <= start_x + (new_col + 1) * pCGI->CFI.bwidth + 1 - 2 * pCGI->CFI.separator)
							break;
					}

					if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK) {
						MRow = new_row, MCol = new_col;
						return CCT_MOUSE_EVENT;
					}
					else {
						if (new_row != MRow || new_col != MCol) {
							char temp[256];
							sprintf(temp, "[��ǰ���] %c��%d��", new_row + 'A', new_col);
							gmw_status_line(pCGI, LOWER_STATUS_LINE, temp, NULL);
							MRow = new_row, MCol = new_col;
							return CCT_MOUSE_EVENT;
						}
					}
				}
			}
		}
		else if (ret == CCT_KEYBOARD_EVENT)
			return CCT_KEYBOARD_EVENT;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}
