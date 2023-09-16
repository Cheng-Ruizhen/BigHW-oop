#pragma once
/* 2152046 程瑞真 大数据 */
#include <string>
using namespace std;

// 阅读时一行文字的信息
struct ROW {
	string str; //存该行文字
	int pos; //这一行第一个字在文件中的位置
	ROW() {
		str = "";
		pos = 0;
	}
};

// 菜单区
const int menu_row = 6; //实际为12行
const int menu_col = 4; //实际为8个汉字

//阅读区
const int read_row = 22; //44行
const int read_col = 33; //66个汉字

int getAllFiles(string*& files, int& num);
int read(string& opt);