#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "struct_def.h"
using namespace std;
void add_board(struct board** head, struct board** tail, int line_id, int x, char type)
// 加板
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
// 删除一行的板
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
// 生成一行的板
{
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		// 大概五分之一概率加一个板，初始化的时候重置一下种子，以后看能不能在设置里面定义概率
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
// 左右为闭区间
{
	while (start_line_id <= end_line_id) {
		gen_board(set, head, tail, start_line_id);
		start_line_id++;
	}
}

int land_on_board(struct settings* set, struct board* board_HEAD, struct player* Player) {
	//返回值为板的类型编号
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

void read_data(struct settings* set) {
	FILE* open = fopen("gameinit.dat", "r");
	char str[100];
	set->map_height = 25;
	set->map_width = 60;
	set->map_board_length = 5;
	set->player_height = 2;
	set->player_width = 1;
	set->velocity_LR = 1;
	set->dp_tpf = 100;
	set->remain_bounce_line = 7;
	strcpy(set->ip_address, "47.103.31.177");
	if (open != NULL) {
		while (!feof(open)) {
			fscanf(open, "%s", str);
			if (!strcmp(str, "[map_height]")) {
				fscanf(open, "%d", &set->map_height);
				continue;
			}
			if (!strcmp(str, "[map_width]")) {
				fscanf(open, "%d", &set->map_width);
				continue;
			}
			if (!strcmp(str, "[map_board_length]")) {
				fscanf(open, "%d", &set->map_board_length);
				continue;
			}
			if (!strcmp(str, "[player_height]")) {
				fscanf(open, "%d", &set->player_height);
				continue;
			}
			if (!strcmp(str, "[player_width]")) {
				fscanf(open, "%d", &set->player_width);
				continue;
			}
			if (!strcmp(str, "[velocity_LR]")) {
				fscanf(open, "%f", &set->velocity_LR);
				continue;
			}
			if (!strcmp(str, "[dp_tpf]")) {
				fscanf(open, "%llu", &set->dp_tpf);
				continue;
			}
			if (!strcmp(str, "[remain_bounce_line]")) {
				fscanf(open, "%d", &set->remain_bounce_line);
				continue;
			}
			if (!strcmp(str, "[IP_address]")) {
				fscanf(open, "%s", set->ip_address);
			}
		}
		fclose(open);
	}
	open = fopen("gameinit.dat", "w");
	fprintf(open, "%s\n%d\n", "[map_height]", set->map_height);
	fprintf(open, "%s\n%d\n", "[map_width]", set->map_width);
	fprintf(open, "%s\n%d\n", "[map_board_length]", set->map_board_length);
	fprintf(open, "%s\n%d\n", "[player_height]", set->player_height);
	fprintf(open, "%s\n%d\n", "[player_width]", set->player_width);
	fprintf(open, "%s\n%f\n", "[velocity_LR]", set->velocity_LR);
	fprintf(open, "%s\n%llu\n", "[dp_tpf]", set->dp_tpf);
	fprintf(open, "%s\n%d\n", "[remain_bounce_line]", set->remain_bounce_line);
	fprintf(open, "%s\n%s", "[IP_address]", set->ip_address);
	fclose(open);
}

bool score_cmp(struct score a, struct score b) {
	return a.player_score > b.player_score;
}

void save_score(struct score* player_score, int* current_score, char* name, int score) {
	strcpy(player_score[*current_score].player_name, name);
	player_score[*current_score].player_score = score;
	player_score[*current_score].if_upload = 0;
	sort(&player_score[0], &player_score[*(current_score)+1], score_cmp);
	if ((*current_score) != MAX_SCORE_NUMBER) {
		(*current_score)++;
	}
	FILE* out = fopen("score_data.dat", "wb");
	fwrite(current_score, sizeof(int), 1, out);
	fwrite(player_score, sizeof(struct score), *current_score, out);
	fclose(out);
}

void read_score(struct score* player_score, int* current_score) {
	memset(player_score, 0, sizeof(struct score) * (MAX_SCORE_NUMBER + 1));
	FILE* read = fopen("score_data.dat", "rb");
	if (read != NULL) {
		fread(current_score, sizeof(int), 1, read);
		fread(player_score, sizeof(struct score), *current_score, read);
		fclose(read);
	}
}

void exit_game(struct score* player_score, int* current_score) {
	FILE* out = fopen("score_data.dat", "wb");
	fwrite(current_score, sizeof(int), 1, out);
	fwrite(player_score, sizeof(struct score), *current_score, out);
	fclose(out);
	exit(0);
}