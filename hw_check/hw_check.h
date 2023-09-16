/* 2152046 程瑞真 大数据 */
#pragma once
#include <sstream>
#include <string>
using namespace std;
/* 加入自己需要的定义 */
struct student {
	char course[64];
	char no[64];
	char name[64];
	bool status1, status2;
	int castoff;
	struct {
		char no[64];
		char name[64];
		bool right;
	}info[20];
	int rnum;
};

int read_stulist(ostringstream& msg,string stulist_fname, student* stu, int& stunum);
void print_stulist(ostringstream& msg, student* stu, int stunum);
void read_2nd_line(ostringstream& msg, string cno, string filename, student* stu, int stunum);
void print_2nd_line(ostringstream& msg, student* stu, int stunum);
void print_finalscore(ostringstream& msg, student* stu, int stunum);

