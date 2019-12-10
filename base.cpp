#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
//#define DEBUG
#include "front.cpp"
#include "struct_def.h"
#include "back_end.h"
#include "connect_to_server.h"
#include <time.h>
#include <conio.h>
//#define BACK_DEBUG

void main_iterface();//开始的界面 
void start_game() {
	set.line = 1;
	player1.x = set.map_width >> 1;
	player1.y = 1;
	player1.pre_time = GetTickCount();
	player1.pre_board = 1;
	player1.pre_board_type = 1;
	player1.remain_bounce_line = set.remain_bounce_line;
	add_board(&head, &tail, 1, (set.map_width >> 1) - 1, 1, 0, 0, 0);
	multi_gen_board(&set, &head, &tail, 4, 100);
	int current_board = 100;  //记录当前板生成到了哪一行
	while (1) {
		unsigned long long current_time = GetTickCount();//此次操作开始的基准时间
		//读取键盘输入
		int key_state = get_key_state();
		//更新x方向位置
		player1.x += (key_state * set.velocity_LR);
		if (player1.x <= 1) {
			player1.x = set.map_width - 1;
		}
		if (player1.x >= set.map_width) {
			player1.x = 1;
		}
		//更新y方向位置
		if (player1.remain_bounce_line > 0) {
			switch (player1.pre_board_type) {
			case 1:
				player1.y++;
				player1.remain_bounce_line--;
				break;
			case 2:
				player1.y += 2;
				player1.remain_bounce_line -= 2;
				break;
			default:
				break;
			}
		}
		else {
			player1.y--;
		}
		//判断是否应生成新板
		if (player1.y >= current_board - 20) {
			multi_gen_board(&set, &head, &tail, current_board + 1, current_board + 100);
			current_board += 100;
		}
		//处理板的移动情况
		move_board(&set, head);
		//判断是否坠入虚空
		if (drop_into_void(&set, &player1)) {
			gameover(&set,&player1);
			fflush(stdin);
			getchar();
			while (head != NULL) {
				delete_board(&head, &tail, head->line_id);
			}
			//char name[200]; 
			//最好把名字定义成全局 
			in_name();//输入名字！ 
			//save_score(high_score, &current_score, name, set.line);
			
			return;
		}
		//判断是否碰撞
		switch (land_on_board(&set, head, &player1))
		{
		case 1:
			player1.pre_board = player1.y;
			player1.pre_board_type = 1;
			player1.pre_time = current_time;
			player1.remain_bounce_line = set.remain_bounce_line;
			break;
		case 2:
			player1.pre_board = player1.y;
			player1.pre_board_type = 2;
			player1.pre_time = current_time;
			player1.remain_bounce_line = set.remain_bounce_line + 10;
			break;
		case 3:
			player1.pre_board = player1.y;
			player1.pre_board_type = 1;
			player1.pre_time = current_time;
			player1.remain_bounce_line = set.remain_bounce_line;
			break;
		default:
			break;
		}
		//提交渲染
#ifdef DEBUG
		for (int i = 0; i <= set.map_height; i++) {
			for (int j = 0; j <= set.map_width; j++) {
				printf("%d ", Map[i][j]);
			}
			putchar('\n');
		}
		system("pause");
#endif // DEBUG
		render_player(&player1, &set);
		render_boards(head, &set, &Map);
		show(&player1, &set);
		//判断是否删除之前的板
		while (head != NULL && head->line_id < set.line) {
			delete_board(&head, &tail, head->line_id);
		}
		//线程休息
		if (GetTickCount() - current_time > set.dp_tpf) {
			continue;
		}
		DWORD end_time = set.dp_tpf - (GetTickCount() - current_time);
		Sleep(end_time);
	}
}
void init() {  //游戏数据的初始化
	read_data(&set);
	Map = (char**)malloc(sizeof(char*) * (set.map_height + 1));
	for (int i = 0; i <= set.map_height; i++) {
		Map[i] = (char*)malloc(sizeof(char) * set.map_width + 1);
		for (int j = 0; j <= set.map_width; j++) {
			Map[i][j] = ' ';
		}
	}
	srand(time(0));
	
	//读入玩家历史得分数据
	read_score(high_score, &current_score);
	
	 //创建新的控制台缓冲区
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOutput = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    //隐藏缓冲区的光标
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutBuf, &cci);
	SetConsoleCursorInfo(hOutput, &cci);
	
	
	/*COORD size = { set.map_width+5,set.map_height+5 };
	
	SetConsoleScreenBufferSize(, size);
	SetConsoleScreenBufferSize(, size);

	Sleep(3000);
	
	
	system("mode con cols=30 lines=20");
	Sleep(3000);
	modeset(20,30);*/modeset(set.map_width+3,set.map_height+5,hOutput);
	modeset(set.map_width+3,set.map_height+5,hOutBuf);


	print_frame(&set,hOutput);
	print_frame(&set,hOutBuf);
	main_iterface();
		
}

#ifdef BACK_DEBUG
FILE* file;
void print_board(struct board* p) {
	while (p != NULL) {
		fprintf(file,"%d %d %d\n", p->line_id, p->type, p->x);
		p = p->next;
	}
}
int main() {
	file = fopen("D:\\DEBUG_OUTPUT.txt", "w");
	settings set = {80,60,3};
	srand(time(0));
	for (int i = 1; i <= 20; i++) {
		gen_board(&set, i);
		print_board(head);
		fputc('\n', file);
	}
	fclose(file);
}
#endif // BACK_DEBUG
int main() {
	init();
	while(1)
	{	SetConsoleActiveScreenBuffer(hOutput);
		fflush(stdin);
		char s=_getch();
		switch(s)
		{	case '4':	game_rank();break;//排行榜 
			case '3':	exit_game(high_score, &current_score);break;//退出游戏 
			case '2':	intro();break;//游戏介绍 
			case '1':	start_game();	break; //开始游戏
			default:	continue; //防止你按错键 导致无法进入游戏 
		}
		
		Sleep(1000); 
	}
	system("pause");
	return 0;
}
