#pragma once
#include <stdlib.h>
#include "struct_def.h"
void add_board(struct board** head, struct board** tail, int line_id, int x, char type)
// �Ӱ�
{
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

void delete_board(struct board** head, struct board** tail, int line_id)
// ɾ��һ�еİ�
{
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

void gen_board(struct settings* settings, struct board** head, struct board** tail, int line_id)
// ����һ�еİ�
{
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		// ������֮һ���ʼ�һ���壬��ʼ����ʱ������һ�����ӣ��Ժ��ܲ������������涨�����
		if (rand() % (20 + settings->line / 8) == 1) {
			add_board(head, tail, line_id, i, 1);
			i += board_width;
			continue;
		}
		if (rand() % 500 == 1) {
			add_board(head, tail, line_id, i, 2);
			i += board_width;
			continue;
		}
		if (rand() % (250 + settings->line / 8) == 1) {
			add_board(head, tail, line_id, i, 3);
			i += board_width;
			continue;
		}
	}
}

void multi_gen_board(struct settings* set, struct board** head, struct board** tail, int start_line_id, int end_line_id)
// ����Ϊ������
{
	while (start_line_id <= end_line_id) {
		gen_board(set, head, tail, start_line_id);
		start_line_id++;
	}
}

int land_on_board(struct settings* set, struct board* board_HEAD, struct player* Player) {
	//����ֵΪ������ͱ��
	while (board_HEAD != NULL && (board_HEAD->line_id != Player->y)) {
		board_HEAD = board_HEAD->next;
	}
	while (board_HEAD != NULL && (board_HEAD->line_id == Player->y)) {
		if (Player->x >= board_HEAD->x && Player->x <= (board_HEAD->x + set->map_board_length - 1)) {
			if (board_HEAD->type == 4) {
				return board_HEAD->type;
			}
			Player->y += 1;
			if (board_HEAD->type == 3) {
				board_HEAD->type = 4;
				return 3;
			}
			
			return board_HEAD->type;
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