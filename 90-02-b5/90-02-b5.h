#pragma once
/* 2152046 ������ ������ */
#include <string>
using namespace std;

// �Ķ�ʱһ�����ֵ���Ϣ
struct ROW {
	string str; //���������
	int pos; //��һ�е�һ�������ļ��е�λ��
	ROW() {
		str = "";
		pos = 0;
	}
};

// �˵���
const int menu_row = 6; //ʵ��Ϊ12��
const int menu_col = 4; //ʵ��Ϊ8������

//�Ķ���
const int read_row = 22; //44��
const int read_col = 33; //66������

int getAllFiles(string*& files, int& num);
int read(string& opt);