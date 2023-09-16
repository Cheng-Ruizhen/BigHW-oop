/* 2152046 程瑞真 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include "hw_check.h"
using namespace std;

int read_stulist(ostringstream& msg,string stulist_fname, student* stu, int& stunum)
{
	ifstream fin;
	string filename = "./source/" + stulist_fname;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		msg << "	--> 无法打开" << filename << endl;
		return -1;
	}
	int count = 0;
	char content[256] = { '\0' };
	while (!fin.eof()) {
		char course[64] = { '\0' }, no[64] = { '\0' }, name[64] = { '\0' }, sta1[64] = { '\0' }, sta2[64] = { '\0' };
		fin.getline(content, 256, '\n');
		if (content[0] == '\0' && fin.eof())   //最后一个换行
			break;
		if (fin.fail()) {  //长度超过了256字节
			msg << "	--> 学生名单[" << filename << "]中的第[" << count << "]行有错，忽略" << endl;
			break;
		}
		count++;

		//开始读取content内容
		unsigned j = 0;
		for (; j < strlen(content); j++) {
			if (content[j] != ' ' && content[j] != '	')
				break;
		}
		if (content[j] == '#') {  //注释行
			msg << "	--> 学生名单[" << filename << "]中的第[" << count << "]行为注释行，忽略" << endl;
			if (fin.fail()) {
				fin.clear();
				while (fin.get() != '\n')
					;
			}
		}
		else {
			int right = 1;
			unsigned k = j;
			if (j == strlen(content))
				right = 2;
			else {
				for (; k < j + 63 && content[k] != ' ' && content[k] != '	' && k < strlen(content); k++)
					course[k - j] = content[k];
				if (k == strlen(content))
					right = 0;
				j = k;
				while ((content[j] == ' ' || content[j] == '	') && j < strlen(content))
					j++;
				if (j == strlen(content))
					right = 0;
				for (k = j; k < j + 63 && content[k] != ' ' && content[k] != '	' && k < strlen(content); k++)
					no[k - j] = content[k];
				if (k == strlen(content))
					right = 0;
				j = k;
				while ((content[j] == ' ' || content[j] == '	') && j < strlen(content))
					j++;
				if (j == strlen(content))
					right = 0;
				for (k = j; k < j + 63 && content[k] != ' ' && content[k] != '	' && k < strlen(content); k++)
					name[k - j] = content[k];
				if (k == strlen(content))
					right = 0;
				j = k;
				while ((content[j] == ' ' || content[j] == '	') && j < strlen(content))
					j++;
				if (j == strlen(content))
					right = 0;
				for (k = j; k < j + 63 && content[k] != ' ' && content[k] != '	' && k < strlen(content); k++)
					sta1[k - j] = content[k];
				if (k == strlen(content))
					right = 0;
				j = k;
				while ((content[j] == ' ' || content[j] == '	') && j < strlen(content))
					j++;
				if (j == strlen(content))
					right = 0;
				for (k = j; k < j + 63 && content[k] != ' ' && content[k] != '	' && k < strlen(content); k++)
					sta2[k - j] = content[k];
				j = k;
				while ((content[j] == ' ' || content[j] == '	') && j < strlen(content))
					j++;
				if (j != strlen(content))
					right = 0;
			}
			if (right == 0) //content中不止五列
				msg << "	--> 学生名单[" << filename << "]中的第[" << count << "]行有错，忽略" << endl;
			else if (right == 1) {  //content中小于等于五列
				int repeat = 0;
				bool status1 = (bool)(atoi(sta1)), status2 = (bool)(atoi(sta2));
				for (int i = 0; i < stunum; i++) {
					if (!strcmp(stu[i].no, no)) {
						repeat = 1;
						break;
					}
				}
				if (repeat == 1)
					msg << "	--> 学生名单[" << filename << "]中的第[" << count << "]行学号[" << no << "]重复，忽略" << endl;
				else {
					strcpy(stu[stunum].course, course), strcpy(stu[stunum].no, no), strcpy(stu[stunum].name, name);
					stu[stunum].status1 = status1, stu[stunum].status2 = status2;
					stu[stunum].rnum = 0, stu[stunum].castoff = 0;
					stunum++;
				}
			}
		}
		if (fin.eof())
			break;
	}
	msg << "读取完成，共[" << count << "]行有内容，去重后学生总数[" << stunum << "]个" << endl;
	fin.close();
	return 0;
}

void print_stulist(ostringstream& msg, student* stu, int stunum)
{
	//排序
	for (int i = 0; i < stunum; i++) {
		for (int j = i; j < stunum; j++) {
			if (strcmp(stu[i].course, stu[j].course) > 0)
				swap(stu[i], stu[j]);
			else if (strcmp(stu[i].course, stu[j].course) == 0)
				if (strcmp(stu[i].no, stu[j].no) > 0)
					swap(stu[i], stu[j]);
		}
	}

	//输出
	msg << "序号	课号	学号	姓名	原始程序正确	校验程序正确" << endl;
	for (int i = 0; i < stunum; i++) {
		char temp[256];
		sprintf(temp, "%d#", i + 1);
		msg << setiosflags(ios::left) << setfill('	');
		msg << setw(strlen(temp) + 1) << temp << setw(strlen(stu[i].course) + 1) << stu[i].course << setw(strlen(stu[i].no) + 1) << stu[i].no << setw(strlen(stu[i].name) + 1) << stu[i].name << setw(2) << stu[i].status1 << stu[i].status2 << endl;
	}
	msg << resetiosflags(ios::left) << setfill(' ');
}

void read_2nd_line(ostringstream& msg, string courseid, string filename, student* stu, int stunum)
{
	string file;
	msg << "序号	课号	学号	姓名	文件名	分析结果" << endl;
	for (int i = 0; i < stunum; i++) {
		ifstream fin;
		char temp[256];
		file.clear();
		file = file + "./source/" + stu[i].course + "-" + filename + "/" + stu[i].no + "-" + stu[i].name + "-" + filename;
		sprintf(temp, "%d#", i + 1);
		msg << setiosflags(ios::left) << setfill('	');
		msg << setw(strlen(temp) + 1) << temp << setw(strlen(stu[i].course) + 1) << stu[i].course << setw(strlen(stu[i].no) + 1) << stu[i].no << setw(strlen(stu[i].name) + 1) << stu[i].name << setw(filename.size() + 1) << filename;
		fin.open(file, ios::in);
		if (!fin.is_open()) {
			msg << "未提交" << endl;
			stu[i].status2 = 0;
		}
		else {
			char content[256] = { '\0' };
			int count = 0;
			while (!fin.eof()) {
				fin.getline(content, 256, '\n');
				if (content[0] == '\0' && fin.eof())
					break;
				count++;
				int s = 0;
				for (unsigned j = 0; j < strlen(content); j++) {
					if (content[j] != ' ' && content[j] != '	' && content[j] != '\r') {
						s = 1;
						break;
					}
				}
				if (s == 0) {  //空行
					count--;
					continue;
				}
				if (count == 2)
					break;
				if (fin.fail()) {
					fin.clear();
					while (fin.get() != '\n')
						;
				}
			}
			if (fin.fail()) {
				msg << "非空行超过了合理长度[256]字节." << endl;
				fin.clear();
			}
			else {
				int f = 0, f1 = 0, f2 = 0;
				unsigned j = 0, k = 0;
				for (; j < strlen(content) - 1; j++) {
					if (content[j] != ' ' && content[j] != '	')
						break;
				}
				if (content[j] == '/' && content[j + 1] == '/')
					f = 1;
				else if (content[j] == '/' && content[j + 1] == '*')
					f1 = 1;
				for (; k < strlen(content) - 1; k++) {
					if (content[k] == '*' && content[k + 1] == '/') {
						f2 = 1;
						break;
					}
				}
				if (f == 0 && (f2 == 0 || f1 == 0))
					msg << "第2行信息有错[不是//或/*开头，或者以/*开头但*/不在同一行]" << endl;
				else {
					if (f2 == 1)
						content[k] = '\0';
					unsigned k = j + 2;
					int fnum = 1, count = 1;
					char no[64] = { '\0' }, name[64] = { '\0' };
					for (; k < strlen(content); k++) {
						while ((content[k] == ' ' || content[k] == '	' || content[k] == '\r') && k < strlen(content))
							k++;
						if (k == strlen(content))
							break;
						unsigned l = k;
						for (; l < strlen(content) && content[l] != ' ' && content[l] != '	' && l < k + 63; l++)
							no[l - k] = content[l];
						k = l;
						while ((content[k] == ' ' || content[k] == '	') && k < strlen(content))
							k++;
						for (l = k; l < strlen(content) && content[l] != ' ' && content[l] != '	' && l < k + 63 && content[l] != '\r'; l++)
							name[l - k] = content[l];
						k = l;
						if (!strcmp(stu[i].no, no)) {
							if (fnum != 1)
								msg << setw(5) << "	";
							msg << "第[" << count << "]项写了自己，耍无赖么？" << endl;
							fnum++;
							break;
						}
						int status = 0;
						for (int j = 0; j < stunum; j++) {
							if (!strcmp(stu[j].no, no) && !strcmp(stu[j].name, name)) {
								status = 1;
								break;
							}
						}
						if (status == 1) {  //有匹配项
							int a = 1;
							for (int g = 0; g < stu[i].rnum; g++) {
								if (!strcmp(stu[i].info[g].no, no)) {
									a = 0;
									break;
								}
							}
							if (a == 0) {
								msg << "互验列表第[" << count << "]项的学号[" << no << "]重复，忽略" << endl;
								fnum++;
							}
							else {
								strcpy(stu[i].info[stu[i].rnum].no, no);
								strcpy(stu[i].info[stu[i].rnum].name, name);
								count++, stu[i].rnum++;
							}
						}
						else {
							if (fnum != 1)
								msg << setw(5) << "	";
							if (strlen(name) == 0) {
								if (strlen(no))
									msg << "第[" << count - 1 << "]个学生后面的信息不全，忽略" << endl;
								else
									break;
							}
							else
								msg << "第[" << count << "]项的学号[" << no << "]/姓名[" << name << "]不正确，忽略" << endl;
							fnum++, count++;
						}
						memset(no, '\0', 64), memset(name, '\0', 64);
					}
					if (stu[i].rnum != 0) {
						if (fnum == 1)
							msg << "ok" << endl;
						//将读到的按学号排序
						for (int j = 0; j < stu[i].rnum; j++) {
							for (int k = j; k < stu[i].rnum; k++) {
								if (strcmp(stu[i].info[j].no, stu[i].info[k].no) > 0)
									swap(stu[i].info[j], stu[i].info[k]);
							}
						}
						msg << "	" << "读到的正确互验者" << endl;
						for (int j = 0; j < stu[i].rnum; j++) {
							sprintf(temp, "%d", j + 1);
							msg << "	" << setw(strlen(temp) + 1) << j + 1 << setw(strlen(stu[i].info[j].no) + 1) << stu[i].info[j].no << stu[i].info[j].name << endl;
						}
					}
					else if (stu[i].rnum == 0 && fnum == 1)
						msg << "ok" << endl;
				}
			}
			fin.close();
		}
	}
}

void print_2nd_line(ostringstream& msg, student* stu, int stunum)
{
	msg << "序号	课号	学号	姓名" << endl;
	for (int i = 0; i < stunum; i++) {
		char temp[256] = { '\0' };
		sprintf(temp, "%d#", i + 1);
		msg << setiosflags(ios::left) << setfill('	');
		msg << setw(strlen(temp) + 1) << temp << setw(strlen(stu[i].course) + 1) << stu[i].course << setw(strlen(stu[i].no) + 1) << stu[i].no << stu[i].name;
		if (stu[i].rnum == 0)
			msg << "\t无有效匹配项" << endl;
		else {
			msg << endl;
			for (int j = 0; j < stu[i].rnum; j++) {
				sprintf(temp, "$%d", j + 1);
				int k = 0;
				for (; k < stunum; k++) {
					if (!strcmp(stu[i].info[j].no, stu[k].no))
						break;
				}
				int L = 0;
				for (; L < stu[k].rnum; L++) {
					if (!strcmp(stu[i].no, stu[k].info[L].no))
						break;
				}
				if (L == stu[k].rnum) {
					stu[i].info[j].right = 0;
					stu[i].castoff++;
					msg << '	' << setw(strlen(temp) + 1) << temp << setw(strlen(stu[i].info[j].no) + 1) << stu[i].info[j].no << setw(strlen(stu[i].info[j].name) + 1) << stu[i].info[j].name << "抛弃了你" << endl;
				}
				else {
					stu[i].info[j].right = 1;
					msg << '	' << setw(strlen(temp) + 1) << temp << setw(strlen(stu[i].info[j].no) + 1) << stu[i].info[j].no << setw(strlen(stu[i].info[j].name) + 1) << stu[i].info[j].name << "ok" << endl;
				}
			}
		}
	}
}

void print_finalscore(ostringstream& msg, student* stu, int stunum)
{
	msg << "序号	课号	学号	姓名	原始程序得分	检查程序原始分	检查程序扣分1	检查程序扣分2	检查程序总分" << endl;
	for (int i = 0; i < stunum; i++) {
		msg << setiosflags(ios::left) << setfill('	');
		int original = stu[i].status1 * 100, check = stu[i].status2 * 100, deduct1 = stu[i].castoff * 100, deduct2 = (5 - stu[i].rnum) > 0 ? (5 - stu[i].rnum) * 20 : 0;
		char temp1[256] = { '\0' }, temp2[256] = { '\0' }, temp3[256] = { '\0' }, temp4[256] = { '\0' }, temp5[256] = { '\0' };
		sprintf(temp1, "%d#", i + 1);     //序号
		sprintf(temp2, "%d", original);   //原始程序得分
		sprintf(temp3, "%d", check);      //检查程序得分
		sprintf(temp4, "%d", -deduct1);    //检查程序扣分1
		sprintf(temp5, "%d", -deduct2);    //检查程序扣分2
		msg << setw(strlen(temp1) + 1) << temp1 << setw(strlen(stu[i].course) + 1) << stu[i].course << setw(strlen(stu[i].no) + 1) << stu[i].no << setw(strlen(stu[i].name) + 1) << stu[i].name
			<< setw(strlen(temp2) + 1) << temp2 << setw(strlen(temp3) + 1) << temp3 << setw(strlen(temp4) + 1) << temp4 << setw(strlen(temp5) + 1) << temp5 << check - deduct1 - deduct2 << endl;
	}
}