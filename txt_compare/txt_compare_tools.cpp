/* 2152046 程瑞真 大数据 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include "txt_compare.h"
#include "../include/cmd_console_tools.h"
#include "../include/class_aat.h"
using namespace std;

/***************************************************************************
  函数名称：is_nullstring
  功    能：判断是否为空行
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool is_nullstring(string str)
{
	int len = str.size(), i = 0;
	for (; i < len; i++) {
		if (str[i] != ' ' && str[i] != '	')
			break;
	}
	if (len == 0 || i == len)
		return true;
	else
		return false;
}

/***************************************************************************
  函数名称：IgnoreSpaces
  功    能：
  输入参数：str      : 读入每行字符串
            TrimType : 空格的比较方式
  返 回 值：忽略掉所选空格后的字符串
  说    明：
***************************************************************************/
string IgnoreSpaces(string str, string TrimType)
{
	if (TrimType == "none")
		return str;
	else{
		string newstr;
		int leftpos = 0, rightpos = str.size() - 1;
		for (; leftpos < (int)(str.size()); leftpos++)
			if (str[leftpos] != ' ' && str[leftpos] != '	')
				break;
		for (; rightpos >= 0; rightpos--)
			if (str[rightpos] != ' ' && str[rightpos] != '	')
				break;
		if (leftpos > rightpos)
			newstr = "";
		else if (TrimType == "left")
			newstr = str.substr(leftpos);
		else if (TrimType == "right")
			newstr = str.substr(0, rightpos + 1);
		else
			newstr = str.substr(leftpos, rightpos - leftpos + 1);
		return newstr;
	}
}

/***************************************************************************
  函数名称：FirstTimeDiff
  功    能：返回两个字符串首次不同的下标
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int FirstTimeDiff(string str1, string str2, int eof1, int eof2)
{
	int len1 = str1.size(), len2 = str2.size(), i = 0;
	if (eof1 == 1 || eof2 == 1)
		return -1;
	for (; i < len1 && i < len2; i++)
		if (str1[i] != str2[i])
			break;
	return i;
}

/***************************************************************************
  函数名称：display_normal
  功    能：正常方式标出每一行的不同
  输入参数：string str1,str2  : 两文件的行字符串(若为eof则也赋成空串)
			int    eof1,eof2  : 两文件是否结束
  返 回 值：
  说    明：
***************************************************************************/
void display_normal(string str1, string str2, int eof1, int eof2)
{
	int len1 = str1.size(), len2 = str2.size();

	cout << "文件1 : ";
	for (int i = 0; i < len1; i++) {
		if (i >= len2 || str1[i] != str2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			if (str1[i] == '\r')
				cout << 'X';
			else
				cout << str1[i];
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		}
		else{
			if (str1[i] == '\r')
				cout << 'X';
			else
				cout << str1[i];
		}
	}
	if (eof1 == 1)
		cout << "<EOF>" << endl;
	else {
		if (len1 == 0) {
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			cout << "<EMPTY>";
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		}
		cout << "<CR>" << endl;
	}
	cout << "文件2 : ";
	for (int i = 0; i < len2; i++) {
		if (i >= len1 || str1[i] != str2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			if (str2[i] == '\r')
				cout << 'X';
			else
				cout << str2[i];
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		}
		else{
			if (str2[i] == '\r')
				cout << 'X';
			else
				cout << str2[i];
		}
	}
	if (eof2 == 1)
		cout << "<EOF>" << endl;
	else {
		if (len2 == 0) {
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			cout << "<EMPTY>";
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		}
		cout << "<CR>" << endl;
	}
}

/***************************************************************************
  函数名称：display_detailed
  功    能：详细方式标出每一行的不同
  输入参数：string str1,str2  : 两文件的行字符串(若为eof则也赋成空串)
			int    eof1,eof2  : 两文件是否结束
  返 回 值：
  说    明：
***************************************************************************/
void display_detailed(string str1, string str2, int eof1, int eof2)
{
	int len1 = str1.size(), len2 = str2.size(), len = max(len1, len2);

	cout << setw(8) << ' ' << setw((len / 10 + 1) * 10) << setfill('-') << "" << setfill(' ') << endl;
	cout << setw(8) << ' ';
	for (int i = 0; i < len / 10 + 1; i++)
		cout << setiosflags(ios::left) << setw(10) << i;
	cout << resetiosflags(ios::left) << endl;
	cout << setw(8) << ' ';
	for (int i = 0; i < (len / 10 + 1) * 10; i++)
		cout << i % 10;
	cout << endl;
	cout << setw(8) << ' ' << setw((len / 10 + 1) * 10) << setfill('-') << "" << setfill(' ') << endl;

	display_normal(str1, str2, eof1, eof2);

	string temp;
	cout << "文件1(HEX) : " << endl;
	if (len1 == 0) {
		if (eof1 == 1)
			cout << "<EOF>" << endl;
		else
			cout << "<EMPTY>" << endl;
	}
	else {
		for (int i = 0; i < ((len1 - 1) / 16 + 1) * 16; i++) {
			if (i % 16 == 0) {
				cout << setw(8) << setfill('0') << hex << i << " : ";
				temp.clear();
			}
			if (i % 16 == 8) {
				if (i < len1)
					cout << "- ";
				else
					cout << "  ";
			}
			if (i < len1)
				cout << hex << (int)(str1[i]) << " ";
			else
				cout << "   ";
			if (i < len1 && str1[i] >= 33 && str1[i] <= 126)
				temp += str1[i];
			else if (i < len1)
				temp += '.';
			if (i % 16 == 15)
				cout << ' ' << temp << endl;
		}
	}

	cout << "文件2(HEX) : " << endl;
	if (len2 == 0) {
		if (eof2 == 1)
			cout << "<EOF>" << endl;
		else
			cout << "<EMPTY>" << endl;
	}
	else {
		for (int i = 0; i < ((len2 - 1) / 16 + 1) * 16; i++) {
			if (i % 16 == 0) {
				cout << setw(8) << setfill('0') << hex << i << " : " << setfill(' ');
				temp.clear();
			}
			if (i % 16 == 8) {
				if (i < len2)
					cout << "- ";
				else
					cout << "  ";
			}
			if (i < len2)
				cout << hex << (int)(str2[i]) << " ";
			else
				cout << "   ";
			if (i < len2 && str2[i] >= 33 && str2[i] <= 126)
				temp += str2[i];
			else if (i < len2)
				temp += '.';
			if (i % 16 == 15)
				cout << ' ' << temp << endl;
		}
	}
	cout << dec << "";
}

/***************************************************************************
  函数名称：display
  功    能：调用上述两个显示函数
  输入参数：int    row1,row2  : 此时读取的行数
			string str1,str2  : 两文件的行字符串(若为eof则也赋成空串)
			int    eof1,eof2  : 两文件是否结束
			string DisplayType: 显示的方式（none/normal/detailed）
			bool   debug      : 是否需要debug形式输出
  返 回 值：
  说    明：
***************************************************************************/
int display(int row1, int row2, string str1, string str2, int eof1, int eof2, string DisplayType, bool debug)
{
	if (eof1 == 1 && eof2 == 1)
		return 0;

	int len1 = str1.size(), len2 = str2.size();
	int diffpos = FirstTimeDiff(str1, str2, eof1, eof2);

	if (diffpos == len1 && diffpos == len2 ) // 两串相等
	{
		if (debug) {
			if (len1 != 0)
				cout << "第[" << row1 << " / " << row2 << "] - 一致：" << str1 << "<CR>" << endl << endl;
			else
				cout << "第[" << row1 << " / " << row2 << "] - 一致：<EMPTY><CR>" << endl << endl;
		}
		return 0;
	}

	if (DisplayType == "none" && !debug)
		return 1;

	cout << "第[" << row1 << " / " << row2 << "]行 - ";
	if (eof1 == 1)
		cout << "文件1已结束/文件2仍有内容：" << endl;
	else if (eof2 == 1)
		cout << "文件1仍有内容/文件2已结束：" << endl;
	else if (diffpos == len2)
		cout << "文件1的尾部有多余字符：" << endl;
	else if (diffpos == len1)
		cout << "文件2的尾部有多余字符：" << endl;
	else
		cout << "第[" << diffpos << "]个字符开始有差异：" << endl;

	if (DisplayType == "normal" || (DisplayType != "detailed" && debug))
		display_normal(str1, str2, eof1, eof2);
	else if (DisplayType == "detailed")
		display_detailed(str1, str2, eof1, eof2);
	cout << endl;
	return 1;
}

/***************************************************************************
  函数名称：basic_information
  功    能：debug模式下基础信息的输出
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int basic_information(args_analyse_tools* const args)
{
	args_analyse_print(args);
	cout << endl;
	cout << "File1        : " << args[OPT_ARGS_FILE1].get_string() << endl;
	cout << "File2        : " << args[OPT_ARGS_FILE2].get_string() << endl;
	cout << "Trim         : " << args[OPT_ARGS_TRIM].get_string() << endl;
	cout << "Display      : " << args[OPT_ARGS_DISPLAY].get_string() << endl;
	cout << "Offset       : " << args[OPT_ARGS_LINEOFFSET].get_int() << endl;
	cout << "MaxDiff      : " << args[OPT_ARGS_MAX_DIFF].get_int() << endl;
	cout << "MaxLine      : " << args[OPT_ARGS_MAX_LINE].get_int() << endl;
	cout << "IgnoreBlank  : " << args[OPT_ARGS_IGNORE_BLANK].existed() << endl;
	cout << "CR/CRLF      : " << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() << endl;
	cout << "Debug        : " << args[OPT_ARGS_DEBUG].existed() << endl << endl;

	ifstream fin1, fin2;
	int memory1 = 0, memory2 = 0;
	int lines1 = 0, lines2 = 0;
	int maxline1 = 0, maxline2 = 0;
	int format1 = 0, format2 = 0; // 文件格式：0为Windows；1为Linux
	fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	if (!fin1.is_open()) {
		cout << "第1个文件[" << args[OPT_ARGS_FILE1].get_string() << "]无法打开." << endl;
		return -1;
	}
	fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!fin2.is_open()) {
		cout << "第2个文件[" << args[OPT_ARGS_FILE2].get_string() << "]无法打开." << endl;
		fin1.close();
		return -1;
	}
	string temp;
	while (!fin1.eof()) {
		temp.clear();
		getline(fin1, temp, '\n');
		memory1 += (temp.size() + 1);
		lines1++;
		maxline1 = max(maxline1, (int)(temp.size()));
		if (!fin1.eof() && (temp.size() == 0 || temp[temp.size() - 1] != '\r'))
			format1 = 1;
		else if (!fin1.eof())
			format1 = 0;
	}
	while (!fin2.eof()) {
		temp.clear();
		getline(fin2, temp, '\n');
		memory2 += (temp.size() + 1);
		lines2++;
		maxline2 = max(maxline2, (int)(temp.size()));
		if (!fin2.eof() && (temp.size() == 0 || temp[temp.size() - 1] != '\r'))
			format2 = 1;
		else if (!fin2.eof())
			format2 = 0;
	}

	cout << "第1个文件的基本信息：" << endl;
	cout << "====================================================================================================" << endl;
	cout << "文 件 名：" << args[OPT_ARGS_FILE1].get_string() << endl;
	cout << "大    小：" << memory1 - 1 << endl;
	cout << "行    数：" << lines1 - 1 << endl;
	cout << "最大行长：" << maxline1 + args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() << endl;
	cout << "文件格式：";
	if (format1 == 0)
		cout << "Windows" << endl;
	else
		cout << "Linux" << endl;
	cout << "====================================================================================================" << endl << endl;

	cout << "第2个文件的基本信息：" << endl;
	cout << "====================================================================================================" << endl;
	cout << "文 件 名：" << args[OPT_ARGS_FILE2].get_string() << endl;
	cout << "大    小：" << memory2 - 1 << endl;
	cout << "行    数：" << lines2 - 1 << endl;
	cout << "最大行长：" << maxline2 + args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() << endl;
	cout << "文件格式：";
	if (format2 == 0)
		cout << "Windows" << endl;
	else
		cout << "Linux" << endl;
	cout << "====================================================================================================" << endl << endl;

	int row1 = 1, row2 = 1;
	if (args[OPT_ARGS_LINEOFFSET].existed()) {
		if (args[OPT_ARGS_LINEOFFSET].get_int() < 0) {
			cout << "[" << args[OPT_ARGS_FILE1].get_string() << "] 跳过 [" << -args[OPT_ARGS_LINEOFFSET].get_int() << " ]行" << endl;
			fin1.close();
			fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
			for (int i = 0; i < -args[OPT_ARGS_LINEOFFSET].get_int(); i++) {
				temp.clear();
				if (!fin1.eof()) {
					getline(fin1, temp, '\n');
					temp = IgnoreSpaces(temp, args[OPT_ARGS_TRIM].get_string());
					if (args[OPT_ARGS_IGNORE_BLANK].existed() && is_nullstring(temp)) {
						cout << "第[* / " << row1 << "] - 为空：忽略." << endl << endl;
						i--;
					}
					else
						cout << temp << endl;
				}
				else
					cout << endl;
				row1++;
			}
		}
		else {
			cout << "[" << args[OPT_ARGS_FILE2].get_string() << "] 跳过 [" << args[OPT_ARGS_LINEOFFSET].get_int() << " ]行" << endl;
			fin2.close();
			fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
			for (int i = 0; i < args[OPT_ARGS_LINEOFFSET].get_int(); i++) {
				temp.clear();
				if (!fin2.eof()) {
					getline(fin2, temp, '\n');
					temp = IgnoreSpaces(temp, args[OPT_ARGS_TRIM].get_string());
					if (args[OPT_ARGS_IGNORE_BLANK].existed() && is_nullstring(temp)) {
						cout << "第[* / " << row2 << "] - 为空：忽略." << endl << endl;
						i--;
					}
					else
						cout << temp << endl;
				}
				else
					cout <<"#"<< endl;
				row2++;
			}
		}
	}
	fin1.close(), fin2.close();
	cout << endl;
	return 0;
}

/***************************************************************************
  函数名称：analyse
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int analyse(args_analyse_tools* const args)
{
	ifstream fin1, fin2;
	fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	if (!fin1.is_open()) {
		cout << "第1个文件[" << args[OPT_ARGS_FILE1].get_string() << "]无法打开." << endl;
		return -1;
	}
	fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!fin2.is_open()) {
		cout << "第2个文件[" << args[OPT_ARGS_FILE2].get_string() << "]无法打开." << endl;
		fin1.close();
		return -1;
	}

	int row1 = 1, row2 = 1; // 两文件的行数

	if (args[OPT_ARGS_LINEOFFSET].existed()) { // 统一行差
		string temp;
		if (args[OPT_ARGS_LINEOFFSET].get_int() < 0) {
			row1 -= args[OPT_ARGS_LINEOFFSET].get_int();
			for (int i = 0; i < -args[OPT_ARGS_LINEOFFSET].get_int(); i++)
				getline(fin1, temp, '\n');
		}
		else if (args[OPT_ARGS_LINEOFFSET].get_int() > 0) {
			row2 += args[OPT_ARGS_LINEOFFSET].get_int();
			for (int i = 0; i < args[OPT_ARGS_LINEOFFSET].get_int(); i++)
				getline(fin2, temp, '\n');
		}
	}

	if (args[OPT_ARGS_DEBUG].existed())
		basic_information(args);

	if (args[OPT_ARGS_DISPLAY].get_string() != "none" || args[OPT_ARGS_DEBUG].existed()) {
		cout << "比较结果输出：" << endl;
		cout << "====================================================================================================" << endl;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		cout << "起始行：" << row1 - 1 << " / " << row2 - 1 << endl;

	int count = 1, diffnum = 0;
	int eof1 = 0, eof2 = 0;
	string str1, str2;
	int status1 = 0, status2 = 0;
	while (!fin1.eof() || !fin2.eof())
	{
		count++;
		str1.clear(), str2.clear();
		getline(fin1, str1, '\n');
		getline(fin2, str2, '\n');

		eof1 = fin1.eof(), eof2 = fin2.eof();
		/* 若最后一行没回车，且忽略空行，进行特判 */
		if (args[OPT_ARGS_IGNORE_BLANK].existed()) {
			if (fin1.eof() && str1.size() != 0 && str1[str1.size() - 1] != '\r') {
				eof1 = 0;
				fin1.clear();
			}
			if (fin2.eof() && str2.size() != 0 && str2[str2.size() - 1] != '\r') {
				eof2 = 0;
				fin2.clear();
			}
		}

		if (!args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed()) {
			if (str1.size() > 0 && str1[str1.size() - 1] == '\r')
				str1.pop_back();
			if (str2.size() > 0 && str2[str2.size() - 1] == '\r')
				str2.pop_back();
		}

		str1 = IgnoreSpaces(str1, args[OPT_ARGS_TRIM].get_string());
		str2 = IgnoreSpaces(str2, args[OPT_ARGS_TRIM].get_string());

		if (args[OPT_ARGS_IGNORE_BLANK].existed()) {
			while (is_nullstring(str1)) {
				eof1 = (fin1.peek() == EOF) ? 1 : 0, eof2 = (fin2.peek() == EOF) ? 1 : 0;
				if (fin1.eof())
					break;
				if (args[OPT_ARGS_DEBUG].existed())
					cout << "第[" << row1 << " / *] - 为空：忽略." << endl << endl;
				getline(fin1, str1, '\n');
				if (!args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed())
					if (str1.size() > 0 && str1[str1.size() - 1] == '\r')
						str1.pop_back();
				str1 = IgnoreSpaces(str1, args[OPT_ARGS_TRIM].get_string());
				row1++;
			}
			while (is_nullstring(str2)) {
				eof1 = (fin1.peek() == EOF) ? 1 : 0, eof2 = (fin2.peek() == EOF) ? 1 : 0;
				if (fin2.eof())
					break;
				if (args[OPT_ARGS_DEBUG].existed())
					cout << "第[* / " << row2 << "] - 为空：忽略." << endl << endl;
				getline(fin2, str2, '\n');
				if (!args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed())
					if (str2.size() > 0 && str2[str2.size() - 1] == '\r')
						str2.pop_back();
				str2 = IgnoreSpaces(str2, args[OPT_ARGS_TRIM].get_string());
				row2++;
			}
		}

		/* normal/detailed以在该函数中输出 */
		diffnum += display(row1++, row2++, str1, str2, eof1, eof2, args[OPT_ARGS_DISPLAY].get_string(), args[OPT_ARGS_DEBUG].existed());

		/* 不同行超过最大限制 */
		if (args[OPT_ARGS_MAX_DIFF].existed()) {
			if (args[OPT_ARGS_MAX_DIFF].get_int() != 0 && diffnum > args[OPT_ARGS_MAX_DIFF].get_int())
				break;
		}

		/* 超过允许检查的最大行数 */
		if (args[OPT_ARGS_MAX_LINE].existed()) {
			if (args[OPT_ARGS_MAX_LINE].get_int() != 0 && count > args[OPT_ARGS_MAX_LINE].get_int())
				break;
		}

		/* display额外参数为none时，找到一处不同，即可直接退出 */
		if (args[OPT_ARGS_DISPLAY].get_string() == "none" && !args[OPT_ARGS_DEBUG].existed()) {
			if (diffnum == 1) {
				cout << "文件不同." << endl;
				break;
			}
		}

		if (fin1.eof() || fin2.eof())
			break;
	}

	if (args[OPT_ARGS_DISPLAY].get_string() == "none" && !args[OPT_ARGS_DEBUG].existed()) {
		if (diffnum == 0)
			cout << "文件相同." << endl;
	}
	else {
		if (diffnum == 0) {
			cout << "在指定检查条件下完全一致." << endl;
			cout << "====================================================================================================" << endl;
		}
		else {
			cout << "====================================================================================================" << endl;
			cout << "在指定检查条件下共" << diffnum << "行有差异." << endl;
			cout << "阅读提示：" << endl;
			cout << "	1、空行用<EMPTY>标出" << endl;
			cout << "	2、文件结束用<EOF>标出" << endl;
			cout << "	3、两行相同列位置的差异字符用亮色标出" << endl;
			cout << "	4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)" << endl;
			cout << "	5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)" << endl;
			cout << "	6、每行最后用<CR>标出(方便看清隐含字符)" << endl;
			cout << "	7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出" << endl;
			if (args[OPT_ARGS_DISPLAY].get_string() != "detailed")
				cout << "	8、用--display detailed可以得到更详细的信息" << endl;
			cout<< "====================================================================================================" << endl;
		}
	}

	fin1.close(), fin2.close();
	return 0;
}