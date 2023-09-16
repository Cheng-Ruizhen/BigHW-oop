/* 2152046 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_AAT_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */
// �ж��Ƿ�Ϊ��������ʽ
static bool is_para(const char* name)
{
	return (strlen(name) >= 2 && name[0] == '-' && name[1] == '-');
}

// �ж��Ƿ�Ϊ����
static bool is_int(const char* str)
{
	unsigned i = (str[0] == '-');
	for (; i < strlen(str); i++)
		if (str[i] < '0' || str[i]>'9')
			return false;
	return true;
}

// �жϸ���int���Ӳ����Ƿ��ڹ涨��set��
static bool in_int_set(int num, int* set)
{
	int pos = 0;
	while (1) {
		if (set[pos] == num || set[pos] == INVALID_INT_VALUE_OF_SET)
			break;
		pos++;
	}
	return !(set[pos] == INVALID_INT_VALUE_OF_SET);
}

// �жϸ���string���Ӳ����Ƿ��ڹ涨��set��
static bool in_string_set(string str, string* set)
{
	int pos = 0;
	while (1) {
		if (set[pos] == str || set[pos] == "")
			break;
		pos++;
	}
	return !(set[pos] == "");
}

// �ж�IP��ʽ�Ƿ���ȷ����IP��ʽ��ȷ��ͨ�����÷�ʽ����u_int32��ʽ
static bool is_IP(const char* ip,unsigned& ans)
{
	int len = strlen(ip), count = 0;
	int temp = 0;
	for (int i = 0; i < len; i++) {
		if (ip[i] == '.') {
			if (i == 0 || i == len - 1 || (ip[i + 1] < '0' || ip[i + 1]>'9') || temp >= 256)
				return false;
			count++;
			ans += (temp << (4 - count) * 8);
			temp = 0;
		}
		else if (ip[i] < '0' || ip[i]>'9')
			return false;
		else
			temp = temp * 10 + ip[i] - '0';
	}
	if (temp >= 256)
		return false;
	ans += temp;
	if (count != 3)
		return false;
	else
		return true;
}

// ��u_int32��ʽ��IPת��string
static string IntSwitchTOString(unsigned IP_value)
{
	int addr1 = (IP_value & 0xff000000) >> 24;
	int addr2 = (IP_value & 0x00ff0000) >> 16;
	int addr3 = (IP_value & 0x0000ff00) >> 8;
	int addr4 = (IP_value & 0x000000ff);
	string ipaddr = to_string(addr1) + '.' + to_string(addr2) + '.' + to_string(addr3) + '.' + to_string(addr4);
	return ipaddr; 
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	args_name = "";
	extargs_type = ST_EXTARGS_TYPE::null;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_bool_default = def;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_int_default = def;
	extargs_int_value = def;
	extargs_int_min = _min;
	extargs_int_max = _max;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_int_set_num = 0;
	while (set[extargs_int_set_num] != INVALID_INT_VALUE_OF_SET)
		extargs_int_set_num++;
	if (def_of_set_pos >= extargs_int_set_num)
		extargs_int_default = 0;
	else
		extargs_int_default = set[def_of_set_pos];
	extargs_int_set = new(nothrow) int[extargs_int_set_num + 1];
	if (extargs_int_set)
		for (int i = 0; i <= extargs_int_set_num; i++)
			extargs_int_set[i] = set[i];
	else
		extargs_int_set_num = 0;
	extargs_int_value = extargs_int_default;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	if (type == ST_EXTARGS_TYPE::str){
		extargs_string_default = def;
		extargs_string_value = def;
	}
	else {
		unsigned IP = 0;
		is_IP(def.c_str(), IP);
		extargs_ipaddr_default = IP;
		extargs_ipaddr_value = IP;
	}
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_string_set_num = 0;
	while (set[extargs_string_set_num].compare("") != 0)
		extargs_string_set_num++;
	if (def_of_set_pos >= extargs_string_set_num)
		extargs_string_default = "0";
	else
		extargs_string_default = set[def_of_set_pos];
	extargs_string_set = new(nothrow) string[extargs_string_set_num + 1];
	if (extargs_string_set)
		for (int i = 0; i <= extargs_string_set_num; i++)
			extargs_string_set[i] = set[i];
	else
		extargs_string_set_num = 0;
	extargs_string_value = extargs_string_default;
	args_existed = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
	if (extargs_type == ST_EXTARGS_TYPE::int_with_set_default || extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
		if (extargs_int_set)
			delete[] extargs_int_set;
	if (extargs_type == ST_EXTARGS_TYPE::str_with_set_default || extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
		if (extargs_string_set)
			delete[] extargs_string_set;
}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return IntSwitchTOString(extargs_ipaddr_value); //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			./log_login --limit 10 --user root                    ���޺�����������0
			./ulimits_check --memory 256 --cpu 1 [./log_login ***]���к�����������1
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	for (int i = 1; i < argc; i++) {
		int pos = 0;
		while (args[pos].get_name().compare(argv[i]) != 0 && args[pos].get_name().compare("") != 0)
			pos++;  
		if (args[pos].get_name().compare("") == 0) // û���ҵ���Ӧ����
		{
			if (follow_up_args == 1 && !(argv[i][0] == '-' && argv[i][1] == '-')) // �к����������Ҹò�����Ϊ--��ͷ���ò�����֮��Ĳ��������̶�����
				return i;
			else {
				if (argv[i][0] == '-' && argv[i][1] == '-')
					cout << "����[" << argv[i] << "]�Ƿ�." << endl;
				else
					cout << "����[" << argv[i] << "]��ʽ�Ƿ�." << endl;
				return -1;
			}
		}
		else
		{
			if (args[pos].args_existed == 1) { // �����ظ�����
				cout << "����[" << args[pos].args_name << "]�ظ�." << endl;
				return -1;
			}
			args[pos].args_existed = 1;
			for (int j = 0; j < args[pos].extargs_num; j++)
			{
				i++; // ÿ��һ�����������argv�������һλ
				unsigned IP = 0;
				switch (args[pos].extargs_type)
				{
				case ST_EXTARGS_TYPE::int_with_default:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					args[pos].extargs_int_value = (atoi(argv[i]) >= args[pos].extargs_int_min && atoi(argv[i]) <= args[pos].extargs_int_max) ? atoi(argv[i]) : args[pos].extargs_int_default;
					break;
				case ST_EXTARGS_TYPE::int_with_error:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (atoi(argv[i]) < args[pos].extargs_int_min || atoi(argv[i]) > args[pos].extargs_int_max) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:int, ��Χ[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					else
						args[pos].extargs_int_value = atoi(argv[i]);
					break;
				case ST_EXTARGS_TYPE::int_with_set_default:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ�����������. (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] ȱʡ:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!in_int_set(atoi(argv[i]), args[pos].extargs_int_set))
						args[pos].extargs_int_value = args[pos].extargs_int_default;
					else
						args[pos].extargs_int_value = atoi(argv[i]);
					break;
				case ST_EXTARGS_TYPE::int_with_set_error:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ�����������. (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (!in_int_set(atoi(argv[i]), args[pos].extargs_int_set)) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:int, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					else
						args[pos].extargs_int_value = atoi(argv[i]);
					break;
				case ST_EXTARGS_TYPE::str:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:string";
						if (args[pos].extargs_string_default.compare("") != 0)
							cout << " ȱʡ:" << args[pos].extargs_string_default;
						cout << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:string";
						if (args[pos].extargs_string_default.compare("") != 0)
							cout << " ȱʡ:" << args[pos].extargs_string_default;
						cout << ")" << endl;
						return -1;
					}
					args[pos].extargs_string_value = argv[i];
					break;
				case ST_EXTARGS_TYPE::str_with_set_default:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:string, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "] ȱʡ:" << args[pos].extargs_string_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:string, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "] ȱʡ:" << args[pos].extargs_string_default << ")" << endl;
						return -1;
					}
					if (!in_string_set(argv[i], args[pos].extargs_string_set))
						args[pos].extargs_string_value = args[pos].extargs_string_default;
					else
						args[pos].extargs_string_value = argv[i];
					break;
				case ST_EXTARGS_TYPE::str_with_set_error:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:string, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:string, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "])" << endl;
						return -1;
					}
					if (!in_string_set(argv[i], args[pos].extargs_string_set)) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:string, ��ȡֵ[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "])" << endl;
						return -1;
					}
					else
						args[pos].extargs_string_value = argv[i];
					break;
				case ST_EXTARGS_TYPE::ipaddr_with_default:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:IP��ַ ȱʡ:" << args[pos].extargs_ipaddr_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:IP��ַ ȱʡ:" << args[pos].extargs_ipaddr_default << ")" << endl;
						return -1;
					}
					IP = 0;
					if (!is_IP(argv[i], IP))
						args[pos].extargs_ipaddr_value = args[pos].extargs_ipaddr_default;
					else
						args[pos].extargs_ipaddr_value = IP;
					break;
				case ST_EXTARGS_TYPE::ipaddr_with_error:
					if (i == argc) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ������� (����:IP��ַ)" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "����[" << args[pos].args_name << "]ȱ�ٸ��Ӳ���. (����:IP��ַ)" << endl;
						return -1;
					}
					IP = 0;
					if (!is_IP(argv[i], IP)) {
						cout << "����[" << args[pos].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:IP��ַ)" << endl;
						return -1;
					}
					else
						args[pos].extargs_ipaddr_value = IP;
					break;
				}// end of switch
			}// end of for
		}
	}
	return argc; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	int pos = 0;
	unsigned name_len = 4, def_len = 16, val_len = 16, rs_len = 9;
	string temp;
	while (args[pos].args_name.compare("") != 0){
		name_len = (name_len > args[pos].args_name.size()) ? name_len : args[pos].args_name.size();

		if (args[pos].extargs_type == ST_EXTARGS_TYPE::str || args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
			def_len = def_len > args[pos].extargs_string_default.size() ? def_len : args[pos].extargs_string_default.size();
		else if (args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
			def_len = max((int)(def_len), (int)(args[pos].get_str_ipaddr().size()));

		temp.clear();
		if (args[pos].args_existed) {
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::str || args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				val_len = max((int)(val_len), (int)(args[pos].extargs_string_value.size()));
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default || args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				val_len = max((int)(val_len), (int)(args[pos].get_str_ipaddr().size()));
		}

		temp.clear();
		if (args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_error)
			temp = '[' + to_string(args[pos].extargs_int_min) + ".." + to_string(args[pos].extargs_int_max) + ']';
		else if (args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
			for (int i = 0; i < args[pos].extargs_int_set_num; i++) {
				temp += to_string(args[pos].extargs_int_set[i]);
				if (i != args[pos].extargs_int_set_num - 1)
					temp += '/';
			}
		}
		else if (args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
			for (int i = 0; i < args[pos].extargs_string_set_num; i++) {
				temp += args[pos].extargs_string_set[i];
				if (i != args[pos].extargs_string_set_num - 1)
					temp += '/';
			}
		}
		rs_len = (rs_len > temp.size()) ? rs_len : temp.size();
		pos++;
	}

	int len = name_len + 21 + def_len + 6 + val_len + rs_len + 6;
	cout << setiosflags(ios::left);
	cout << setw(len) << setfill('=') << "" << endl;
	cout << setfill(' ') << setw(name_len + 1) << "name" << setw(22) << "type" << setw(def_len + 1) << "default" << setw(7) << "exists" << setw(val_len + 1) << "value" << "range/set" << endl;
	cout << setw(len) << setfill('=') << "" << endl;
	cout << setfill(' ');
	pos = 0;
	while (args[pos].args_name.compare("") != 0) {
		cout << setw(name_len + 1) << args[pos].args_name;
		switch (args[pos].extargs_type) {
		case ST_EXTARGS_TYPE::boolean:
			cout << setw(22) << "Bool";
			if (args[pos].extargs_bool_default)
				cout << setw(def_len + 1) << "true";
			else
				cout << setw(def_len + 1) << "false";
			cout << setw(7) << args[pos].args_existed;
			if (args[pos].args_existed)
				cout << setw(val_len + 1) << "true";
			else
				cout << setw(val_len + 1) << "/";
			cout << "/" << endl;
			break;
		case ST_EXTARGS_TYPE::int_with_default:
		case ST_EXTARGS_TYPE::int_with_error:
		case ST_EXTARGS_TYPE::int_with_set_default:
		case ST_EXTARGS_TYPE::int_with_set_error:
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_default)
				cout << setw(22) << "IntWithDefault";
			else if(args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_error)
				cout << setw(22) << "IntWithError";
			else if(args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
				cout << setw(22) << "IntSETWithDefault";
			else
				cout << setw(22) << "IntSETWithError";
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
				cout << setw(def_len + 1) << args[pos].extargs_int_default;
			else
				cout << setw(def_len + 1) << "/";
			cout << setw(7) << args[pos].args_existed;
			if (args[pos].args_existed)
				cout << setw(val_len + 1) << args[pos].extargs_int_value;
			else
				cout << setw(val_len + 1) << "/";
			temp.clear();
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[pos].extargs_type == ST_EXTARGS_TYPE::int_with_error)
				temp = temp + '[' + to_string(args[pos].extargs_int_min) + ".." + to_string(args[pos].extargs_int_max) + ']';
			else {
				for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
					temp += to_string(args[pos].extargs_int_set[k]);
					if (k != args[pos].extargs_int_set_num - 1)
						temp += '/';
				}
			}
			cout << temp << endl;
			break;
		default:
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::str)
				cout << setw(22) << "String";
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
				cout << setw(22) << "StringSETWithDefault";
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				cout << setw(22) << "StringSETWithError";
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
				cout << setw(22) << "IPAddrWithDefault";
			else
				cout << setw(22) << "IPAddrWithError";
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::str) {
				if (args[pos].extargs_string_default.compare("") == 0)
					cout << setw(def_len + 1) << '/';
				else
					cout << setw(def_len + 1) << args[pos].extargs_string_default;
			}
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
				cout << setw(def_len + 1) << args[pos].extargs_string_default;
			else if (args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
				cout << setw(def_len + 1) << IntSwitchTOString(args[pos].extargs_ipaddr_default);
			else
				cout << setw(def_len + 1) << '/';
			cout << setw(7) << args[pos].args_existed;
			if (args[pos].args_existed) {
				if (args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default || args[pos].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
					cout << setw(val_len + 1) << args[pos].get_str_ipaddr();
				else
					cout << setw(val_len + 1) << args[pos].extargs_string_value;
			}
			else
				cout << setw(val_len + 1) << '/';
			if (args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[pos].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
				temp.clear();
				for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
					temp += args[pos].extargs_string_set[k];
					if (k != args[pos].extargs_string_set_num - 1)
						temp += '/';
				}
				cout << temp << endl;
			}
			else
				cout << '/' << endl;
			break;
		}//end of switch
		pos++;
	}//end of while
	cout << setw(len) << setfill('=') << "" << endl;
	cout << setfill(' ') << resetiosflags(ios::left);
	return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_AAT_TOOLS
