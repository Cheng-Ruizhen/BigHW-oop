/* 215246 程瑞真 大数据 */
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
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */
   /***************************************************************************
	 函数名称：
	 功    能：保证中文制表符两字节的要求
	 输入参数：cosnt char* f	        ：要改为的中文制表符
			   char* pCGI_f		    ：结构体中表示这一位置制表符的指针
	 返 回 值：
	 说    明：
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
	 函数名称：
	 功    能：画一个小色块
	 输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
	           int bgcolor                          ：色块的背景色
			   int fgcolor                          ：色块的前景色
			   char* ch                             ：色块中所要打印的字符
	 返 回 值：
	 说    明：没有规定打印位置，使用本函数前需先跳转到想要打印的位置，且无延时
	           主要用于显示色块移动时的动画效果
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
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = row;
	pCGI->col_num = col;

	/* 下状态栏位置随之变化 */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * row + 2 - pCGI->CFI.separator;

	/* 状态栏宽度随之变化 */
	pCGI->SLI.width = pCGI->CFI.bwidth * col + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (strcmp(fontname, "Terminal") && strcmp(fontname, "新宋体")) //若设置其他字体则直接返回
		return 0;
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	strcpy(pCGI->CFT.font_type, fontname);
	cct_setfontsize(fontname, fs_high, fs_width);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;

	/* 主框架起始位置随之改变 */
	pCGI->start_x = left_cols;
	pCGI->start_y = up_lines;

	/* 上下状态栏位置随之改变 */
	pCGI->SLI.top_start_x = pCGI->start_x;
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
		gmw_set_frame_linetype(pCGI, "╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬");
	else if (type == 2)
		gmw_set_frame_linetype(pCGI, "┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋");
	else if (type == 3)
		gmw_set_frame_linetype(pCGI, "╒", "╘", "╕", "╛", "═", "┃", "╤", "╧", "╞", "╡", "╪");
	else if (type == 4)
		gmw_set_frame_linetype(pCGI, "╓", "╙", "╖", "╜", "━", "║", "╥", "╨", "╟", "╢", "╫");
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.block_width = block_width % 2 == 0 ? block_width : block_width + 1;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.separator = separator;

	pCGI->CFI.block_high_ext = pCGI->CFI.separator;
	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;

	/* 下状态栏的位置随之改变 */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* 状态栏宽度随之变化 */
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.bgcolor = bgcolor != -1 ? bgcolor : pCGI->area_bgcolor;
	pCGI->CFI.fgcolor = fgcolor != -1 ? fgcolor : pCGI->area_fgcolor;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == 1)
		gmw_set_block_linetype(pCGI, "╔", "╚", "╗", "╝", "═", "║");
	else if(type==2)
		gmw_set_block_linetype(pCGI, "┏", "┗", "┓", "┛", "━", "┃");
	else if(type==3)
		gmw_set_block_linetype(pCGI, "╒", "╘", "╕", "╛", "═", "┃");
	else if(type==4)
		gmw_set_block_linetype(pCGI, "╓", "╙", "╖", "╜", "━", "║");
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_set_linetype(top_left,    pCGI->CBI.top_left);
	gmw_inner_set_linetype(lower_left,  pCGI->CBI.lower_left);
	gmw_inner_set_linetype(top_right,   pCGI->CBI.top_right);
	gmw_inner_set_linetype(lower_right, pCGI->CBI.lower_right);
	gmw_inner_set_linetype(h_normal,    pCGI->CBI.h_normal);
	gmw_inner_set_linetype(v_normal,    pCGI->CBI.v_normal);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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

	/* 下状态栏的位置随之改变 */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	/* 状态栏宽度随之变化 */
	pCGI->SLI.width = pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	/* 下状态栏的位置随之改变 */
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator;

	/* cmd窗口的大小随之变化 */
	pCGI->lines = pCGI->SLI.lower_start_y + pCGI->extern_down_lines + 5;
	pCGI->cols = pCGI->start_x + pCGI->SLI.width + pCGI->extern_right_cols + 1;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
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


	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* 改变 cmd 窗口大小和颜色 */
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_cls();

	int start_x = pCGI->start_x,
		start_y = pCGI->start_y + pCGI->top_status_line; //游戏主框架起始点
	if (pCGI->draw_frame_with_col_no) //先画列标
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
			if (i == (int)pCGI->draw_frame_with_col_no) //第一行
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
			else if (i == pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh * pCGI->row_num + 2 - pCGI->CFI.separator - 1) //最后一行
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
				if (!pCGI->CFI.separator) //无分割线
				{
					if (j == 2 * pCGI->draw_frame_with_row_no || j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2)
						cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					else
						cct_showstr(start_x + j, start_y + i, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				else //有分割线
				{
					if ((i - pCGI->draw_frame_with_col_no) % pCGI->CFI.bhigh == 0) //具有行分割线
					{
						if (j == 2 * pCGI->draw_frame_with_row_no) //第一列
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_left_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else if (j == pCGI->CFI.bwidth * pCGI->col_num + 2 * pCGI->draw_frame_with_row_no + 4 - 2 * pCGI->CFI.separator - 2) //最后一列
							cct_showstr(start_x + j, start_y + i, pCGI->CFI.v_right_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else if ((j - 2 * pCGI->draw_frame_with_row_no) % pCGI->CFI.bwidth == 0) //横竖交界处
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor); //恢复初始颜色
	int x, y;
	if (type == TOP_STATUS_LINE && pCGI->top_status_line)
	{
		cct_gotoxy(0, pCGI->SLI.top_start_y);//光标设到指定位置
		cout << setw(pCGI->cols - 1) << ' '; //先用空格清空整行
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
		cct_gotoxy(0, pCGI->SLI.lower_start_y);//光标设到指定位置
		cout << setw(pCGI->cols - 1) << ' '; //先用空格清空整行
		if (catchy_msg != NULL)
		{
			cct_showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, catchy_msg, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor, 1, (int)strlen(catchy_msg) < pCGI->SLI.width ? strlen(catchy_msg) : pCGI->SLI.width);
			cct_getxy(x, y);
			cct_showstr(x, y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width - strlen(catchy_msg) ? strlen(msg) : pCGI->SLI.width - strlen(catchy_msg));
		}
		else
			cct_showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, (int)strlen(msg) < pCGI->SLI.width ? strlen(msg) : pCGI->SLI.width);
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* 色块左上角的坐标 */
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
		/* 找出bdi_value所对应的显示值 */
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* 找出bdi_value所对应的显示值 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	int X, Y;
	int ret;
	int loop = 1;

	if(pCGI->is_need_mouse)
		cct_enable_mouse();  //允许使用鼠标
	cct_setcursor(CURSOR_INVISIBLE);	//关闭光标

	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);
		if (ret == CCT_MOUSE_EVENT) {
			if (MAction == MOUSE_RIGHT_BUTTON_CLICK) //若按下鼠标右键，返回CCT_MOUSE_EVENT
				return CCT_MOUSE_EVENT;
			else //否则先判断位置是否非法，再具体分析
			{
				int start_x = pCGI->start_x + 2 * pCGI->draw_frame_with_row_no,
					start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->top_status_line; //游戏主区域的起始点

				/* 判断位置是否非法 */
				bool legal = 1; //legal为 0 则表示非法
				if (X <= start_x + 1 || X >= start_x + pCGI->CFI.bwidth * pCGI->col_num)
					legal = 0; //在区域左右两边，不合法
				else if (Y <= start_y || Y >= pCGI->SLI.lower_start_y - 1)
					legal = 0; //在区域上下两边，不合法
				if (pCGI->CFI.separator) //有分割线的特殊情况(区域内的非法点)
				{
					if ((X - start_x) % pCGI->CFI.bwidth == 0 || (X - start_x - 1) % pCGI->CFI.bwidth == 0)
						legal = 0; //竖直分割线非法
					else if ((Y - start_y) % pCGI->CFI.bhigh == 0)
						legal = 0; //水平分割线非法
				}

				if (!legal) //若非法，则根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
				{
					//MRow = -1, MCol = -1;
					if (update_lower_status_line)
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法", NULL);
				}
				else //若合法，则根据鼠标操作返回相应值
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
							sprintf(temp, "[当前光标] %c行%d列", new_row + 'A', new_col);
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
	return 0; //此句可根据需要修改
}
