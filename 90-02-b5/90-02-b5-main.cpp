/* 2152046 程瑞真 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "../include/popmenu.h"
#include "../include/cmd_console_tools.h"
#include "90-02-b5.h"
using namespace std;

int main()
{
	string* files = NULL;
	int num;
	if (getAllFiles(files, num) == -1)
		return -1;
	while (1) {
		string opt = pop_menu(files, num, menu_row, menu_col);
		int status = read(opt);
		cct_setcolor();
		if (status <= 0)
			return status;
	}

	delete[] files;
	return 0;
}