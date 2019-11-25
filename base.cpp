#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
//#define BACK_DEBUG
struct settings {
	int map_height;        //��ͼ�߶�
	int map_width;         //��ͼ���
	int map_board_length;  //���峤��
	int player_height;     //��Ҹ߶�
	int player_width;      //��ҿ��
	int player_drop_speed; //��������ٶ�
	int dp_tpf;            //����tickˢ�»���
	int dp_tpl;            //����tickˢ��һ��
};
settings set;
struct player {
	int x, y;              //���λ��
};
struct player player1;
struct board {
	int line_id;           //��������
	int x;                 //��˵�λ��
	char type;             //������ͣ�Ĭ��Ϊ0
	struct board* next;    //��һ���ڵ�
};
struct board* head = NULL;
struct board* tail = NULL;
void add_board(int line_id, int x, char type) { //�Ӱ�
	struct board* p = (struct board*)malloc(sizeof(struct board));
	p->line_id = line_id;
	p->x = x;
	p->type = type;
	p->next = NULL;
	if (head == NULL) {
		head = tail = p;
		return;
	}
	tail->next = p;
	tail = p;
}
void delete_board(int line_id) {  //ɾ��һ�еİ�
	struct board* p;
	while (head != NULL && head->line_id == line_id) {
		p = head;
		if (tail == p) {
			tail = NULL;
		}
		head = head->next;
		free(p);
	}
}
void gen_board(struct settings* settings, int line_id) {//����һ�еİ�
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		if (rand() % 5 == 1) { //������֮һ���ʼ�һ���壬��ʼ����ʱ������һ�����ӣ��Ժ��ܲ������������涨�����
			add_board(line_id, i, 0);
			i += board_width;
		}
	}
}
void init() {
	FILE* open = fopen("D:\\gamedata\\gameinit.dat", "r");
	if (open == NULL) {
		printf("ȱ�������ļ�");
		exit(0);
	}
	fscanf_s(open, "%d", &set.map_height);
	fscanf_s(open, "%d", &set.map_width);
	fscanf_s(open, "%d", &set.map_board_length);
	fscanf_s(open, "%d", &set.player_height);
	fscanf_s(open, "%d", &set.player_width);
	fscanf_s(open, "%d", &set.player_drop_speed);
	fscanf_s(open, "%d", &set.dp_tpf);
	fscanf_s(open, "%d", &set.dp_tpl);
	fclose(open);
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