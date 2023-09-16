/* 2152046 ������ ������ */
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ���������ԣ�ʵ��ʹ��ʱ��������һ��cpp(����hw_check_secondline.cpp)������
 ***************************************************************************/
int check_secondline(const string& cno, const string &stulist_fname, const string& filename, const string &debug)
{
	cout << "��ʼ����Դ�����ļ����м��..." << endl;//������ҵҪ��ʵ�֣�������Ҫ�Ķ�
	ostringstream msg;

	/* �������������䣬��������cout */
	int stunum = 0, max_student = 1010;
	student* stu = new(nothrow) student[max_student];
	if (stu == NULL)
		return -1;

	/* ���κ�.dat */
	msg << "1.��ȡ�κ������ļ�" << endl;
	int status = read_stulist(msg, stulist_fname, stu, stunum); //���������ֵ��Ҳ���Բ�Ҫ������������Լ����������һ����msg 
	/* ��������������(Ҳ���Բ�Ҫ)����������cout */
	if (debug[0] == '1')
		cout << msg.str() << endl;

	/* �������������䣬��������cout */
	if (status == -1) //�޷����ļ���������к�������
		return 0;

	msg.str(""); //���msg�е���Ϣ
	msg << "2.���κ�+ѧ�������ѧ������" << endl;
	print_stulist(msg, stu, stunum); //���������ֵ��Ҳ���Բ�Ҫ������������Լ����������һ����msg 
	/* ��������������(Ҳ���Բ�Ҫ)����������cout */
	if (debug[1] == '1')
		cout << msg.str() << endl;

	/* �������������䣬��������cout */

	msg.str(""); //���msg�е���Ϣ
	msg << "3.����ѧ����ȡ��2�еĽ�����Ϣ" << endl;
	read_2nd_line(msg, cno, filename, stu, stunum); //���������ֵ��Ҳ���Բ�Ҫ������������Լ����������һ����msg 
	/* ��������������(Ҳ���Բ�Ҫ)����������cout */
	if (debug[2] == '1')
		cout << msg.str() << endl;

	/* �������������䣬��������cout */

	/* ��˫��������Ϣƥ�� */
	msg.str(""); //���msg�е���Ϣ
	msg << "4.����ѧ���ĵ�2�л���ƥ����" << endl;
	print_2nd_line(msg, stu, stunum); //���������ֵ��Ҳ���Բ�Ҫ������������Լ����������һ����msg 
	/* ��������������(Ҳ���Բ�Ҫ)����������cout */
	if (debug[3] == '1')
		cout << msg.str() << endl;

	/* �������������䣬��������cout */

	/* �ٸ���ͨ�������м�� */
	msg.str(""); //���msg�е���Ϣ
	msg << "5.����ѧ���ĵ�2�л������յ÷�" << endl;
	print_finalscore(msg, stu, stunum); //���������ֵ��Ҳ���Բ�Ҫ������������Լ����������һ����msg 
	/* ��������������(Ҳ���Բ�Ҫ)����������cout */
	if (debug[4] == '1')
		cout << msg.str() << endl;

	/* �������������䣬��������cout */
	delete[] stu;
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����������ҵ��Ҫ��ʵ�֣���������Ҫ�Ķ�
 ***************************************************************************/
int check_firstline(const string& cno, const string &stilist_fname, const string& filename)
{
	cout << "��ʼ����Դ�����ļ����м��..." << endl;
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static void usage(const char* const procname, const int args_num)
{
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;

	cout << endl;
	cout << "Usage: " << procname << " { --firstline | --secondline  }" << endl;
	cout << setw(wkey) << ' ' << "{ --cno xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --filename xxx/all }" << endl; //Լ��������ʵ���ļ���Ϊall
	cout << setw(wkey) << ' ' << "{ --stulist_fname xxx }" << endl;
	cout << setw(wkey) << ' ' << "{ --debug 11111 }" << endl;
	cout << endl;

	cout << setw(wkey) << ' ' << "��ѡ�ָ���������(��ѡһ)" << endl;
	cout << setw(wopt) << ' ' << "--firstline     : ���м��" << endl;
	cout << setw(wopt) << ' ' << "--secondline    : ���м��" << endl;
	cout << setw(wkey) << ' ' << "��ѡ�" << endl;
	cout << setw(wopt) << ' ' << "--cno           : �κ�" << endl;
	cout << setw(wopt) << ' ' << "--filename      : �ļ���(all��ʾȫ��)" << endl;
	cout << setw(wopt) << ' ' << "--stulist_fname : ��Ӧ��ѧ���б��ļ���" << endl;
	cout << setw(wkey) << ' ' << "��ѡ�" << endl;
	cout << setw(wopt) << ' ' << "--debug         : Debugѡ��(��������--secondline��Ĭ��11111)" << endl;
	cout << endl;

	cout << "e.g.   " << procname << " --firstline --cno 100692 --filename 3-b1.cpp --stulist_fname 100692.txt               : ���100692�γ̵�3-b1.cpp������" << endl;
	cout << "       " << procname << " --firstline --cno 100692 --filename all --stulist_fname 100692-firstline.txt          : ���100692�γ̵�ȫ���ļ�������" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat               : ���100742�γ̵�15-b5.c�Ĵ��У����ȫ����Ϣ" << endl;
	cout << "       " << procname << " --secondline --cno 100742 --filename 15-b5.c --stulist_fname 10108001.dat --debug 01001 : ���100742�γ̵�15-b5.c�Ĵ��У������2��5����Ϣ" << endl;
	cout << endl;

	if (args_num == 1) {
		cout << endl << endl << "����cmd�¼Ӳ�������." << endl << endl;
		system("pause");
	}
}

/* enum��˳��Ҫ��args������˳�򱣳�һ��
   ����enum class ֻ�ǲ���ÿ�ζ���ǿ��ת�� */
enum OPT_ARGS { OPT_ARGS_HELP = 0, OPT_ARGS_FIRSTLINE, OPT_ARGS_SECONDLINE, OPT_ARGS_COURSEID, OPT_ARGS_FILENAME, OPT_ARGS_STULIST_FNAME, OPT_ARGS_DEBUG };

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
		args_analyse_tools()  //���һ�������ڽ���
	};

	int cur_argc, ret = 0;

	//���һ������1����ʾ��ѡ������⣬������������
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//������Ϣ�ں������Ѵ�ӡ
		return -1;
	}

	/* ��Ҫ�Լ�ʵ�� */
	args_analyse_print(args);

	/* ��help�����⴦�� */
	if (args[OPT_ARGS_HELP].existed()) {
		//ֻҪ�� --help���������������ԣ���ʾ��������
		usage(argv[0], argc);
		return -1; //ִ�����ֱ���˳�
	}

	/* ������������ */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_SECONDLINE].existed() != 1) {
		usage(argv[0], argc);
		cout << "����[" << args[OPT_ARGS_FIRSTLINE].get_name() << "|" << args[OPT_ARGS_SECONDLINE].get_name() << "]����ָ��һ����ֻ��ָ��һ��" << endl;
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_COURSEID].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_COURSEID].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_FILENAME].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_FILENAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �жϱ�ѡ���Ƿ���� */
	if (args[OPT_ARGS_STULIST_FNAME].existed() == 0) {
		cout << "����ָ������[" << args[OPT_ARGS_STULIST_FNAME].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �ж� --firstline �� --debug �Ƿ��ͻ */
	if (args[OPT_ARGS_FIRSTLINE].existed() + args[OPT_ARGS_DEBUG].existed() == 2) {
		cout << "����[" << args[OPT_ARGS_FIRSTLINE].get_name() << "]����Ҫ[" << args[OPT_ARGS_DEBUG].get_name() << "]" << endl;
		usage(argv[0], argc);
		return -1;
	}

	/* �ж� --secondline �� --filename all �Ƿ��ͻ */
	if (args[OPT_ARGS_SECONDLINE].existed()) {
		if (args[OPT_ARGS_FILENAME].get_string() == "all") {
			cout << "����[" << args[OPT_ARGS_SECONDLINE].get_name() << "]��[" << args[OPT_ARGS_SECONDLINE].get_name() << "]����Ϊall" << endl;
			usage(argv[0], argc);
			return -1;
		}
	}

	/* �ж� --debug �Ķ�������Ƿ�����Ҫ�� */
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
			cout << "����[--debug]����Ϊ5λ0/1" << endl;
			return -1;
		}
		else if (args[OPT_ARGS_DEBUG].get_string() == "00000") {
			usage(argv[0],argc);
			cout << "����[--debug]������Ϊ[00000]��������ָ��һ��1" << endl;
			return -1;
		}
	}

	/* �ж� --filename �Ķ�������Ƿ�����Ҫ�� */
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
			cout << "����Դ�����ļ�" << endl;
			return -1;
		}
	}

	if (args[OPT_ARGS_FIRSTLINE].existed()) {
		/* �������м�� */
		check_firstline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string());
	}
	else if (args[OPT_ARGS_SECONDLINE].existed()) {
		/* ���д��м�� */
		check_secondline(args[OPT_ARGS_COURSEID].get_string(), args[OPT_ARGS_STULIST_FNAME].get_string(), args[OPT_ARGS_FILENAME].get_string(), args[OPT_ARGS_DEBUG].get_string());
	}

	return 0;
}
