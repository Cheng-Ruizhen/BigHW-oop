/* 2152046 程瑞真 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_AAT_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
// 判断是否为参数名形式
static bool is_para(const char* name)
{
	return (strlen(name) >= 2 && name[0] == '-' && name[1] == '-');
}

// 判断是否为整数
static bool is_int(const char* str)
{
	unsigned i = (str[0] == '-');
	for (; i < strlen(str); i++)
		if (str[i] < '0' || str[i]>'9')
			return false;
	return true;
}

// 判断给定int附加参数是否在规定的set内
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

// 判断给定string附加参数是否在规定的set内
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

// 判断IP格式是否正确；若IP格式正确，通过引用方式返回u_int32形式
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

// 将u_int32形式的IP转成string
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	args_name = "";
	extargs_type = ST_EXTARGS_TYPE::null;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
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
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return IntSwitchTOString(extargs_ipaddr_value); //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
			./log_login --limit 10 --user root                    ：无后续参数，置0
			./ulimits_check --memory 256 --cpu 1 [./log_login ***]：有后续参数，置1
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	for (int i = 1; i < argc; i++) {
		int pos = 0;
		while (args[pos].get_name().compare(argv[i]) != 0 && args[pos].get_name().compare("") != 0)
			pos++;  
		if (args[pos].get_name().compare("") == 0) // 没有找到相应参数
		{
			if (follow_up_args == 1 && !(argv[i][0] == '-' && argv[i][1] == '-')) // 有后续参数，且该参数不为--开头，该参数及之后的参数当作固定参数
				return i;
			else {
				if (argv[i][0] == '-' && argv[i][1] == '-')
					cout << "参数[" << argv[i] << "]非法." << endl;
				else
					cout << "参数[" << argv[i] << "]格式非法." << endl;
				return -1;
			}
		}
		else
		{
			if (args[pos].args_existed == 1) { // 参数重复出现
				cout << "参数[" << args[pos].args_name << "]重复." << endl;
				return -1;
			}
			args[pos].args_existed = 1;
			for (int j = 0; j < args[pos].extargs_num; j++)
			{
				i++; // 每有一个额外参数，argv就往后读一位
				unsigned IP = 0;
				switch (args[pos].extargs_type)
				{
				case ST_EXTARGS_TYPE::int_with_default:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不是整数. (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					args[pos].extargs_int_value = (atoi(argv[i]) >= args[pos].extargs_int_min && atoi(argv[i]) <= args[pos].extargs_int_max) ? atoi(argv[i]) : args[pos].extargs_int_default;
					break;
				case ST_EXTARGS_TYPE::int_with_error:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不是整数. (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					if (atoi(argv[i]) < args[pos].extargs_int_min || atoi(argv[i]) > args[pos].extargs_int_max) {
						cout << "参数[" << args[pos].args_name << "]的附加参数值(" << argv[i] << ")非法. (类型:int, 范围[" << args[pos].extargs_int_min << ".." << args[pos].extargs_int_max << "])" << endl;
						return -1;
					}
					else
						args[pos].extargs_int_value = atoi(argv[i]);
					break;
				case ST_EXTARGS_TYPE::int_with_set_default:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不是整数. (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "] 缺省:" << args[pos].extargs_int_default << ")" << endl;
						return -1;
					}
					if (!in_int_set(atoi(argv[i]), args[pos].extargs_int_set))
						args[pos].extargs_int_value = args[pos].extargs_int_default;
					else
						args[pos].extargs_int_value = atoi(argv[i]);
					break;
				case ST_EXTARGS_TYPE::int_with_set_error:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (!is_int(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不是整数. (类型:int, 可取值[";
						for (int k = 0; k < args[pos].extargs_int_set_num; k++) {
							cout << args[pos].extargs_int_set[k];
							if (k != args[pos].extargs_int_set_num - 1)
								cout << "/";
						}
						cout << "])" << endl;
						return -1;
					}
					if (!in_int_set(atoi(argv[i]), args[pos].extargs_int_set)) {
						cout << "参数[" << args[pos].args_name << "]的附加参数值(" << argv[i] << ")非法. (类型:int, 可取值[";
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
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:string";
						if (args[pos].extargs_string_default.compare("") != 0)
							cout << " 缺省:" << args[pos].extargs_string_default;
						cout << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:string";
						if (args[pos].extargs_string_default.compare("") != 0)
							cout << " 缺省:" << args[pos].extargs_string_default;
						cout << ")" << endl;
						return -1;
					}
					args[pos].extargs_string_value = argv[i];
					break;
				case ST_EXTARGS_TYPE::str_with_set_default:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:string, 可取值[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "] 缺省:" << args[pos].extargs_string_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:string, 可取值[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "] 缺省:" << args[pos].extargs_string_default << ")" << endl;
						return -1;
					}
					if (!in_string_set(argv[i], args[pos].extargs_string_set))
						args[pos].extargs_string_value = args[pos].extargs_string_default;
					else
						args[pos].extargs_string_value = argv[i];
					break;
				case ST_EXTARGS_TYPE::str_with_set_error:
					if (i == argc) {
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:string, 可取值[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "])" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:string, 可取值[";
						for (int k = 0; k < args[pos].extargs_string_set_num; k++) {
							cout << args[pos].extargs_string_set[k];
							if (k != args[pos].extargs_string_set_num - 1)
								cout << '/';
						}
						cout << "])" << endl;
						return -1;
					}
					if (!in_string_set(argv[i], args[pos].extargs_string_set)) {
						cout << "参数[" << args[pos].args_name << "]的附加参数值(" << argv[i] << ")非法. (类型:string, 可取值[";
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
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:IP地址 缺省:" << args[pos].extargs_ipaddr_default << ")" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:IP地址 缺省:" << args[pos].extargs_ipaddr_default << ")" << endl;
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
						cout << "参数[" << args[pos].args_name << "]的附加参数不足 (类型:IP地址)" << endl;
						return -1;
					}
					if (is_para(argv[i])) {
						cout << "参数[" << args[pos].args_name << "]缺少附加参数. (类型:IP地址)" << endl;
						return -1;
					}
					IP = 0;
					if (!is_IP(argv[i], IP)) {
						cout << "参数[" << args[pos].args_name << "]的附加参数值(" << argv[i] << ")非法. (类型:IP地址)" << endl;
						return -1;
					}
					else
						args[pos].extargs_ipaddr_value = IP;
					break;
				}// end of switch
			}// end of for
		}
	}
	return argc; //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
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
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_AAT_TOOLS
