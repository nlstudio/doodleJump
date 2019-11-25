#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
//#define BACK_DEBUG
struct settings {
	int map_height;        //地图高度
	int map_width;         //地图宽度
	int map_board_length;  //跳板长度
	int player_height;     //玩家高度
	int player_width;      //玩家宽度
	int player_drop_speed; //玩家下落速度
	int dp_tpf;            //多少tick刷新画面
	int dp_tpl;            //多少tick刷新一行
};
settings set;
struct player {
	int x, y;              //玩家位置
};
struct player player1;
struct board {
	int line_id;           //板所在行
	int x;                 //左端点位置
	char type;             //板的类型，默认为0
	struct board* next;    //下一个节点
};
struct board* head = NULL;
struct board* tail = NULL;
void add_board(int line_id, int x, char type) { //加板
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
void delete_board(int line_id) {  //删除一行的板
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
void gen_board(struct settings* settings, int line_id) {//生成一行的板
	int board_width = settings->map_board_length;
	int map_width = settings->map_width - board_width;
	for (int i = 1; i <= map_width; i++) {
		if (rand() % 5 == 1) { //大概五分之一概率加一个板，初始化的时候重置一下种子，以后看能不能在设置里面定义概率
			add_board(line_id, i, 0);
			i += board_width;
		}
	}
}
void init() {
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