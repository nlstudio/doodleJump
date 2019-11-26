#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
#define F(time) player1.pre_board+set.velocity_UD*time-0.5*set.player_drop_acc*time*time
//#define BACK_DEBUG
extern int get_key_state();
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
	float velocity_UD;            //向上弹起的初速度
	float velocity_LR;            //左右移动的速度
};
settings set;
struct player {
	float x;
	int y;                        //玩家位置,以玩家最下端的中间位置为基准
	unsigned long long pre_time;  //上一次碰撞的时间
	int pre_board;                //上一次碰撞的板的所在行
};
struct player player1;
struct board {
	int line_id;                  //板所在行
	int x;                        //左端点位置
	char type;                    //板的类型，默认为0
	struct board* next;           //下一个节点
};
struct board* head = NULL;
struct board* tail = NULL;
void add_board(int line_id, int x, char type) { //加板
	struct board* p = (struct board*)malloc(sizeof(struct board));
	while (p == NULL) {
		p = (struct board*)malloc(sizeof(struct board));
	}
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
void multi_gen_board(int start_line_id, int end_line_id) {//左右为闭区间
	while (start_line_id <= end_line_id) {
		gen_board(&set, start_line_id);
		start_line_id += 3;
	}
}
int land_on_board(struct board* board_HEAD,player* Player) {
	while (board_HEAD != NULL && (board_HEAD->line_id != Player->y)) {
		board_HEAD = board_HEAD->next;
	}
	while (board_HEAD != NULL && (board_HEAD->line_id == Player->y)) {
		if (Player->x >= board_HEAD->x && Player->x <= (board_HEAD->x + set.map_board_length - 1)) {
			return 1;
		}
	}
	return 0;
}
//int get_key_state(){
//	Sleep(8);
//	if (GetAsyncKeyState(VK_LEFT) & 0x25)return -1;
//	if (GetAsyncKeyState('A') & 0x41)return -1;
//	if (GetAsyncKeyState(VK_RIGHT) & 0x27)return 1;
//	if (GetAsyncKeyState('D') & 0x43)return 1;
//	return 0;
//}
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
}
void start_game() {
	add_board(1, (set.map_width >> 1) - 1, 0);
	multi_gen_board(4, 100);
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
			multi_gen_board(current_board + 1, current_board + 100);
			current_board += 100;
		}
		//判断是否坠入虚空（暂时空缺）
		//判断是否碰撞
		if (land_on_board(head, &player1)) {
			player1.pre_board = player1.y;
			player1.pre_time = current_time;
		}
		//提交渲染（暂时空缺）
		//线程休息
		DWORD end_time = GetTickCount64() - current_time;
		if (end_time > 0) {
			Sleep(end_time);
		}
	}
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