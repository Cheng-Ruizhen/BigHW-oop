/* 2152046 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//�ɸ�����Ҫ�ټ���������Ҫ��ͷ�ļ�
#include <iomanip>
#include "../include/class_aat.h"
using namespace std;

/* ���������������������Ҳ���Խ��Զ��庯����������cpp�� */
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
	cout << " IP��ַ" << endl;
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char* argv[])
{
	args_analyse_tools args[] = {
		args_analyse_tools("--l",ST_EXTARGS_TYPE::int_with_default,1, 64, 32, 64000),
		args_analyse_tools("--n",ST_EXTARGS_TYPE::int_with_default,1, 4, 1, 1024),
		args_analyse_tools("--t",ST_EXTARGS_TYPE::boolean,         0, false),	
		args_analyse_tools()  //���һ�������ڽ���
	};
	const char* func[] = {
		"��С",
		"����",
		NULL
	};

	if (argc == 1) {
		usage(argv[0], args, func);
		return -1;
	}

	// �ȼ��IP��ַ
	if (!check_IP(argv[argc - 1])) {
		cout << "IP��ַ����" << endl;
		return -1;
	}

	int cur_argc;

	if ((cur_argc = args_analyse_process(argc - 1, argv, args, 1)) < 0)
	{
		//������Ϣ�ں������Ѵ�ӡ
		return -1;
	}
	else if ((argc - cur_argc) != 1) { // �̶��������ҽ���һ��
		if (argv[cur_argc][0] == '-' && argv[cur_argc][1] == '-')
			cout << "����[" << argv[cur_argc] << "]������" << endl;
		else
			cout << "����[" << argv[cur_argc] << "]������--��ͷ�ĺϷ�����" << endl;
		return -1;
	}
	else
	{
		cout << "�������ͨ��" << endl;
		for (int i = 0; args[i].get_name() != ""; i++) {
			cout << args[i].get_name() << " ������";
			if (args[i].get_name() == "--t")
				cout << args[i].existed() << endl;
			else
				cout << args[i].get_int() << endl;
		}
		cout << "IP��ַ��" << argv[argc - 1] << endl;
	}

	return 0;
}