/* 2152046 程瑞真 大数据 */
#pragma once
#include <string>
#include "../include/class_aat.h"
using namespace std;

/* 加入自己需要的定义 */
enum OPT_ARGS { OPT_ARGS_HELP = 0, OPT_ARGS_DEBUG, OPT_ARGS_FILE1, OPT_ARGS_FILE2, OPT_ARGS_TRIM, OPT_ARGS_LINEOFFSET, OPT_ARGS_IGNORE_BLANK, OPT_ARGS_CR_CRLF_NOT_EQUAL, OPT_ARGS_MAX_DIFF, OPT_ARGS_MAX_LINE, OPT_ARGS_DISPLAY };

string IgnoreSpaces(string str, string TrimType);
int FirstTimeDiff(string str1, string str2, int eof1, int eof2);
void display_normal(string str1, string str2, int eof1, int eof2);
void display_detailed(string str1, string str2, int eof1, int eof2);
int display(int row1, int row2, string str1, string str2, int eof1, int eof2, string DisplayType, bool debug);
int analyse(args_analyse_tools* const args);
int basic_information(args_analyse_tools* const args);