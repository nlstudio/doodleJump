﻿#pragma once
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>

/*速度及加速度的单位为行每秒或每秒平方*/
struct settings {
	int map_height;               //地图高度
	int map_width;                //地图宽度
	int map_board_length;         //跳板长度
	int player_height;            //玩家高度
	int player_width;             //玩家宽度
	float player_drop_acc;        //玩家下落加速度
	int dp_tpf;                   //多少tick刷新画面
	int dp_tpl;                   //多少tick刷新一行
	int line;                     //当前游戏最底端所在行的编号
	float velocity_UD;            //向上弹起的初速度
	float velocity_LR;            //左右移动的速度
};

struct player {
	float x;
	int y;                        //玩家位置,以玩家最下端的中间位置为基准
	unsigned long long pre_time;  //上一次碰撞的时间
	int pre_board;                //上一次碰撞的板的所在行
};

struct board {
	int line_id;                  //板所在行
	int x;                        //左端点位置
	char type;                    //板的类型，默认为0
	struct board* next;           //下一个节点
};

int get_key_state()
{
	Sleep(8);
	if (GetAsyncKeyState(VK_LEFT) & 0x25)return -1;
	if (GetAsyncKeyState('A') & 0x41)return -1;
	if (GetAsyncKeyState(VK_RIGHT) & 0x27)return 1;
	if (GetAsyncKeyState('D') & 0x44)return 1;
	return 0;
}

void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos; pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void print_frame(struct settings* set)//游戏开始时，打印边框 
{
	for (int i = 1; i <= set->map_height + 1; i++)
	{
		gotoxy(0, i); printf("|"); gotoxy(set->map_width + 1, i); printf("|");
	}
	for (int i = 1; i <= set->map_width; i++)
	{
		gotoxy(i, 0); printf("_"); gotoxy(i, set->map_height + 1); printf("_");
	}

}

void render_boards(struct board* head, struct settings* set, int*** Map)//将链表生成数组Map 
{
	struct board* p = head;
	while (p != NULL)
	{
		for (int j = p->x; j < p->x + set->map_board_length; j++)
		{
			if (set->map_height - p->line_id + set->line > 0 && set->line <= p->line_id)
				//(*Map)[set->map_height - p->line_id + set->line][j] = (int)(p->type);
				(*Map)[set->map_height - p->line_id + set->line][j] = 1;
		}
		p = p->next;
	}

}

void add_board(struct board** head, struct board** tail, int line_id, int x, char type) { //加板
	struct board* p = (struct board*)malloc(sizeof(struct board));
	while (p == NULL) {
		p = (struct board*)malloc(sizeof(struct board));
	}
	p->line_id = line_id;
	p->x = x;
	p->type = type;
	p->next = NULL;
	if (*head == NULL) {
		*head = *tail = p;
		return;
	}
	(*tail)->next = p;
	*tail = p;
}

void delete_board(struct board** head, struct board** tail, int line_id) {  //删除一行的板
	struct board* p;
	while (*head != NULL && (*head)->line_id == line_id) {
		p = *head;
		if (*tail == p) {
			*tail = NULL;
		}
		*head = (*head)->next;
		free(p);
	}
}

void gen_board(struct settings* settings, struct board** head, struct board** tail, int line_id) {//生成一行的板
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		if (rand() % 5 == 1) { //大概五分之一概率加一个板，初始化的时候重置一下种子，以后看能不能在设置里面定义概率
			add_board(head, tail, line_id, i, 0);
			i += board_width;
		}
	}
}

void multi_gen_board(struct settings* set, struct board** head, struct board** tail, int start_line_id, int end_line_id) {//左右为闭区间
	while (start_line_id <= end_line_id) {
		gen_board(set, head, tail, start_line_id);
		start_line_id += 3;
	}
}

int land_on_board(struct settings* set, struct board* board_HEAD, struct player* Player) {
	while (board_HEAD != NULL && (board_HEAD->line_id != Player->y)) {
		board_HEAD = board_HEAD->next;
	}
	while (board_HEAD != NULL && (board_HEAD->line_id == Player->y)) {
		if (Player->x >= board_HEAD->x && Player->x <= (board_HEAD->x + set->map_board_length - 1)) {
			return 1;
		}
	}
	return 0;
}

int drop_into_void(struct settings* set, struct player* Player) {
	if (Player->y < set->line) {
		return 1;
	}
	return 0;
}