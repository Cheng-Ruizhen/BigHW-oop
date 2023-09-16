/* 2152046 程瑞真 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//可根据需要再加入其它需要的头文件
#include <iomanip>
#include "../include/class_aat.h"
using namespace std;

/* 运行自行添加其它函数，也可以将自定义函数放在其它cpp中 */
int usage(const char* const procname, const args_analyse_tools* const args,const char* func[])
{
	const int width = 7 + strlen(procname) + 1;

	cout << "Usage: " << procname;
	for (int i = 0; args[i].get_name() != ""; i++) {
		cout << " [" << args[i].get_name();
		if (func[i] != NULL)
			cout << " " << func[i];
		cout << ']';
	}
	cout << " IP地址" << endl;
	args_analyse_print(args);

	return 0;
}


bool check_IP(const char* ip)
{
	int len = strlen(ip), count = 0;
	int temp = 0;
	for (int i = 0; i < len; i++) {
		if (ip[i] == '.') {
			if (i == 0 || i == len - 1 || (ip[i + 1] < '0' || ip[i + 1]>'9') || temp >= 256)
				return false;
			count++;
			temp = 0;
		}
		else if (ip[i] < '0' || ip[i]>'9')
			return false;
		else
			temp = temp * 10 + ip[i] - '0';
	}
	if (temp >= 256)
		return false;
	if (count != 3)
		return false;
	else
		return true;
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char* argv[])
{
	args_analyse_tools args[] = {
		args_analyse_tools("--l",ST_EXTARGS_TYPE::int_with_default,1, 64, 32, 64000),
		args_analyse_tools("--n",ST_EXTARGS_TYPE::int_with_default,1, 4, 1, 1024),
		args_analyse_tools("--t",ST_EXTARGS_TYPE::boolean,         0, false),	
		args_analyse_tools()  //最后一个，用于结束
	};
	const char* func[] = {
		"大小",
		"数量",
		NULL
	};

	if (argc == 1) {
		usage(argv[0], args, func);
		return -1;
	}

	// 先检查IP地址
	if (!check_IP(argv[argc - 1])) {
		cout << "IP地址错误" << endl;
		return -1;
	}

	int cur_argc;

	if ((cur_argc = args_analyse_process(argc - 1, argv, args, 1)) < 0)
	{
		//错误信息在函数中已打印
		return -1;
	}
	else if ((argc - cur_argc) != 1) { // 固定参数有且仅有一个
		if (argv[cur_argc][0] == '-' && argv[cur_argc][1] == '-')
			cout << "参数[" << argv[cur_argc] << "]不存在" << endl;
		else
			cout << "参数[" << argv[cur_argc] << "]不是以--开头的合法参数" << endl;
		return -1;
	}
	else
	{
		cout << "参数检查通过" << endl;
		for (int i = 0; args[i].get_name() != ""; i++) {
			cout << args[i].get_name() << " 参数：";
			if (args[i].get_name() == "--t")
				cout << args[i].existed() << endl;
			else
				cout << args[i].get_int() << endl;
		}
		cout << "IP地址：" << argv[argc - 1] << endl;
	}

	return 0;
}