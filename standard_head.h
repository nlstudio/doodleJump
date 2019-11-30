#pragma once
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>

/*�ٶȼ����ٶȵĵ�λΪ��ÿ���ÿ��ƽ��*/
struct settings {
	int map_height;               //��ͼ�߶�
	int map_width;                //��ͼ���
	int map_board_length;         //���峤��
	int player_height;            //��Ҹ߶�
	int player_width;             //��ҿ��
	float player_drop_acc;        //���������ٶ�
	unsigned long long dp_tpf;    //����tickˢ�»���
	int dp_tpl;                   //����tickˢ��һ��
	int line;                     //��ǰ��Ϸ��׶������еı��
	float velocity_UD;            //���ϵ���ĳ��ٶ�
	float velocity_LR;            //�����ƶ����ٶ�
	int remain_bounce_line;       //�����������������
};

struct player {
	float x;
	int y;                        //���λ��,��������¶˵��м�λ��Ϊ��׼
	unsigned long long pre_time;  //��һ����ײ��ʱ��
	int pre_board;                //��һ����ײ�İ��������
	int remain_bounce_line;       //�����������������
};

struct board {
	int line_id;                  //��������
	int x;                        //��˵�λ��
	char type;                    //������ͣ�Ĭ��Ϊ0
	struct board* next;           //��һ���ڵ�
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

void print_frame(struct settings* set)//��Ϸ��ʼʱ����ӡ�߿� 
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

void render_boards(struct board* head, struct settings* set, int*** Map)//��������������Map 
{
	struct board* p = head;
	for(int i=1;i<=set->map_height;i++)
	for(int j=1;j<=set->map_width;j++)
	{(*Map)[i][j] = 0;
	 } 
	while (p != NULL)
	{	if (set->map_height - p->line_id + set->line > 0 && set->line <= p->line_id)
		{	for (int j = p->x; j < p->x + set->map_board_length; j++)
			{	
				//(*Map)[set->map_height - p->line_id + set->line][j] = (int)(p->type);
				(*Map)[set->map_height - p->line_id + set->line][j] = 1;
			}
		
		} 
		p = p->next;
	}

}

void add_board(struct board** head, struct board** tail, int line_id, int x, char type) { //�Ӱ�
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

void delete_board(struct board** head, struct board** tail, int line_id) {  //ɾ��һ�еİ�
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

void gen_board(struct settings* settings, struct board** head, struct board** tail, int line_id) {//����һ�еİ�
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		if (rand() % 3 == 1) { //������֮һ���ʼ�һ���壬��ʼ����ʱ������һ�����ӣ��Ժ��ܲ������������涨�����
			add_board(head, tail, line_id, i, 0);
			i += board_width;
		}
	}
}

void multi_gen_board(struct settings* set, struct board** head, struct board** tail, int start_line_id, int end_line_id) {//����Ϊ������
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
		board_HEAD = board_HEAD->next;
	}
	return 0;
}

int drop_into_void(struct settings* set, struct player* Player) {
	if (Player->y < set->line) {
		return 1;
	}
	return 0;
}
