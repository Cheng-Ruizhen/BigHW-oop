/* 2152046 ������ ������ */
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <io.h>
#include "../include/cmd_gmw_tools.h"
#include "../include/cmd_console_tools.h"
#include "90-02-b5.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж������ַ����Ƿ�Ϊ�մ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool checkNull(string& str)
{
    int len = str.size();
    for (int i = 0; i < len; i++) {
        if (!(str[i] == ' ' || str[i] == '   '))
            return false;
    }
    return true;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ȡָ���ļ����µ�����txt�ļ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int getAllFiles(string*& files,int& num)
{
    //���·��
    string path = "./book";
    //�ļ���� 
    intptr_t hFile = 0;
    //�ļ���Ϣ�ṹ��
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("/*.txt").c_str(), &fileinfo)) != -1)
    {
        int LIST_SIZE = 10, LISTINCREMENT = 10, length = 0;
        files = new(nothrow) string[LIST_SIZE];
        if (files == NULL)
            return -1;

        do
        {
            if (length >= LIST_SIZE) {
                LIST_SIZE += LISTINCREMENT;
                string* newbase = new(nothrow) string[LIST_SIZE];
                if (newbase == NULL)
                    return -1;

                for (int i = 0; i < length; i++)
                    newbase[i] = files[i];
                delete[] files;
                files = newbase;
            }
            string temp = fileinfo.name;
            int pos = temp.find(".txt");
            temp = temp.erase(pos, 4);
            files[length++] = temp; //���洢��׺��
        } 
        while (_findnext(hFile, &fileinfo) == 0);
        num = length + 1;
        _findclose(hFile);
        return 0;
    }
    else {
        cout << "�ļ��в����ڻ��ļ����в�����txt�ļ�" << endl;
        return -1;
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ָ���ļ���С
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
size_t getFileSize(const string fileName) {

    if (fileName == "") {
        return 0;
    }

    // ����һ���洢�ļ�(��)��Ϣ�Ľṹ�壬�������ļ���С�ʹ���ʱ�䡢����ʱ�䡢�޸�ʱ���
    struct stat statbuf;

    // �ṩ�ļ����ַ���������ļ����Խṹ��
    stat(fileName.c_str(), &statbuf);

    // ��ȡ�ļ���С
    size_t filesize = statbuf.st_size;

    return filesize;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ѡ�����Ӧ�ļ����������Ķ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int read(string& opt)
{
    string filepath = "./book/" + opt + ".txt";
    ifstream fin;
    int filesize = 0;
    fin.open(filepath, ios::in);
    if (!fin.is_open()) {
        cout << "�ļ�[" << filepath << "]��ʧ�ܣ�" << endl;
        return -1;
    }
    if ((filesize = getFileSize(filepath)) > 20 * 1024 * 1024) {
        cout << "�ļ�[" << filepath << "]̫���޷��򿪣�" << endl;
        return -1;
    }

    CONSOLE_GRAPHICS_INFO read_CGI; //����һ���Ķ�CGI
    gmw_init(&read_CGI, read_row, read_col); //��ʼ��
    gmw_set_frame_color(&read_CGI, COLOR_HWHITE, COLOR_BLACK); // �׵׺���
    gmw_set_status_line_color(&read_CGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_HYELLOW, COLOR_RED);
    gmw_set_status_line_color(&read_CGI, LOWER_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_BLUE, COLOR_YELLOW);
    gmw_set_font(&read_CGI, "������", 18);				//����
    gmw_set_ext_rowcol(&read_CGI, 2, 3, 4, 4);          //��������
    read_CGI.is_need_mouse = 0;                         //����Ҫ������
    read_CGI.CFI.separator = 0;                         //ɫ��֮�䲻��Ҫ�ָ���

    gmw_draw_frame(&read_CGI); //�����
    read_CGI.SLI.top_start_x++;
    gmw_status_line(&read_CGI, TOP_STATUS_LINE, "", opt.c_str());
    read_CGI.SLI.lower_start_y++;
    const char* temp = "���¼�ͷ���������ƶ�һ�� PgUp/PgDn���������ƶ�һҳ S(s)����������ת N(n)�������������Ķ� Q(q)�����˳��Ķ� F(f)�����л��Ķ���ʽ";
    gmw_status_line(&read_CGI, LOWER_STATUS_LINE, "", temp);

    int SIZE = 512 * 1024, INCREMENT = 2 * 1024 * 1024;
    ROW* Article_Way1 = new(nothrow) ROW[SIZE], * Article_Way2 = new(nothrow) ROW[SIZE];
    int row1 = 0, row2 = 0;
    if (!Article_Way1 || !Article_Way2)
        return -2;

    // ��������ʾ�������ʽһ��ȫ���浽������
    Article_Way1[0].str += "    ";
    while (!fin.eof()) {
        if (row1 >= SIZE) {
            SIZE += INCREMENT;
            ROW* newbase = new(nothrow) ROW[SIZE];
            if (!newbase)
                return -2;
            memcpy(newbase, Article_Way1, row1 * sizeof(ROW));
            delete[] Article_Way1;
            Article_Way1 = newbase;
        }
        int num = Article_Way1[row1].str.size(), chnum = 0;
        char ch;
        Article_Way1[row1].pos = (int)(fin.tellg());
        while (!fin.eof()) {
            fin.get(ch);
            if (fin.eof())
                break;
            if (ch == '\n') { //�����س�
                row1++;
                Article_Way1[row1].str += "    ";//���ո�
                break;
            }
            num++;
            if ((num - chnum + 1) % 2 == 0 && ch >= 33 && ch <= 126)
                chnum++;
            Article_Way1[row1].str += ch;
            if (num == 4 * read_col) { //����һ��
                if (chnum % 2 != 0) { //�ַ�Ϊ���������һ���ַ�Ϊ������֣��ÿո���棬ָ�����
                    Article_Way1[row1].str.pop_back();
                    fin.seekg(-1, ios::cur);
                }
                row1++;
                break;
            }
        }
    }
    fin.close();
    fin.open(filepath, ios::in);
    // ��������ʾ�������ʽ����ȫ���浽������
    while (!fin.eof()) {
        if (row2 >= SIZE) {
            SIZE += INCREMENT;
            ROW* newbase = new(nothrow) ROW[SIZE];
            if (!newbase)
                return -2;
            memcpy(newbase, Article_Way2, row2 * sizeof(ROW));
            delete[] Article_Way2;
            Article_Way2 = newbase;
        }
        int num = Article_Way2[row2].str.size(), chnum = Article_Way2[row2].str.size();
        char ch;
        Article_Way2[row2].pos = (int)(fin.tellg());
        while (!fin.eof()) {
            fin.get(ch);
            if (fin.eof())
                break;
            if (ch == '\n') { //�����س�
                fin.get(ch);
                if (fin.eof())
                    break;
                if (ch == ' ' || ch == '   ') {
                    row2++;
                    Article_Way2[row2].str += ch;
                    break;
                }
                else if (ch == '\n') {
                    Article_Way2[++row2].pos = (int)(fin.tellg());
                    row2++;
                    break;
                }
                else if ((unsigned char)ch == 0xA1) {
                    fin.get(ch);
                    if ((unsigned char)ch == 0xA1) {
                        row2++;
                        Article_Way2[row2].str += ch;
                        Article_Way2[row2].str += ch;
                        break;
                    }
                    else
                        Article_Way2[row2].str += ch;
                }
            }
            num++;
            if ((num - chnum + 1) % 2 == 0 && ch >= 32 && ch <= 126)
                chnum++;
            Article_Way2[row2].str += ch;
            if (num == 4 * read_col) { //����һ��
                if (chnum % 2 != 0) { //�ַ�Ϊ���������һ���ַ�Ϊ������֣��ÿո���棬ָ�����
                    Article_Way2[row2].str.pop_back();
                    fin.seekg(-1, ios::cur);
                }
                row2++;
                break;
            }
        }
    }
    fin.close();

    // ��ȡ�ϴ��˳�λ��
    int start = 0;
    fstream position;
    position.open("./book/position.sav", ios::in);
    if (position.is_open()) {
        while (!position.eof()) {
            string temp;
            getline(position, temp, '\n');
            if (position.eof() && temp == "")
                break;
            if (checkNull(temp))
                continue;
            temp = temp.substr(1, temp.size() - 2);
            if (temp == opt) {
                getline(position, temp, '\n');
                while (checkNull(temp) && !position.eof())
                    getline(position, temp, '\n');
                if (temp != "")
                    start = atoi(temp.c_str());
                break;
            }
        }
    }
    position.close();
    if (start > filesize) //λ�ò�����
        start = 0;

    // ��ʼ�Ķ�
    int maction, mrow = 0, mcol = 0;
    int keycode1, keycode2;
    int ret;
    int start_x = read_CGI.start_x + 2, start_y = read_CGI.start_y + 2;
    int pos = 0; //�Ķ�����һ�е��±�
    ROW* Article_Way = Article_Way1; //�Ȳ��÷�ʽһ
    int row = row1;                  //�Ȳ��÷�ʽһ
    for (int i = 0; i <= row1; i++) {
        if (Article_Way[i].pos > start) {
            pos = i - 1;
            break;
        }
    }
    while (1) {
        char temp[256];
        sprintf(temp, "��ǰ�Ķ����ȣ�%.2f %%", 100.0 * Article_Way[pos].pos / filesize);
        cct_showstr(read_CGI.SLI.lower_start_x, read_CGI.SLI.lower_start_y + 1, temp, COLOR_BLUE, COLOR_HCYAN);
        for (int i = 0; i < 2 * read_row && i <= row1; i++) {
            if (i <= row)
                cct_showstr(start_x, start_y + i, Article_Way[pos + i].str.c_str(), read_CGI.CFI.bgcolor, read_CGI.CFI.fgcolor, 1, 4 * read_col);
            else
                cct_showstr(start_x, start_y + i, " ", read_CGI.CFI.bgcolor, read_CGI.CFI.fgcolor, 4 * read_col);
        }
        ret = gmw_read_keyboard_and_mouse(&read_CGI, maction, mrow, mcol, keycode1, keycode2);
        int status = 0;
        switch (keycode1) {
        case 0xE0:
            switch (keycode2) {
            case KB_ARROW_UP:
                if (pos > 0)
                    pos--;
                break;
            case KB_ARROW_DOWN:
                if (pos + 2 * read_row - 1 < row)
                    pos++;
                break;
            case KB_PAGE_UP:
                if (pos > 2 * read_row - 1)
                    pos -= 2 * read_row;
                else
                    pos = 0;
                break;
            case KB_PAGE_DOWN:
                if (pos <= row - 4 * read_row + 1)
                    pos += 2 * read_row;
                else
                    pos = row - 2 * read_row + 1;
                break;
            } //switch(keycode2)
            break;
        case 'N':
        case 'n':
        case 'Q':
        case 'q':
            position.open("./book/position.sav", ios::in);
            if (position.is_open()) {
                string file;
                while (!position.eof()) {
                    string temp;
                    getline(position, temp, '\n');
                    if (position.eof() && temp == "")
                        break;
                    if (checkNull(temp))
                        continue;
                    file += (temp + '\n');
                    temp = temp.substr(1, temp.size() - 2);
                    if (temp == opt) {
                        status = 1;
                        getline(position, temp, '\n');
                        while(checkNull(temp)&&!position.eof())
                            getline(position, temp, '\n');
                        file += (to_string(Article_Way1[pos].pos) + '\n');
                    }
                }
                if (status == 0) 
                    file += ('[' + opt + "]\n" + to_string(Article_Way1[pos].pos));
                position.close();
                position.open("./book/position.sav", ios::out);
                position.write(file.c_str(), file.size());
                position.close();
            }
            else {
                string file = '[' + opt + "]\n" + to_string(Article_Way1[pos].pos);
                position.open("./book/position.sav", ios::out);
                position.write(file.c_str(), file.size());
                position.close();
            }
            delete[] Article_Way1;
            delete[] Article_Way2;
            cct_gotoxy(0, read_CGI.SLI.lower_start_y + 3);
            cct_setcolor();
            if (keycode1 == 'N' || keycode1 == 'n')
                return 1;
            else
                return 0;
            break;
        case 'F':
        case 'f':
            if (Article_Way == Article_Way1)
                Article_Way = Article_Way2;
            else
                Article_Way = Article_Way1;
            row = row1 + row2 - row;
            if (pos > row - 2 * read_row + 1)
                pos = max(row - 2 * read_row + 1, 0);
            break;
        case 'S':
        case 's':
            int startx = read_CGI.start_x + (2 * read_col + 2) - 22;
            int starty = read_CGI.start_y + read_row + 1;
            cct_setcolor(COLOR_GREEN, COLOR_HWHITE);
            cct_gotoxy(startx, starty);
            cout << "��������Ҫ��ת�İٷֱ�λ��(0...100)��";
            double temp;
            cin >> temp;
            if (temp > 100)
                temp = 100;
            else if (temp < 0)
                temp = 0;
            temp /= 100;
            cct_setcolor(read_CGI.CFI.bgcolor, read_CGI.CFI.fgcolor);
            cct_gotoxy(startx, starty);
            cout << setw(42) << ' ';
            int skip = (int)(filesize * temp), i = 1;
            for (; i <= row; i++) {
                if (skip < Article_Way[i].pos) {
                    pos = i - 1;
                    break;
                }
            }
            if (i == row + 1 || pos > row - 2 * read_row + 1)
                pos = max(row - 2 * read_row + 1, 0);
            break;
        }//switch(keycode1)
    }
    
    return 0;
}