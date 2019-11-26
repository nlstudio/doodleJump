﻿#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG
#include"front.cpp"
#include"standard_head.h"
#define F(time) player1.pre_board+set.velocity_UD*time-0.5*set.player_drop_acc*time*time
//#define BACK_DEBUG
void start_game() {
	add_board(&head, &tail, 1, (set.map_width >> 1) - 1, 0);
	multi_gen_board(&set, &head, &tail, 4, 100);
	int current_board = 100;  //记录当前板生成到了哪一行
	player1.pre_time = GetTickCount64();
	player1.pre_board = 1;
	while (1) {
		unsigned long long current_time = GetTickCount64();//此次操作的基准时间
		//读取键盘输入
		int key_state = get_key_state();
		//更新x方向位置
		player1.x += (key_state * set.velocity_LR);
		if (player1.x <= 1) {
			player1.x = 1;
		}
		if (player1.x >= set.map_width) {
			player1.x = set.map_width;
		}
		//更新y方向位置
		float time = (float)(current_time - player1.pre_time) / 1000;
		player1.y = F(time);
		//判断是否应生成新板
		if (player1.y >= current_board - 20) {
			multi_gen_board(&set, &head, &tail, current_board + 1, current_board + 100);
			current_board += 100;
		}
		//判断是否坠入虚空（暂时空缺）
		if (drop_into_void(&set, &player1)) {
			system("pause");
			exit(0);
		}
		//判断是否碰撞
		if (land_on_board(&set, head, &player1)) {
			player1.pre_board = player1.y;
			player1.pre_time = current_time;
		}
		//提交渲染
		render_boards(head, &set, &Map);
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
		show(&player1, &set);
		//线程休息
		DWORD end_time = GetTickCount64() - current_time;
		if (end_time > 0) {
			Sleep(end_time);
		}
	}
}
void init() {  //游戏数据的初始化
	FILE* open = fopen("D:\\gamedata\\gameinit.dat", "r");
	if (open == NULL) {
		printf("缺少配置文件");
		exit(0);
	}
	fscanf_s(open, "%d", &set.map_height);
	fscanf_s(open, "%d", &set.map_width);
	fscanf_s(open, "%d", &set.map_board_length);
	fscanf_s(open, "%d", &set.player_height);
	fscanf_s(open, "%d", &set.player_width);
	fscanf_s(open, "%f", &set.player_drop_acc);
	fscanf_s(open, "%d", &set.dp_tpf);
	fscanf_s(open, "%d", &set.dp_tpl);
	fscanf_s(open, "%f", &set.velocity_UD);
	fscanf_s(open, "%f", &set.velocity_LR);
	fclose(open);
	player1.x = set.map_width >> 1;
	player1.y = 1;
	Map = (int**)malloc(sizeof(int*) * (set.map_height + 1));
	for (int i = 0; i <= set.map_height; i++) {
		Map[i] = (int*)malloc(sizeof(int) * set.map_width + 1);
		for (int j = 0; j <= set.map_width; j++) {
			Map[i][j] = 0;
		}
	}
	print_frame(&set);
	start_game();
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
	system("pause");
	return 0;
}