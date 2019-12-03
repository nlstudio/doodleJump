#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG
#include "front.cpp"
#include "struct_def.h"
#include "back_end.h"
#include <time.h>
//#define BACK_DEBUG

void main_iterface();//��ʼ�Ľ��� 
void start_game() {
	add_board(&head, &tail, 1, (set.map_width >> 1) - 1, 1);
	multi_gen_board(&set, &head, &tail, 4, 100);
	int current_board = 100;  //��¼��ǰ�����ɵ�����һ��
	player1.pre_time = GetTickCount();
	player1.pre_board = 1;
	player1.pre_board_type = 1;
	player1.remain_bounce_line = set.remain_bounce_line;
	while (1) {
		unsigned long long current_time = GetTickCount();//�˴β�����ʼ�Ļ�׼ʱ��
		//��ȡ��������
		int key_state = get_key_state();
		//����x����λ��
		player1.x += (key_state * set.velocity_LR);
		if (player1.x <= 1) {
			player1.x = 1;
		}
		if (player1.x >= set.map_width) {
			player1.x = set.map_width;
		}
		//����y����λ��
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
		//�ж��Ƿ�Ӧ�����°�
		if (player1.y >= current_board - 20) {
			multi_gen_board(&set, &head, &tail, current_board + 1, current_board + 100);
			current_board += 100;
		}
		//�ж��Ƿ�׹�����
		if (drop_into_void(&set, &player1)) {
			gameover(&set,&player1);
			return; 
		}
		//�ж��Ƿ���ײ
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
		//�ύ��Ⱦ
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
		//�ж��Ƿ�ɾ��֮ǰ�İ�
		while (head != NULL && head->line_id < set.line) {
			delete_board(&head, &tail, head->line_id);
		}
		//�߳���Ϣ
		if (GetTickCount() - current_time > set.dp_tpf) {
			continue;
		}
		DWORD end_time = set.dp_tpf - (GetTickCount() - current_time);
		Sleep(end_time);
	}
}
void init() {  //��Ϸ���ݵĳ�ʼ��
	read_data(&set);
	player1.x = set.map_width >> 1;
	player1.y = 1;
	Map = (char**)malloc(sizeof(char*) * (set.map_height + 1));
	for (int i = 0; i <= set.map_height; i++) {
		Map[i] = (char*)malloc(sizeof(char) * set.map_width + 1);
		for (int j = 0; j <= set.map_width; j++) {
			Map[i][j] = ' ';
		}
	}
	srand(time(0));
	
	 //�����µĿ���̨������
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//������̿�����������д����
        FILE_SHARE_WRITE,//���建�����ɹ���дȨ��
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOutput = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//������̿�����������д����
        FILE_SHARE_WRITE,//���建�����ɹ���дȨ��
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    //���ػ������Ĺ��
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutBuf, &cci);
	SetConsoleCursorInfo(hOutput, &cci);
	
	
	print_frame(&set);
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
		char s=getchar();
		switch(s)
		{	case '3':	exit(0);break;//�˳���Ϸ 
			case '2':	break;//�һ�û��� 
			case '1':	start_game();	break; //��ʼ��Ϸ 
		}
		fflush(stdin);
		getchar();
		fflush(stdin);
		Sleep(1000); 
	}
	system("pause");
	return 0;
}
