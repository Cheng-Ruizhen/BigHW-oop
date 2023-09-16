/* 2152046 信14 程瑞真 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include "lib_mto10_net_tools.h"
#include"cmd_console_tools.h"
#include"90-b2.h"
using namespace std;

#define		MAX_ROW_NUM		8
#define		MAX_COL_NUM		10

int game_progress(mto10_net_tools& client,char choice)
{
	char svrpack[RECVBUF_LEN];	//存放从Server端收到的数据
	char row;
	int col;
	const char* char_map;
	int i = 0;
	if(choice=='B')
		cct_setconsoleborder(8 * client.get_col() + 5, 4 * client.get_row() + 11, 8 * client.get_col() + 5, 4 * client.get_row() + 11);
	while (1) {
		/* -------------------
			Client=>Server
		   ------------------- */
		   /* 根据服务端发来的地图，返回一个解坐标（目前为手工输入形式，需要改为自动求最佳解坐标）
			  提示：可以将client.get_map()的返回值复制到字符数组中，再还原为你自己的二维数组后求解
		   */
		int map[10][12] = { 0 }, v[10][12] = { 0 }, x[2] = { 1,1 };
		char_map = client.get_map();
		for (int i = 1; i <= client.get_row(); i++) {
			for (int j = 1; j <= client.get_col(); j++) {
				map[i][j] = char_map[j - 1 + (i - 1) * client.get_col()] - '0';
			}
		}
		solution(client.get_row(), client.get_col(), map, v, x);
		if(choice=='A')
			client.print_map(); //本函数仅为示意，图形界面中不能调用，要自己写转换函数
		else if (choice == 'B') {
			if (i == 0)
				print_console(client.get_row(), client.get_col(), map, choice);
			cct_setcolor();
			cct_gotoxy(0, 0);
			cout << "Step=" << client.get_step() << " Score=" << client.get_score() << "                         ";
			is_search(client.get_row(), client.get_col(), x[0], x[1], map, v);
			for (int i = 1; i <= client.get_row(); i++) {
				for (int j = 1; j <= client.get_col(); j++) {
					cct_showstr(8 * j - 4, 4 * i - 1, "┌─┐", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 4, 4 * i, "│", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showch(8 * j - 2, 4 * i, (char)(map[i][j] + '0'), map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 1, 4 * i, " │", map[i][j] % 6 + 1, COLOR_BLACK);
					cct_showstr(8 * j - 4, 4 * i + 1, "└─┘", map[i][j] % 6 + 1, COLOR_BLACK);
				}
			}
			if (client.is_gameover()==0) {
				for (int i = 1; i <= client.get_row(); i++) {
					for (int j = 1; j <= client.get_col(); j++) {
						if (v[i][j] == 1) {
							cct_showstr(8 * j - 4, 4 * i - 1, "┌─┐", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 4, 4 * i, "│", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showch(8 * j - 2, 4 * i, (char)(map[x[0]][x[1]] + '0'), map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 1, 4 * i, " │", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
							cct_showstr(8 * j - 4, 4 * i + 1, "└─┘", map[x[0]][x[1]] % 6 + 1, COLOR_WHITE);
						}
					}
				}
			}
			Sleep(200);
			cct_setcolor();
			int max = change(client.get_row(), client.get_col(), x[0], x[1], map, v);
			print_blank(client.get_row(), client.get_col(), map);
			Sleep(100);
			cct_setcolor();
			int p = x[0], q = x[1];
			cct_showstr(8 * x[1] - 4, 4 * x[0] - 1, "┌─┐", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 4, 4 * x[0], "│", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showch(8 * x[1] - 2, 4 * x[0], (char)(map[x[0]][x[1]] + '0'), map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 1, 4 * x[0], " │", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			cct_showstr(8 * x[1] - 4, 4 * x[0] + 1, "└─┘", map[x[0]][x[1]] % 6 + 1, COLOR_BLACK);
			down(client.get_row(), client.get_col(), map, v, x[0], x[1], choice);
			Sleep(100);
			for (int i = 1; i <= client.get_row(); i++) {
				for (int j = 1; j <= client.get_col(); j++) {
					v[i][j] = 0;
				}
			}
			x[0] = p, x[1] = q;
		}
		i++;
		/* 游戏已结束则不再读键*/
		if (client.is_gameover())
			break;

		row = char('A' + x[0] - 1);
		col = x[1] - 1;


		if (row >= 'a' && row <= 'j')
			row -= 32; //如果是小写则转为大写
		client.send_coordinate(row, col);

		/* -------------------
			Server=>Client
		   ------------------- */
		   /* 等待Server端的gameprogress */
		if (client.get_gameprogress_string(svrpack) < 0) {
			return -1;
		}
		/* 打印收到的原始字符串 */
		if(choice=='A')
			cout << "Server应答 : " << endl << svrpack << endl;
	}//end of while(1)

	return 0;
}

int main_net(char choice)
{
	const char* my_no = "2152046";		//本人学号
	const char* my_pwd = "crz135799.";	//本人密码

	int wanted_row, wanted_col, wanted_id, wanted_delay;

#if 1 //条件编译，此处改为 #if 0 则表示启用下面 #else 中的语句
	wanted_row = 5;			//此处按需修改为键盘输入等形式
	wanted_col = 5;			//此处按需修改为键盘输入等形式
	wanted_id = -1;		//id为本次请求的游戏序号，如果置-1，表示服务器随机产生，[0..2^31-1]则表示想固定序号（序号固定，则每次给出的初始矩阵及后续填充均相同，方便调试）
	wanted_delay = 60 * 1000;	//delay为请求的每步应答的最长时延，单位ms
#else
	wanted_row = -1;
	wanted_col = -1;
	wanted_id = -1;
	wanted_delay = -1;
#endif
	mto10_net_tools client;

	if (choice == 'A' || choice == 'B') {
		/* 打开client类对象中的debug开关（调试时可打开，到图形界面时需关闭） */
		client.set_debug_switch(false);
		/* 连接服务器 */
		if (client.connect() < 0) {
			/* 连接失败信息，有没有debug_switch都打印 */
			cout << "连接服务器失败!" << endl;
			return -1;
		}

		/* 向Server端发送认证信息及参数 */
		if (client.send_parameter(my_no, '*', my_pwd, wanted_row, wanted_col, wanted_id, wanted_delay) < 0) {
			cout << "发送认证及参数信息失败!" << endl;
			return -1;
		}

		/* 等待Server端下发参数（游戏开始）*/
		if (client.wait_for_game_start() < 0) {
			cout << "等待GameStart失败!" << endl;
			return -1;
		}

		if (choice == 'A') {
			/* 打印从服务器收到的参数，本次游戏以此参数为准（例：发送6行6列，返回8行10列，则本次游戏以8行10列为准）
		   下面打印的信息，图形化界面可以自行去除 */
			cout << "服务器返回信息 : " << endl;
			cout << "  行数 : " << client.get_row() << endl;
			cout << "  列数 : " << client.get_col() << endl;
			cout << "  ID号 : " << client.get_gameid() << endl;
			cout << "  超时 : " << client.get_delay() / 1000.0 << "(秒)" << endl;
			cout << "  地图 : " << client.get_map() << endl;
		}
		/* 地图串中的值为1-9，A-F，分别表示1-15的值，
			   例如：
				 服务器返回行=3
				 服务器返回列=4
				 服务器返回地图=123A85CF2123
			   则表示当前地图为(本例仅仅是示例)
				 1 2 3  10
				 8 5 12 15
				 2 1 2  3
				游戏结束的条件为：合成到16或无任何可合成元素 */

				/* 进入游戏交互环节
				   1、收到Server的GameOver则返回0，游戏结束
				   2、其它错误均返回-1（报文信息不正确等错误），重连，再次重复	*/
		if (game_progress(client, choice) < 0) {
			client.close();
			return -1;
		}
	}
	else {
		int num = 0;
		client.set_debug_switch(false);
		while (num < 64) {
			num++;
			mto10_net_tools client;
			/* 连接服务器 */
			if (client.connect() < 0) {
				/* 连接失败信息，有没有debug_switch都打印 */
				cout << "连接服务器失败!" << endl;
				continue;
			}

			/* 向Server端发送认证信息及参数 */
			if (client.send_parameter(my_no, '-', my_pwd, wanted_row, wanted_col, wanted_id, wanted_delay) < 0) {
				cout << "发送认证及参数信息失败!" << endl;
				continue;
			}

			/* 等待Server端下发参数（游戏开始）*/
			if (client.wait_for_game_start() < 0) {
				cout << "等待GameStart失败!" << endl;
				continue;
			}

			if (game_progress(client, choice) < 0) {
				client.close();
				continue;
			}
			cout << setiosflags(ios::left);
			cout << "序号=" << setw(2) << num << " MAPID=" << setw(10) << client.get_gameid() << " ,游戏结束，最终得分：" << client.get_score() << endl;
			client.close();
		}
	}
	if (choice == 'B') {
		cct_gotoxy(0, client.get_col() * 4 + 4);
		cct_setcolor();
	}
	cout << "游戏结束" << endl;

	return 0;
}

void solution(int row, int col, int map[10][12], int v[10][12],int x[2])
{
	int min = 1, num[10][12] = { 0 }, num1[10][12] = { 0 };
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			no_search(row, col, i, j, map, v);
			for (int x = 1; x <= row; x++) {
				for (int y = 1; y <= col; y++) {
					if (v[x][y] != 0) {
						num[i][j]++;
						v[x][y] = 0;
					}
				}
			}
			if (num[i][j] > 1) {
				min = map[i][j];
			}
		}
	}
	int i, j;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (map[i][j] < min && num[i][j]>1)
				min = map[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (map[i][j] == min && num[i][j] > 1) {
				if (row <= 6 && col <= 7) {
					x[0] = i, x[1] = j;
					return;
				}
				num1[i][j] = num[i][j];
			}
		}
	}
	int min_num = 0;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] != 0)
				min_num = num1[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] < min_num && num1[i][j]>1)
				min_num = num1[i][j];
		}
	}
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (num1[i][j] == min_num) {
				x[0] = i, x[1] = j;
				return;
			}
		}
	}
}
