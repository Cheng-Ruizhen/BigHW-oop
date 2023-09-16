/* 2152046 ������ ������ */
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
  �������ƣ�is_nullstring
  ��    �ܣ��ж��Ƿ�Ϊ����
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�IgnoreSpaces
  ��    �ܣ�
  ���������str      : ����ÿ���ַ���
            TrimType : �ո�ıȽϷ�ʽ
  �� �� ֵ�����Ե���ѡ�ո����ַ���
  ˵    ����
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
  �������ƣ�FirstTimeDiff
  ��    �ܣ����������ַ����״β�ͬ���±�
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�display_normal
  ��    �ܣ�������ʽ���ÿһ�еĲ�ͬ
  ���������string str1,str2  : ���ļ������ַ���(��Ϊeof��Ҳ���ɿմ�)
			int    eof1,eof2  : ���ļ��Ƿ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void display_normal(string str1, string str2, int eof1, int eof2)
{
	int len1 = str1.size(), len2 = str2.size();

	cout << "�ļ�1 : ";
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
	cout << "�ļ�2 : ";
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
  �������ƣ�display_detailed
  ��    �ܣ���ϸ��ʽ���ÿһ�еĲ�ͬ
  ���������string str1,str2  : ���ļ������ַ���(��Ϊeof��Ҳ���ɿմ�)
			int    eof1,eof2  : ���ļ��Ƿ����
  �� �� ֵ��
  ˵    ����
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
	cout << "�ļ�1(HEX) : " << endl;
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

	cout << "�ļ�2(HEX) : " << endl;
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
  �������ƣ�display
  ��    �ܣ���������������ʾ����
  ���������int    row1,row2  : ��ʱ��ȡ������
			string str1,str2  : ���ļ������ַ���(��Ϊeof��Ҳ���ɿմ�)
			int    eof1,eof2  : ���ļ��Ƿ����
			string DisplayType: ��ʾ�ķ�ʽ��none/normal/detailed��
			bool   debug      : �Ƿ���Ҫdebug��ʽ���
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int display(int row1, int row2, string str1, string str2, int eof1, int eof2, string DisplayType, bool debug)
{
	if (eof1 == 1 && eof2 == 1)
		return 0;

	int len1 = str1.size(), len2 = str2.size();
	int diffpos = FirstTimeDiff(str1, str2, eof1, eof2);

	if (diffpos == len1 && diffpos == len2 ) // �������
	{
		if (debug) {
			if (len1 != 0)
				cout << "��[" << row1 << " / " << row2 << "] - һ�£�" << str1 << "<CR>" << endl << endl;
			else
				cout << "��[" << row1 << " / " << row2 << "] - һ�£�<EMPTY><CR>" << endl << endl;
		}
		return 0;
	}

	if (DisplayType == "none" && !debug)
		return 1;

	cout << "��[" << row1 << " / " << row2 << "]�� - ";
	if (eof1 == 1)
		cout << "�ļ�1�ѽ���/�ļ�2�������ݣ�" << endl;
	else if (eof2 == 1)
		cout << "�ļ�1��������/�ļ�2�ѽ�����" << endl;
	else if (diffpos == len2)
		cout << "�ļ�1��β���ж����ַ���" << endl;
	else if (diffpos == len1)
		cout << "�ļ�2��β���ж����ַ���" << endl;
	else
		cout << "��[" << diffpos << "]���ַ���ʼ�в��죺" << endl;

	if (DisplayType == "normal" || (DisplayType != "detailed" && debug))
		display_normal(str1, str2, eof1, eof2);
	else if (DisplayType == "detailed")
		display_detailed(str1, str2, eof1, eof2);
	cout << endl;
	return 1;
}

/***************************************************************************
  �������ƣ�basic_information
  ��    �ܣ�debugģʽ�»�����Ϣ�����
  ���������
  �� �� ֵ��
  ˵    ����
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
	int format1 = 0, format2 = 0; // �ļ���ʽ��0ΪWindows��1ΪLinux
	fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	if (!fin1.is_open()) {
		cout << "��1���ļ�[" << args[OPT_ARGS_FILE1].get_string() << "]�޷���." << endl;
		return -1;
	}
	fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!fin2.is_open()) {
		cout << "��2���ļ�[" << args[OPT_ARGS_FILE2].get_string() << "]�޷���." << endl;
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

	cout << "��1���ļ��Ļ�����Ϣ��" << endl;
	cout << "====================================================================================================" << endl;
	cout << "�� �� ����" << args[OPT_ARGS_FILE1].get_string() << endl;
	cout << "��    С��" << memory1 - 1 << endl;
	cout << "��    ����" << lines1 - 1 << endl;
	cout << "����г���" << maxline1 + args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() << endl;
	cout << "�ļ���ʽ��";
	if (format1 == 0)
		cout << "Windows" << endl;
	else
		cout << "Linux" << endl;
	cout << "====================================================================================================" << endl << endl;

	cout << "��2���ļ��Ļ�����Ϣ��" << endl;
	cout << "====================================================================================================" << endl;
	cout << "�� �� ����" << args[OPT_ARGS_FILE2].get_string() << endl;
	cout << "��    С��" << memory2 - 1 << endl;
	cout << "��    ����" << lines2 - 1 << endl;
	cout << "����г���" << maxline2 + args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() << endl;
	cout << "�ļ���ʽ��";
	if (format2 == 0)
		cout << "Windows" << endl;
	else
		cout << "Linux" << endl;
	cout << "====================================================================================================" << endl << endl;

	int row1 = 1, row2 = 1;
	if (args[OPT_ARGS_LINEOFFSET].existed()) {
		if (args[OPT_ARGS_LINEOFFSET].get_int() < 0) {
			cout << "[" << args[OPT_ARGS_FILE1].get_string() << "] ���� [" << -args[OPT_ARGS_LINEOFFSET].get_int() << " ]��" << endl;
			fin1.close();
			fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
			for (int i = 0; i < -args[OPT_ARGS_LINEOFFSET].get_int(); i++) {
				temp.clear();
				if (!fin1.eof()) {
					getline(fin1, temp, '\n');
					temp = IgnoreSpaces(temp, args[OPT_ARGS_TRIM].get_string());
					if (args[OPT_ARGS_IGNORE_BLANK].existed() && is_nullstring(temp)) {
						cout << "��[* / " << row1 << "] - Ϊ�գ�����." << endl << endl;
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
			cout << "[" << args[OPT_ARGS_FILE2].get_string() << "] ���� [" << args[OPT_ARGS_LINEOFFSET].get_int() << " ]��" << endl;
			fin2.close();
			fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
			for (int i = 0; i < args[OPT_ARGS_LINEOFFSET].get_int(); i++) {
				temp.clear();
				if (!fin2.eof()) {
					getline(fin2, temp, '\n');
					temp = IgnoreSpaces(temp, args[OPT_ARGS_TRIM].get_string());
					if (args[OPT_ARGS_IGNORE_BLANK].existed() && is_nullstring(temp)) {
						cout << "��[* / " << row2 << "] - Ϊ�գ�����." << endl << endl;
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
  �������ƣ�analyse
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int analyse(args_analyse_tools* const args)
{
	ifstream fin1, fin2;
	fin1.open(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	if (!fin1.is_open()) {
		cout << "��1���ļ�[" << args[OPT_ARGS_FILE1].get_string() << "]�޷���." << endl;
		return -1;
	}
	fin2.open(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!fin2.is_open()) {
		cout << "��2���ļ�[" << args[OPT_ARGS_FILE2].get_string() << "]�޷���." << endl;
		fin1.close();
		return -1;
	}

	int row1 = 1, row2 = 1; // ���ļ�������

	if (args[OPT_ARGS_LINEOFFSET].existed()) { // ͳһ�в�
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
		cout << "�ȽϽ�������" << endl;
		cout << "====================================================================================================" << endl;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		cout << "��ʼ�У�" << row1 - 1 << " / " << row2 - 1 << endl;

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
		/* �����һ��û�س����Һ��Կ��У��������� */
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
					cout << "��[" << row1 << " / *] - Ϊ�գ�����." << endl << endl;
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
					cout << "��[* / " << row2 << "] - Ϊ�գ�����." << endl << endl;
				getline(fin2, str2, '\n');
				if (!args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed())
					if (str2.size() > 0 && str2[str2.size() - 1] == '\r')
						str2.pop_back();
				str2 = IgnoreSpaces(str2, args[OPT_ARGS_TRIM].get_string());
				row2++;
			}
		}

		/* normal/detailed���ڸú�������� */
		diffnum += display(row1++, row2++, str1, str2, eof1, eof2, args[OPT_ARGS_DISPLAY].get_string(), args[OPT_ARGS_DEBUG].existed());

		/* ��ͬ�г���������� */
		if (args[OPT_ARGS_MAX_DIFF].existed()) {
			if (args[OPT_ARGS_MAX_DIFF].get_int() != 0 && diffnum > args[OPT_ARGS_MAX_DIFF].get_int())
				break;
		}

		/* ������������������ */
		if (args[OPT_ARGS_MAX_LINE].existed()) {
			if (args[OPT_ARGS_MAX_LINE].get_int() != 0 && count > args[OPT_ARGS_MAX_LINE].get_int())
				break;
		}

		/* display�������Ϊnoneʱ���ҵ�һ����ͬ������ֱ���˳� */
		if (args[OPT_ARGS_DISPLAY].get_string() == "none" && !args[OPT_ARGS_DEBUG].existed()) {
			if (diffnum == 1) {
				cout << "�ļ���ͬ." << endl;
				break;
			}
		}

		if (fin1.eof() || fin2.eof())
			break;
	}

	if (args[OPT_ARGS_DISPLAY].get_string() == "none" && !args[OPT_ARGS_DEBUG].existed()) {
		if (diffnum == 0)
			cout << "�ļ���ͬ." << endl;
	}
	else {
		if (diffnum == 0) {
			cout << "��ָ�������������ȫһ��." << endl;
			cout << "====================================================================================================" << endl;
		}
		else {
			cout << "====================================================================================================" << endl;
			cout << "��ָ����������¹�" << diffnum << "���в���." << endl;
			cout << "�Ķ���ʾ��" << endl;
			cout << "	1��������<EMPTY>���" << endl;
			cout << "	2���ļ�������<EOF>���" << endl;
			cout << "	3��������ͬ��λ�õĲ����ַ�����ɫ���" << endl;
			cout << "	4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)" << endl;
			cout << "	5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)" << endl;
			cout << "	6��ÿ�������<CR>���(���㿴�������ַ�)" << endl;
			cout << "	7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���" << endl;
			if (args[OPT_ARGS_DISPLAY].get_string() != "detailed")
				cout << "	8����--display detailed���Եõ�����ϸ����Ϣ" << endl;
			cout<< "====================================================================================================" << endl;
		}
	}

	fin1.close(), fin2.close();
	return 0;
}