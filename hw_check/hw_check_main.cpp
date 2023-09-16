/* 2152046 程瑞真 大数据 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "../include/class_aat.h"
#include "hw_check.h"

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：具体略，实际使用时，放入另一个cpp(例：hw_check_secondline.cpp)更合理
 ***************************************************************************/
int check_secondline(const string& cno, const string &stulist_fname, const string& filename, const string &debug)
{
	cout << "开始进行源程序文件次行检查..." << endl;//本次作业要求实现，本函数要改动
	ostringstream msg;

	/* 允许加入其它语句，但不能有cout */
	int stunum = 0, max_student = 1010;
	student* stu = new(nothrow) student[max_student];
	if (stu == NULL)
		return -1;

	/* 读课号.dat */
	msg << "1.读取课号配置文件" << endl;
	int status = read_stulist(msg, stulist_fname, stu, stunum); //允许带返回值（也可以不要），参数自行约定，其中有一个是msg 
	/* 允许加入其它语句(也可以不要)，但不能有cout */
	if (debug[0] == '1')
		cout << msg.str() << endl;

	/* 允许加入其它语句，但不能有cout */
	if (status == -1) //无法打开文件，无需进行后续操作
		return 0;

	msg.str(""); //清空msg中的信息
	msg << "2.按课号+学号排序的学生名单" << endl;
	print_stulist(msg, stu, stunum); //允许带返回值（也可以不要），参数自行约定，其中有一个是msg 
	/* 允许加入其它语句(也可以不要)，但不能有cout */
	if (debug[1] == '1')
		cout << msg.str() << endl;

	/* 允许加入其它语句，但不能有cout */

	msg.str(""); //清空msg中的信息
	msg << "3.所有学生读取第2行的解析信息" << endl;
	read_2nd_line(msg, cno, filename, stu, stunum); //允许带返回值（也可以不要），参数自行约定，其中有一个是msg 
	/* 允许加入其它语句(也可以不要)，但不能有cout */
	if (debug[2] == '1')
		cout << msg.str() << endl;

	/* 允许加入其它语句，但不能有cout */

	/* 将双方进行信息匹配 */
	msg.str(""); //清空msg中的信息
	msg << "4.所有学生的第2行互验匹配结果" << endl;
	print_2nd_line(msg, stu, stunum); //允许带返回值（也可以不要），参数自行约定，其中有一个是msg 
	/* 允许加入其它语句(也可以不要)，但不能有cout */
	if (debug[3] == '1')
		cout << msg.str() << endl;

	/* 允许加入其它语句，但不能有cout */

	/* 再根据通过与否进行检查 */
	msg.str(""); //清空msg中的信息
	msg << "5.所有学生的第2行互验最终得分" << endl;
	print_finalscore(msg, stu, stunum); //允许带返回值（也可以不要），参数自行约定，其中有一个是msg 
	/* 允许加入其它语句(也可以不要)，但不能有cout */
	if (debug[4] == '1')
		cout << msg.str() << endl;

	/* 允许加入其它语句，但不能有cout */
	delete[] stu;
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：本次作业不要求实现，本函数不要改动
 ***************************************************************************/
int check_firstline(const string& cno, const string &stilist_fname, const string& filename)
{
	cout << "开始进行源程序文件首行检查..." << endl;
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const procname, const int args_num)
{
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;

	cout << endl;
	cout << "Usage: " << procname << " { --firstline | --secondline  }" << endl;
	cout << setw(wkey) << ' ' << "{ --cno xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --filename xxx/all }" << endl; //约定不允许实际文件名为all
	cout << setw(wkey) << ' ' << "{ --stulist_fname xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --debug 11111 }" << endl;
	cout << endl;

	cout << setw(wkey) << ' ' << "必选项：指定检查类型(多选一)" << endl;
	cout << setw(wopt) << ' ' << "--firstline     : 首行检查" << endl;
	cout << setw(wopt) << ' ' << "--secondline    : 次行检查" << endl;
	cout << setw(wkey) << ' ' << "必选项：" << endl;
	cout << setw(wopt) << ' ' << "--cno           : 课号" << endl;
	cout << setw(wopt) << ' ' << "--filename      : 文件名(all表示全部)" << endl;
	cout << setw(wopt) << ' ' << "--stulist_fname : 对应的学生列表文件名" << endl;
	cout << setw(wkey) << ' ' << "可选项：" << endl;
	cout << setw(wopt) << ' ' << "--debug         : Debug选项(仅适用于--secondline，默认11111)" << endl;
	cout << endl;

	cout << "e.g.   " << procname << " --firstline --cno 100692 --filename 3-b1.cpp --stulist_fname 100692.txt               : 检查100692课程的3-b1.cpp的首行" << endl;
	cout << "       " << procname << " --firstline --cno 100692 --filename all --stulist_fname 100692-firstline.txt          : 检查100692课程的全部文件的首行" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat               : 检查100742课程的15-b5.c的次行，输出全部信息" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat --debug 01001 : 检查100742课程的15-b5.c的次行，仅输出2、5项信息" << endl;
	cout << endl;

	if (args_num == 1) {
		cout << endl << endl << "请在cmd下加参数运行." << endl << endl;
		system("pause");
	}
}

/* enum的顺序要和args的排列顺序保持一致
   不用enum class 只是不想每次都做强制转换 */
enum OPT_ARGS { OPT_ARGS_HELP = 0, OPT_ARGS_FIRSTLINE, OPT_ARGS_SECONDLINE, OPT_ARGS_COURSEID, OPT_ARGS_FILENAME, OPT_ARGS_STULIST_FNAME, OPT_ARGS_DEBUG };

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	if (argc == 1) {
		usage(argv[0], argc);
		return -1;
	}

	args_analyse_tools args[] = {
		args_analyse_tools("--help", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--firstline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--secondline", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--cno", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--filename", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--stulist_fname", ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--debug", ST_EXTARGS_TYPE::str, 1, string("11111")),
		args_analyse_tools()  //最后一个，用于结束
	};

	int cur_argc, ret = 0;

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		return -1;
	}

	/* 需要自己实现 */
	args_analyse_print(args);

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		usage(argv[0], argc);
		return -1; //执行完成直接退出
	}

	/* 检查参数互斥性 */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_SECONDLINE].existed() != 1) {
		usage(argv[0], argc);
		cout << "参数[" << args[OPT_ARGS_FIRSTLINE].get_name() << "|" << args[OPT_ARGS_SECONDLINE].get_name() << "]必须指定一个且只能指定一个" << endl;
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_COURSEID].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_COURSEID].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_FILENAME].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_FILENAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断必选项是否存在 */
	if (args[OPT_ARGS_STULIST_FNAME].existed() == 0) {
		cout << "必须指定参数[" << args[OPT_ARGS_STULIST_FNAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断 --firstline 和 --debug 是否冲突 */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_DEBUG].existed() == 2) {
		cout << "参数[" << args[OPT_ARGS_FIRSTLINE].get_name() << "]不需要[" << args[OPT_ARGS_DEBUG].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* 判断 --secondline 和 --filename all 是否冲突 */
	if (args[OPT_ARGS_SECONDLINE].existed()) {
		if (args[OPT_ARGS_FILENAME].get_string() == "all") {
			cout << "参数[" << args[OPT_ARGS_SECONDLINE].get_name() << "]的[" << args[OPT_ARGS_SECONDLINE].get_name() << "]不能为all" << endl;
			usage(argv[0], argc);
			return -1;
		}
	}

	/* 判断 --debug 的额外参数是否满足要求 */
	if (args[OPT_ARGS_DEBUG].existed()) {
		int status = 1, len = args[OPT_ARGS_DEBUG].get_string().size();
		if (len != 5)
			status = 0;
		else
			for (int i = 0; i < 5; i++)
				if (args[OPT_ARGS_DEBUG].get_string()[i] != '0' && args[OPT_ARGS_DEBUG].get_string()[i] != '1')
					status = 0;
		if (status == 0) {
			usage(argv[0], argc);
			cout << "参数[--debug]必须为5位0/1" << endl;
			return -1;
		}
		else if (args[OPT_ARGS_DEBUG].get_string() == "00000") {
			usage(argv[0],argc);
			cout << "参数[--debug]的设置为[00000]，请至少指定一个1" << endl;
			return -1;
		}
	}

	/* 判断 --filename 的额外参数是否满足要求 */
	if (args[OPT_ARGS_FILENAME].existed()) {
		int len = args[OPT_ARGS_FILENAME].get_string().size();
		string ch;
		for (int i = 0; i < len; i++) {
			if (args[OPT_ARGS_FILENAME].get_string()[i] == '.') {
				while (i != len){
					ch += args[OPT_ARGS_FILENAME].get_string()[i];
					i++;
				}
				break;
			}
		}
		if (ch == "" || (ch != ".c" && ch != ".cpp")) {
			cout << "不是源程序文件" << endl;
			return -1;
		}
	}

	if (args[OPT_ARGS_FIRSTLINE].existed()) {
		/* 进行首行检查 */
		check_firstline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string());
	}
	else if (args[OPT_ARGS_SECONDLINE].existed()) {
		/* 进行次行检查 */
		check_secondline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string(), args[OPT_ARGS_DEBUG].get_string());
	}

	return 0;
}
